from distutils.core import setup, Extension
import os


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

pygds_fnlst = [ os.path.join('src', fn) for fn in [
	'PyCoreArray.cpp', 'pygds.cpp' ] ]


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



setup(name='pygds',
	version='0.1',
	description='Python Interface to CoreArray Genomic Data Structure (GDS) Files',
	url='http://github.com/zhengxwen/pygds',
	author='Xiuwen Zheng',
	author_email='zhengxwen@gmail.com',
	license='GPLv3',
	packages=['pygds'],
	ext_modules=[ Extension('pygds.ccall',
		corearray_fnlst + pygds_fnlst,
		include_dirs=[ 'include', 'src/CoreArray' ],
		define_macros=[
			('USING_PYTHON', None),
			('COREARRAY_USE_ZLIB_EXT', None),
			('COREARRAY_NO_LZ4', None),
			('COREARRAY_NO_LZMA', None)
		],
		libraries = [ 'z' ]
	) ]
)
