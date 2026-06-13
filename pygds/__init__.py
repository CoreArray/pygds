# import numpy
import numpy as np
# import os
import os
# import c library
import pygds.ccall as cc


## export version number
__version__ = '0.1.0'



# ===========================================================================

def cleanup_gds(filename, verbose=True):
	"""Clean up fragments

	Clean up the fragments of a CoreArray Genomic Data Structure (GDS) file.

	Parameters
	----------
	filename : str
			the file name of an existing GDS file
	verbose : bool
		if True, show information

	Returns
	-------
	None
	"""
	cc.tidy_up(filename, verbose)


def get_include():
	"""
	Return the directory that contains the pygds \\*.h header files.
	Extension modules that need to compile against pygds should use this
	function to locate the appropriate include directory.
	Notes
	-----
	When using ``distutils``, for example in ``setup.py``.
	::
		import pygds as gds
		...
		Extension('pkg_name', ...
			include_dirs=[ gds.get_include() ])
		...
	"""
	import pygds
	return os.path.join(os.path.dirname(pygds.__file__), 'include')


def get_example_path(filename=None):
	"""Example files

	Return a file name in the folder of example data.

	Parameters
	----------
	filename : str
		a file name in the folder of example data, or None for returning the path of example folder

	Returns
	-------
	string

	Examples
	--------
	>>> get_example_path('ceu_exon.gds')
	"""
	import pygds
	s = os.path.dirname(pygds.__file__)
	if filename is None:
		return os.path.join(s, 'data')
	else:
		return os.path.join(s, 'data', filename)


# ---------------------------------------------------------------------------
# Internal helpers for writing data
# ---------------------------------------------------------------------------

# numpy dtype kind -> GDS storage name (used when storage is not specified)
def _infer_storage(val):
	if isinstance(val, str):
		return 'string'
	arr = np.asarray(val)
	k = arr.dtype.kind
	if k in ('U', 'S', 'O'):
		return 'string'
	if k == 'b':
		return 'int8'
	return str(arr.dtype)


def _prepare_value(val):
	"""Normalise 'val' for the C layer.

	Returns (obj, dim, is_string) where obj is either a C-contiguous numpy
	array (numeric) or a flat list of str (character), dim is the C-order
	shape as a list, and is_string indicates the kind.
	"""
	if isinstance(val, str):
		return ([val], [1], True)
	arr = np.asarray(val)
	if arr.ndim == 0:
		arr = arr.reshape(1)
	if arr.dtype.kind in ('U', 'S', 'O'):
		flat = ['' if x is None else str(x) for x in arr.ravel(order='C').tolist()]
		return (flat, list(arr.shape), True)
	arr = np.ascontiguousarray(arr)
	return (arr, list(arr.shape), False)


def apply_gdsn(nodes, margins, fun, *args, as_is='none', **kwargs):
	"""Apply a function over a margin of one or more array nodes

	Iterate in lock-step over the given margin of each node; for each index,
	call ``fun(slice0[, slice1, ...], *args, **kwargs)`` where each slice is
	the corresponding sub-array with its margin axis removed.

	Parameters
	----------
	nodes : gdsnode or a list of gdsnode
	margins : int or a list of int
		the axis (0-based, numpy C-order) to iterate over, per node
	fun : callable
	as_is : str
		'none', 'list', or 'unlist' (see gdsnode.apply)

	Returns
	-------
	None, list, or numpy array depending on 'as_is'
	"""
	single = isinstance(nodes, gdsnode)
	if single:
		nodes = [nodes]
		margins = [margins]
	if len(nodes) != len(margins):
		raise ValueError("'nodes' and 'margins' must have the same length.")

	dims = [n.description()['dim'] for n in nodes]
	counts = [dims[k][margins[k]] for k in range(len(nodes))]
	N = counts[0]
	if any(c != N for c in counts):
		raise ValueError("All nodes must have the same length along their margins.")

	results = []
	for i in range(N):
		slices = []
		for k, n in enumerate(nodes):
			m = margins[k]
			dim = dims[k]
			sel = [None] * len(dim)
			bv = np.zeros(dim[m], dtype=bool)
			bv[i] = True
			sel[m] = bv
			s = n.readex(sel)
			s = np.asarray(s)
			if s.ndim == len(dim) and s.shape[m] == 1:
				s = np.squeeze(s, axis=m)
			slices.append(s)
		r = fun(slices[0], *args, **kwargs) if single \
			else fun(*slices, *args, **kwargs)
		if as_is != 'none':
			results.append(r)

	if as_is == 'list':
		return results
	elif as_is == 'unlist':
		if not results:
			return np.array([])
		return np.concatenate([np.atleast_1d(np.asarray(r)).ravel()
			for r in results])
	return None


