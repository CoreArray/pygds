# import c library
import pygds.ccall as cc



# ===========================================================================

class gdsfile:
	"""
	Common base class for GDS files
	"""

	def __init__(self):
		self.filename = ''
		self.fileid = -1;

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

	def show(self):
		print("file id: %d" % self.fileid)





# ===========================================================================

class gdsnode:
	"""
	Common base class for GDS node
	"""

	def __init__(self):
		self.ok = -1;

	def show(self):
		print("ok: %d" % self.ok)




