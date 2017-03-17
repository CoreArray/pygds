# import c library
import pygds.ccall as cc



# ===========================================================================

def cleanup_gds(filename, verbose=True):
	cc.tidy_up(filename, verbose)



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
		print("file id: %d" % self.fileid)





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
		return ""

	def description(self):
		return ""

	def show(self):
		print("ok: %d" % self.ok)