# ===========================================================================

class gdsfile:
	"""
	Common base class for GDS files
	"""

	def __init__(self):
		self.filename = ''
		self.fileid = -1

	def __del__(self):
		self.close()


	def create(self, filename, allow_dup=False):
		"""Create a GDS file

		Create a new CoreArray Genomic Data Structure (GDS) file.

		Parameters
		----------
		filename : str
			the file name of a new GDS file to be created
		allow_dup : bool
			if True, it is allowed to open a GDS file with read-only mode when it has been opened in the same session

		Returns
		-------
		None

		See Also
		--------
		open : open an existing GDS file
		close: close a GDS file
		"""
		self.fileid = cc.create_gds(filename, allow_dup)
		self.filename = os.path.abspath(filename)


	def open(self, filename, readonly=True, allow_dup=False):
		"""Open an GDS file

		Open an existing file of CoreArray Genomic Data Structure (GDS) for reading or writing.

		Parameters
		----------
		filename : str
			the file name of an existing GDS file
		readonly : bool
			if True, the file is opened read-only; otherwise, it is allowed to write data to the file
		allow_dup : bool
			if True, it is allowed to open a GDS file with read-only mode when it has been opened in the same session

		Returns
		-------
		None

		See Also
		--------
		create : create a GDS file
		close: close a GDS file

		Examples
		--------
		>>> fn = get_example_path('ceu_exon.gds')
		>>> f = gds.gdsfile()
		>>> f.open(fn)
		>>> f.show()
		>>> f.close()
		"""
		self.fileid = cc.open_gds(filename, readonly, allow_dup)
		self.filename = os.path.abspath(filename)


	def close(self):
		"""Close a GDS file

		Close a CoreArray Genomic Data Structure (GDS) file.

		Returns
		-------
		None

		See Also
		--------
		create : create a GDS file
		open : open an existing GDS file
		"""
		cc.close_gds(self.fileid)
		self.fileid = -1
		self.filename = ''


	def sync(self):
		"""Synchronize a GDS file

		Write the data cached in memory to disk.

		Returns
		-------
		None
		"""
		cc.sync_gds(self.fileid)


	def filesize(self):
		"""Get the file size

		Get the size of a GDS file.

		Returns
		-------
		double
		"""
		return cc.filesize(self.fileid)


	def root(self):
		"""GDS root node

		Get the root of a GDS file.

		Returns
		-------
		gdsnode
		"""
		v = gdsnode()
		(v.idx, v.pid) = cc.root_gds(self.fileid)
		return v


	def index(self, path, silent=False):
		"""Get a GDS node

		Get a specified GDS node with a given path

		Parameters
		----------
		path : str
			the path specifying a GDS node with '/' as a separator
		silent : bool
			if True, return None if the specified node does not exist; otherwise raise an error

		Returns
		-------
		gdsnode
		"""
		v = gdsnode()
		v.idx, v.pid = cc.index_gds(self.fileid, path, silent)
		if v.idx >= 0:
			return v
		else:
			return None


	def show(self, attribute=False, all=False):
		"""Display a GDS file

		Display the structure of a GDS file.

		Parameters
		----------
		attribute : bool
			if True, show the attribute(s)
		all : bool
			if False, hide GDS nodes with an attribute "R.invisible"

		Returns
		-------
		None
		"""
		sz = self.filesize()
		if sz >= 1073741824:
			s = '(%.1fG)' % (sz/1073741824)
		elif sz >= 1048576:
			s = '(%.1fM)' % (sz/1048576)
		elif sz >= 1024:
			s = '(%.1fK)' % (sz/1024)
		else:
			s = '(%gB)' % sz
		print('File:', self.filename, s)
		self.root().show(attribute, all)


	def diagnosis(self):
		"""Diagnose the GDS file

		Returns
		-------
		dict : {num_fragment, size} - the number of data fragments and the
		total file size in bytes
		"""
		return cc.diagnosis_gds(self.fileid)





# ===========================================================================

# TODO: require Py_ssize_t (Python >=2.5)


