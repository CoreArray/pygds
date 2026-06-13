import os
import sys
import sysconfig

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import numpy


class build_ext_cppstd(build_ext):
	"""Apply the C++ standard flag to C++ translation units only.

	CoreArray ships one C source (dParallel_Ext.c); clang rejects -std=c++NN
	on C files, so the flag must not be passed globally.
	"""
	CXX_STD = '-std=c++11'

	def build_extension(self, ext):
		orig_compile = self.compiler._compile

		def _compile(obj, src, ext_, cc_args, extra_postargs, pp_opts):
			postargs = list(extra_postargs)
			if src.endswith(('.cpp', '.cxx', '.cc', '.C')):
				postargs = [self.CXX_STD] + postargs
			return orig_compile(obj, src, ext_, cc_args, postargs, pp_opts)

		self.compiler._compile = _compile
		try:
			super().build_extension(ext)
		finally:
			self.compiler._compile = orig_compile


corearray_fnlst = [ os.path.join('src', 'CoreArray', fn) for fn in [
	'CoreArray.cpp',
	'dAllocator.cpp',
	'dAny.cpp',
	'dBase.cpp',
	'dBitGDS.cpp',
	'dEndian.cpp',
	'dFile.cpp',
	'dParallel.cpp',
	'dParallel_Ext.c',
	'dPlatform.cpp',
	'dRealGDS.cpp',
	'dSerial.cpp',
	'dSparse.cpp',
	'dStrGDS.cpp',
	'dStream.cpp',
	'dStruct.cpp',
	'dVLIntGDS.cpp' ] ]

zlib_fnlst = [ os.path.join('src', 'ZLIB', fn) for fn in [
	'adler32.c',
	'compress.c',
	'crc32.c',
	'deflate.c',
	'infback.c',
	'inffast.c',
	'inflate.c',
	'inftrees.c',
	'trees.c',
	'uncompr.c',
	'zutil.c' ] ]

lz4_fnlst = [ os.path.join('src', 'LZ4', fn) for fn in [
	'lz4.c',
	'lz4hc.c',
	'lz4frame.c',
	'xxhash.c' ] ]

pygds_fnlst = [ os.path.join('src', fn) for fn in [
	'PyCoreArray.cpp',
	'pygds.cpp' ] ]


# ----------------------------------------------------------------------------
# Locate liblzma (xz). Honour an explicit prefix, otherwise probe the usual
# Homebrew / system locations so the extension links against a real liblzma.
# ----------------------------------------------------------------------------
def _lzma_dirs():
	inc, lib = [], []
	prefix = os.environ.get('LZMA_PREFIX') or os.environ.get('XZ_PREFIX')
	candidates = []
	if prefix:
		candidates.append(prefix)
	candidates += [
		'/opt/homebrew/opt/xz',   # Apple Silicon Homebrew
		'/usr/local/opt/xz',      # Intel Homebrew
		'/opt/homebrew',
		'/usr/local',
		'/usr',
	]
	for c in candidates:
		i, l = os.path.join(c, 'include'), os.path.join(c, 'lib')
		if os.path.exists(os.path.join(i, 'lzma.h')):
			inc.append(i)
			lib.append(l)
			break
	return inc, lib


lzma_inc, lzma_lib = _lzma_dirs()

extra_compile_args = ['-O2']
extra_link_args = []
# Make sure the dynamic linker can find a Homebrew liblzma at runtime.
for l in lzma_lib:
	extra_link_args += ['-Wl,-rpath,' + l]

ext = Extension(
	'pygds.ccall',
	corearray_fnlst + zlib_fnlst + lz4_fnlst + pygds_fnlst,
	include_dirs = [ 'pygds/include', 'src/CoreArray', numpy.get_include() ] + lzma_inc,
	library_dirs = lzma_lib,
	define_macros = [
		('USING_PYTHON', None),
		('_FILE_OFFSET_BITS', 64),
		# XZ/LZMA via the system liblzma (external-lib mode); LZ4 bundled in src/LZ4
		('COREARRAY_USE_LZMA_EXT', None),
		('NPY_NO_DEPRECATED_API', 'NPY_1_7_API_VERSION'),
	],
	libraries = [ 'lzma' ],
	extra_compile_args = extra_compile_args,
	extra_link_args = extra_link_args,
)

setup(ext_modules=[ext], cmdclass={'build_ext': build_ext_cppstd})
