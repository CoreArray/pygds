from distutils.core import setup, Extension
import os
import numpy


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
	'dSerial.cpp',
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

pygds_fnlst = [ os.path.join('src', fn) for fn in [
	'PyCoreArray.cpp',
	'pygds.cpp' ] ]



setup(name='pygds',
	version = '0.1.0',
	description = 'Python Interface to CoreArray Genomic Data Structure (GDS) Files',
	url = 'http://github.com/CoreArray/pygds',
	author = 'Xiuwen Zheng',
	author_email = 'zhengxwen@gmail.com',
	license = 'GPLv3',
	packages = [ 'pygds' ],
	install_requires = [ 'numpy' ],
	ext_modules = [ Extension('pygds.ccall',
		corearray_fnlst + zlib_fnlst + pygds_fnlst,
		include_dirs = [ 'pygds/include', 'src/CoreArray', numpy.get_include() ],
		define_macros = [
			('USING_PYTHON', None),
			('_FILE_OFFSET_BITS', 64),
			('COREARRAY_USE_LZMA_EXT', None),
			('COREARRAY_NO_LZ4', None),
		],
		libraries = [ 'lzma' ]
	) ],
	package_data = {
		'pygds': [ 'include/*.h', 'data/*.gds' ]
	}
)