class gdsnode:
	"""
	Common base class for GDS node
	"""

	def __init__(self):
		self.idx = -1
		self.pid = 0


	def ls(self, inc_hidden=False):
		"""Get a list of names

		Get a list of names for its child nodes.

		Parameters
		----------
		inc_hidden : bool
			whether including hidden variables or folders

		Returns
		-------
		a list of strings
		"""
		return cc.ls_gdsn(self.idx, self.pid, inc_hidden)


	def index(self, path, silent=False):
		"""Get a GDS node

		Get a specified GDS node with a given path

		Parameters
		----------
		path : str
			the path specifying a GDS node with '/' as a separator
		silent : bool
			if True, return None if the specified node does not exist; otherwise raise an error

		Returns
		-------
		gdsnode
		"""
		v = gdsnode()
		v.idx, v.pid = cc.index_gdsn(self.idx, self.pid, path, silent)
		if v.idx >= 0:
			return v
		else:
			return None


	def name(self, full=False):
		"""Get the node name

		Get the variable name of a GDS node.

		Parameters
		----------
		full : bool
			if False, return the node name; otherwise the name with a full path

		Returns
		-------
		string
		"""
		return cc.name_gdsn(self.idx, self.pid, full)


	def rename(self, newname):
		"""Rename a GDS node

		Rename a GDS node.

		Parameters
		----------
		newname : str
			the new name of a specified node

		Returns
		-------
		None
		"""
		cc.rename_gdsn(self.idx, self.pid, newname)


	def description(self):
		"""Variable description

		Get the description of a GDS node.

		Returns
		-------
		dictionary:
		    name: the variable name of a specified node
		    fullname: the full name of a specified node
		    storage: the storage mode in the GDS file
		    trait: the description of data field, like "Int8"
		    type: a factor indicating the storage mode
		        Label -- a label node,
		        Folder -- a directory,
		        VFolder -- a virtual folder linking to another GDS file,
		        Raw -- raw data
		        Integer -- integers,
		        Factor -- factor values,
		        Logical -- logical values
		        Real -- floating numbers,
		        String -- characters,
		    Unknown -- unknown type
		    dim: the dimension of data field
		    encoder: encoder for compressed data, such like "ZIP"
		    compress: the compression method: "", "ZIP.max", etc
		    cpratio: data compression ratio, NaN indicates no compression
		    size: the size of data stored in the GDS file
		    good: logical, indicates the state of GDS file, e.g., False if the virtual folder fails to link the target GDS file
		    hidden: logical, True if it is a hidden node
		    message: if applicable, messages of the GDS node, such like error messages, log information
		"""
		return cc.desp_gdsn(self.idx, self.pid)


	def getattr(self):
		"""Get attributes

		Get the attributes of a GDS node.

		Returns
		-------
		dictionary of attributes
		"""
		return cc.getattr_gdsn(self.idx, self.pid)


	def read(self, start=None, count=None, cvt=''):
		"""Read data

		Read data field of a GDS node.

		Parameters
		----------
		start : a list of integers
			starting from 0 for each dimension component
		count : a list of integers
			the length of each dimnension, -1 indicates that all entries along that dimension should be read, starting from 'start'
		cvt : str
			'': no conversion; 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64': signed and unsigned integer;
			'utf8': UTF-8 string; 'utf16': UTF-16 string

		Returns
		-------
		a numpy array object
		"""
		return cc.read_gdsn(self.idx, self.pid, start, count, cvt)


	def readex(self, sel, cvt=''):
		"""Read data with selection

		Read data field of a GDS node with selection

		Parameters
		----------
		sel : a list of bool vectors
			bool vectors indicating dimension selection, or None for all entries along that dimension
		cvt : str
			'': no conversion; 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64': signed and unsigned integer;
			'utf8': UTF-8 string; 'utf16': UTF-16 string

		Returns
		-------
		a numpy array object
		"""
		return cc.read2_gdsn(self.idx, self.pid, sel, cvt)


	# -------------------------------------------------------------------
	# Node creation and data writing
	# -------------------------------------------------------------------

	def add(self, name, val=None, storage='', compress='', valdim=None,
			closezip=True, replace=False, visible=True, check=True,
			maxlen=0, offset=float('nan'), scale=float('nan')):
		"""Add a new GDS node

		Create a new GDS node under this folder, optionally writing data.

		Parameters
		----------
		name : str
			the name of the new node
		val : array-like, optional
			initial data (a numpy array, a sequence, or a list of strings);
			if None, an empty node is created
		storage : str
			the storage mode, e.g. 'int8', 'int32', 'float64', 'string',
			'packedreal16', 'fstring'; if '', it is inferred from 'val'
		compress : str
			the compression method, e.g. '', 'ZIP', 'ZIP_RA', 'LZMA',
			'LZMA_RA', 'LZ4', 'LZ4_RA'
		valdim : a list of ints, optional
			reset the dimensions after writing
		closezip : bool
			if True, close the compression after writing the initial data
		replace : bool
			if True, replace an existing node of the same name
		visible : bool
			if False, mark the node hidden (R.invisible)
		maxlen : int
			maximum length for fixed-length strings (fstring*)
		offset, scale : float
			offset / scale for packed real numbers (packedreal*)

		Returns
		-------
		gdsnode
		"""
		if storage == '' or storage is None:
			storage = _infer_storage(val) if val is not None else ''
		v = gdsnode()
		v.idx, v.pid = cc.add_gdsn(self.idx, self.pid, name, storage, compress,
			bool(visible), bool(replace), int(maxlen),
			float(offset), float(scale))
		if val is not None:
			obj, dim, is_str = _prepare_value(val)
			cc.writeall_gdsn(v.idx, v.pid, obj, [int(d) for d in dim])
			if compress != '' and closezip:
				cc.readmode_gdsn(v.idx, v.pid)
			if valdim is not None:
				cc.setdim_gdsn(v.idx, v.pid, [int(d) for d in valdim], False)
		return v


	def addfolder(self, name, type='directory', gds_fn='', replace=False,
			visible=True):
		"""Add a new folder node

		Parameters
		----------
		name : str
			the name of the new folder
		type : str
			'directory' for a sub folder, or 'virtual' to link another GDS file
		gds_fn : str
			the file name of an existing GDS file (when type='virtual')
		replace : bool
			if True, replace an existing node of the same name
		visible : bool
			if False, mark the folder hidden

		Returns
		-------
		gdsnode
		"""
		v = gdsnode()
		v.idx, v.pid = cc.addfolder_gdsn(self.idx, self.pid, name, type, gds_fn,
			bool(visible), bool(replace))
		return v


	def delete(self, child=None, force=False):
		"""Delete a GDS node

		Delete this node, or a named child node when 'child' is given.

		Parameters
		----------
		child : str, optional
			the name of a child node to delete; if None, delete this node
		force : bool
			if True, also delete a non-empty folder
		"""
		if child is not None:
			n = self.index(child)
			if n is not None:
				n.delete(force=force)
			return
		cc.delete_gdsn(self.idx, self.pid, bool(force))
		self.idx = -1
		self.pid = 0


	def append(self, val):
		"""Append data to an array node

		Parameters
		----------
		val : array-like
			a numpy array, a sequence, or a list of strings
		"""
		obj, _dim, _is_str = _prepare_value(val)
		cc.append_gdsn(self.idx, self.pid, obj)


	def write(self, val, start=None, count=None):
		"""Write data to a node

		Write data to the whole array, or to a sub-region when both 'start'
		and 'count' are given.

		Parameters
		----------
		val : array-like
			a numpy array, a sequence, or a list of strings
		start : a list of ints, optional
			0-based starting position for each dimension
		count : a list of ints, optional
			the length along each dimension
		"""
		obj, dim, _is_str = _prepare_value(val)
		if start is None and count is None:
			cc.writeall_gdsn(self.idx, self.pid, obj, [int(d) for d in dim])
		elif start is not None and count is not None:
			cc.write_gdsn(self.idx, self.pid, obj,
				[int(s) for s in start], [int(c) for c in count])
		else:
			raise ValueError("'start' and 'count' should be both None or both set.")


	def setdim(self, dim, permute=False):
		"""Set the dimensions of an array node

		Parameters
		----------
		dim : a list of ints
			the new dimensions
		permute : bool
			if True, permute the data when reshaping
		"""
		cc.setdim_gdsn(self.idx, self.pid, [int(d) for d in dim], bool(permute))


	def compression(self, compress=''):
		"""Set the compression mode of a node

		Parameters
		----------
		compress : str
			the compression method, e.g. '', 'ZIP', 'LZMA_RA', 'LZ4_RA'

		Returns
		-------
		gdsnode (self)
		"""
		cc.compression_gdsn(self.idx, self.pid, compress)
		return self


	def readmode(self):
		"""Close the compression and switch to read mode

		Returns
		-------
		gdsnode (self)
		"""
		cc.readmode_gdsn(self.idx, self.pid)
		return self


	def putattr(self, name, val):
		"""Add or set an attribute

		Parameters
		----------
		name : str
			the attribute name
		val : int, float, bool, str, or a list of these
			the attribute value
		"""
		cc.putattr_gdsn(self.idx, self.pid, name, val)


	def delattr(self, name):
		"""Delete an attribute

		Parameters
		----------
		name : str
			the attribute name
		"""
		cc.delattr_gdsn(self.idx, self.pid, name)


	def exist(self, path):
		"""Test whether a path exists under this folder node

		Parameters
		----------
		path : str
			the path with '/' as a separator

		Returns
		-------
		bool
		"""
		return cc.exist_gdsn(self.idx, self.pid, path)


	def is_sparse(self):
		"""Test whether this node is a sparse array

		Returns
		-------
		bool
		"""
		return cc.is_sparse_gdsn(self.idx, self.pid)


	# -------------------------------------------------------------------
	# Relocation, copy, caching and embedded files
	# -------------------------------------------------------------------

	def moveto(self, loc_node, relpos='after'):
		"""Move this node relative to another node

		Parameters
		----------
		loc_node : gdsnode
			the reference node (must be in the same folder unless relpos='into')
		relpos : str
			'after', 'before', 'replace', 'replace+rename', or 'into'
			(move into the folder 'loc_node')
		"""
		cc.moveto_gdsn(self.idx, self.pid, loc_node.idx, loc_node.pid, relpos)


	def copyto(self, name, source):
		"""Copy a source node into this folder under a new name

		Parameters
		----------
		name : str
			the name of the copy
		source : gdsnode
			the node to copy

		Returns
		-------
		gdsnode
		"""
		v = gdsnode()
		v.idx, v.pid = cc.copyto_gdsn(self.idx, self.pid, name,
			source.idx, source.pid)
		return v


	def assign(self, source):
		"""Assign (deep copy) the content of a source node to this node

		Parameters
		----------
		source : gdsnode
			the source node
		"""
		cc.assign_gdsn(self.idx, self.pid, source.idx, source.pid)


	def cache(self):
		"""Cache the data associated with this node in memory"""
		cc.cache_gdsn(self.idx, self.pid)


	def unload(self):
		"""Unload this node from memory"""
		cc.unload_gdsn(self.idx, self.pid)


	def addfile(self, name, filename, compress='', replace=False, visible=True):
		"""Embed an external file as a stream container under this folder

		Parameters
		----------
		name : str
			the name of the new node
		filename : str
			the path of the external file to embed
		compress : str
			the compression method
		replace : bool
			if True, replace an existing node of the same name
		visible : bool
			if False, mark the node hidden

		Returns
		-------
		gdsnode
		"""
		v = gdsnode()
		v.idx, v.pid = cc.addfile_gdsn(self.idx, self.pid, name, filename,
			compress, bool(replace), bool(visible))
		return v


	def getfile(self, out_filename):
		"""Extract an embedded file node to an external file

		Parameters
		----------
		out_filename : str
			the path of the output file
		"""
		cc.getfile_gdsn(self.idx, self.pid, out_filename)


	# -------------------------------------------------------------------
	# Computation over the data
	# -------------------------------------------------------------------

	def apply(self, margin, fun, *args, as_is='none', **kwargs):
		"""Apply a function over a margin of this array node

		Iterate over axis 'margin' (0-based, in numpy C-order), calling
		``fun(slice, *args, **kwargs)`` for each index, where 'slice' is the
		sub-array with the margin axis removed.

		Parameters
		----------
		margin : int
			the axis to iterate over (0-based)
		fun : callable
			the function applied to each slice
		as_is : str
			'none' (return None), 'list' (collect results into a list), or
			'unlist' (concatenate results into a single numpy array)

		Returns
		-------
		None, list, or numpy array depending on 'as_is'
		"""
		return apply_gdsn(self, margin, fun, *args, as_is=as_is, **kwargs)


	def digest(self, algorithm='md5'):
		"""Compute a hash digest of this node's data

		The digest is computed over the raw element bytes in storage order
		(little-endian for numeric data; UTF-8 for strings). For plain numeric
		arrays this matches R gdsfmt's ``digest.gdsn``.

		Parameters
		----------
		algorithm : str
			a hashlib algorithm name, e.g. 'md5', 'sha1', 'sha256', 'sha512'

		Returns
		-------
		str : the hex digest
		"""
		import hashlib
		data = self.read()
		arr = np.asarray(data)
		if arr.dtype.kind in ('U', 'S', 'O'):
			flat = arr.ravel(order='C').tolist()
			b = ''.join('' if x is None else str(x) for x in flat).encode('utf-8')
		else:
			b = np.ascontiguousarray(arr).tobytes()
		h = hashlib.new(algorithm)
		h.update(b)
		return h.hexdigest()


	def summarize(self):
		"""Summarize the data of this node

		Returns
		-------
		dict : for numeric data {min, max, mean, num_na, n}; otherwise {n}
		"""
		d = self.description()
		data = self.read()
		arr = np.asarray(data)
		if d['type'] in ('Integer', 'Factor', 'Logical', 'Real') and \
				arr.dtype.kind not in ('U', 'S', 'O'):
			v = arr.astype(float, copy=False)
			finite = v[np.isfinite(v)]
			n = int(v.size)
			if finite.size:
				return dict(min=float(finite.min()), max=float(finite.max()),
					mean=float(finite.mean()), num_na=n - int(finite.size), n=n)
			return dict(min=float('nan'), max=float('nan'), mean=float('nan'),
				num_na=n, n=n)
		return dict(n=int(arr.size))


	def show(self, attribute=False, all=False, expand=True):
		"""Display a GDS node

		Display the structure of a GDS node.

		Parameters
		----------
		attribute : bool
			if True, show the attribute(s)
		all : bool
			if False, hide GDS nodes with an attribute "R.invisible"

		Returns
		-------
		None
		"""

		def enum(node, prefix, fullname, last, attr, expand):
			d = node.description()
			if d['type'] == 'Label':
				lText = ' '; rText = ' '
			elif d['type'] == 'VFolder':
				if d['good']:
					lText = '[ -->'; rText = ']'
				else:
					lText = '[ -X-'; rText = ']'; expand = False
			elif d['type'] == 'Folder':
				lText = '['; rText = ']'
			elif d['type'] == 'Unknown':
				lText = '   -X-'; rText = ''; expand = False
			else:
				lText = '{'; rText = '}'

			s = ''.join([ prefix, '+ ', node.name(fullname), '   ', lText,
				' ', d['trait'] ])

			# if logical, factor, list, or data.frame
			if d['type'] == 'Logical':
				s = s + ',logical'
			elif d['type'] == 'Factor':
				s = s + ',factor'

			# show the dimension
			if d['dim'] != None:
				s = s + ' ' + ('x'.join([str(i) for i in d['dim']]))

			# show compression
			if d['encoder'] != '':
				if attribute:
					s = s + ' ' + d['compress']
				else:
					s = s + ' ' + d['encoder']

			if np.isfinite(d['cpratio']):
				if d['cpratio'] >= 0.10:
					s = s + ('(%0.1f%%)' % (100*d['cpratio']))
				else:
					s = s + ('(%0.2f%%)' % (100*d['cpratio']))

			sz = d['size']
			if np.isfinite(sz):
				if sz >= 1073741824:
					s = s + ', %.1fG' % (sz/1073741824)
				elif sz >= 1048576:
					s = s + ', %.1fM' % (sz/1048576)
				elif sz >= 1024:
					s = s + ', %.1fK' % (sz/1024)
				else:
					s = s + ', %gB' % sz

			s = s + ' ' + rText

			# attributes
			at = node.getattr()
			if at != None:
				s = s + ' *'
				if attr:
					s = s + '< ' + str(at)

			print(s)

			if expand and d['type']=='Folder':
				nm = node.ls(all)
				for i in range(len(nm)):
					n = len(prefix)
					if i < len(nm)-1:
						if n >= 3:
							if last:
								s = prefix[:n-3] + '   |--'
							else:
								s = prefix[:n-3] + '|  |--'
						else:
							s = '|--'
					else:
						if n >= 3:
							if last:
								s = prefix[:n-3] + '   \\--'
							else:
								s = prefix[:n-3] + '|  \\--'
						else:
							s = '\\--'
					enum(node.index(nm[i]), s, False, i>=len(nm)-1, attr, expand)

			return None

		enum(self, "", True, True, attribute, expand)

