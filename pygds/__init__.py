# import c library
import pygds.ccall as cc
# import numpy
import numpy as np



# ===========================================================================

def cleanup_gds(filename, verbose=True):
	cc.tidy_up(filename, verbose)



# ===========================================================================

class gdsfile:
	'''
	Common base class for GDS files
	'''

	def __init__(self):
		self.filename = ''
		self.fileid = -1

	def __del__(self):
		self.close()

	def create(self, filename, allow_dup=False):
		self.fileid = cc.create_gds(filename, allow_dup)
		self.filename = filename

	def open(self, filename, readonly=True, allow_dup=False):
		self.fileid = cc.open_gds(filename, readonly, allow_dup)
		self.filename = filename

	def close(self):
		cc.close_gds(self.fileid)
		self.fileid = -1
		self.filename = ''

	def sync(self):
		cc.sync_gds(self.fileid)

	def filesize(self):
		return cc.filesize(self.fileid)

	def root(self):
		v = gdsnode()
		(v.idx, v.pid) = cc.root_gds(self.fileid)
		return v

	def index(self, path, silent=False):
		v = gdsnode()
		v.idx, v.pid = cc.index_gds(self.fileid, path, silent)
		if v.idx >= 0:
			return v
		else:
			return None

	def show(self):
		print('File:', self.filename)
		self.root().show()





# ===========================================================================

# TODO: require Py_ssize_t (Python >=2.5)


class gdsnode:
	'''
	Common base class for GDS node
	'''

	def __init__(self):
		self.idx = -1
		self.pid = 0

	def ls(self, inc_hidden=False):
		return cc.ls_gdsn(self.idx, self.pid, inc_hidden)

	def index(self, path, silent=False):
		v = gdsnode()
		v.idx, v.pid = cc.index_gdsn(self.idx, self.pid, path, silent)
		if v.idx >= 0:
			return v
		else:
			return None

	def name(self, full=False):
		return cc.name_gdsn(self.idx, self.pid, full)

	def rename(self, newname):
		cc.rename_gdsn(self.idx, self.pid, newname)

	def description(self):
		return cc.desp_gdsn(self.idx, self.pid)

	def getattr(self):
		return cc.getattr_gdsn(self.idx, self.pid)

	def read(self, start=None, count=None, cvt=''):
		return cc.read_gdsn(self.idx, self.pid, start, count, cvt)

	def readex(self):
		return 1


	def show(self, all=False, attribute=False, expand=True):

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
				lText = '	 -X-'; rText = ''; expand = False
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

