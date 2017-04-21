# import numpy
import numpy as np
# import os
import os
# import c library
import pygds.ccall as cc



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
	if filename == None:
		return os.path.join(s, 'data')
	else:
		return os.path.join(s, 'data', filename)



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
		self.filename = filename


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
		self.filename = filename


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
			bool vectors indicating dimension selection
		cvt : str
			'': no conversion; 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64': signed and unsigned integer;
			'utf8': UTF-8 string; 'utf16': UTF-16 string

		Returns
		-------
		a numpy array object
		"""
		return None


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

