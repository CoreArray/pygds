// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// pygds.cpp: Python Interface to CoreArray Genomic Data Structure (GDS) Files
//
// Copyright (C) 2017    Xiuwen Zheng
//
// pygds is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 3 as
// published by the Free Software Foundation.
//
// pygds is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with pygds.
// If not, see <http://www.gnu.org/licenses/>.


#define COREARRAY_PYGDS_PACKAGE

#include <PyGDS_CPP.h>
#include <string>
#include <set>
#include <map>


namespace pygds
{
	extern PdGDSFile PYGDS_GDS_Files[];
	extern int GetFileIndex(PdGDSFile file, bool throw_error=true);


	/// initialization and finalization
	class COREARRAY_DLL_LOCAL CInitNameObject
	{
	public:

		/// comparison of "const char *"
		struct strCmp {
			bool operator()(const char* s1, const char* s2) const
				{ return strcmp(s1, s2) < 0; }
		};

		/// mapping object
		map<const char*, const char*, strCmp> ClassMap;

		/// initialization
		CInitNameObject()
		{
			// ==============================================================
			// integer

			ClassMap["int8"  ] = TdTraits< C_Int8   >::StreamName();
			ClassMap["uint8" ] = TdTraits< C_UInt8  >::StreamName();
			ClassMap["int16" ] = TdTraits< C_Int16  >::StreamName();
			ClassMap["uint16"] = TdTraits< C_UInt16 >::StreamName();
			ClassMap["int32" ] = TdTraits< C_Int32  >::StreamName();
			ClassMap["uint32"] = TdTraits< C_UInt32 >::StreamName();
			ClassMap["int64" ] = TdTraits< C_Int64  >::StreamName();
			ClassMap["uint64"] = TdTraits< C_UInt64 >::StreamName();
			ClassMap["int24" ] = TdTraits< CoreArray::Int24  >::StreamName();
			ClassMap["uint24"] = TdTraits< CoreArray::UInt24 >::StreamName();

			ClassMap["bit1"] = TdTraits< CoreArray::BIT1 >::StreamName();
			ClassMap["bit2"] = TdTraits< CoreArray::BIT2 >::StreamName();
			ClassMap["bit3"] = TdTraits< CoreArray::BIT3 >::StreamName();
			ClassMap["bit4"] = TdTraits< CoreArray::BIT4 >::StreamName();
			ClassMap["bit5"] = TdTraits< CoreArray::BIT5 >::StreamName();
			ClassMap["bit6"] = TdTraits< CoreArray::BIT6 >::StreamName();
			ClassMap["bit7"] = TdTraits< CoreArray::BIT7 >::StreamName();
			ClassMap["bit8"] = TdTraits< C_UInt8         >::StreamName();

			ClassMap["bit9" ] = TdTraits< CoreArray::BIT9  >::StreamName();
			ClassMap["bit10"] = TdTraits< CoreArray::BIT10 >::StreamName();
			ClassMap["bit11"] = TdTraits< CoreArray::BIT11 >::StreamName();
			ClassMap["bit12"] = TdTraits< CoreArray::BIT12 >::StreamName();
			ClassMap["bit13"] = TdTraits< CoreArray::BIT13 >::StreamName();
			ClassMap["bit14"] = TdTraits< CoreArray::BIT14 >::StreamName();
			ClassMap["bit15"] = TdTraits< CoreArray::BIT15 >::StreamName();
			ClassMap["bit16"] = TdTraits< C_UInt16         >::StreamName();

			ClassMap["bit24"] = TdTraits< CoreArray::UInt24 >::StreamName();
			ClassMap["bit32"] = TdTraits< C_UInt32          >::StreamName();
			ClassMap["bit64"] = TdTraits< C_UInt64          >::StreamName();

			ClassMap["sbit2"] = TdTraits< CoreArray::SBIT2 >::StreamName();
			ClassMap["sbit3"] = TdTraits< CoreArray::SBIT3 >::StreamName();
			ClassMap["sbit4"] = TdTraits< CoreArray::SBIT4 >::StreamName();
			ClassMap["sbit5"] = TdTraits< CoreArray::SBIT5 >::StreamName();
			ClassMap["sbit6"] = TdTraits< CoreArray::SBIT6 >::StreamName();
			ClassMap["sbit7"] = TdTraits< CoreArray::SBIT7 >::StreamName();
			ClassMap["sbit8"] = TdTraits< C_Int8           >::StreamName();

			ClassMap["sbit9" ] = TdTraits< CoreArray::SBIT9  >::StreamName();
			ClassMap["sbit10"] = TdTraits< CoreArray::SBIT10 >::StreamName();
			ClassMap["sbit11"] = TdTraits< CoreArray::SBIT11 >::StreamName();
			ClassMap["sbit12"] = TdTraits< CoreArray::SBIT12 >::StreamName();
			ClassMap["sbit13"] = TdTraits< CoreArray::SBIT13 >::StreamName();
			ClassMap["sbit14"] = TdTraits< CoreArray::SBIT14 >::StreamName();
			ClassMap["sbit15"] = TdTraits< CoreArray::SBIT15 >::StreamName();
			ClassMap["sbit16"] = TdTraits< C_Int16           >::StreamName();

			ClassMap["sbit24"] = TdTraits< CoreArray::Int24 >::StreamName();
			ClassMap["sbit32"] = TdTraits< C_Int32          >::StreamName();
			ClassMap["sbit64"] = TdTraits< C_Int64          >::StreamName();


			// ==============================================================
			// Real number

			ClassMap["float32"] = TdTraits< C_Float32 >::StreamName();
			ClassMap["float64"] = TdTraits< C_Float64 >::StreamName();
			ClassMap["packedreal8"]  = TdTraits< TREAL8  >::StreamName();
			ClassMap["packedreal16"] = TdTraits< TREAL16 >::StreamName();
			ClassMap["packedreal24"] = TdTraits< TREAL24 >::StreamName();
			ClassMap["packedreal32"] = TdTraits< TREAL32 >::StreamName();


			// ==============================================================
			// String

			ClassMap["string"   ] = TdTraits< VARIABLE_LEN<C_UTF8>  >::StreamName();
			ClassMap["string16" ] = TdTraits< VARIABLE_LEN<C_UTF16> >::StreamName();
			ClassMap["string32" ] = TdTraits< VARIABLE_LEN<C_UTF32> >::StreamName();
			ClassMap["cstring"  ] = TdTraits< C_STRING<C_UTF8>  >::StreamName();
			ClassMap["cstring16"] = TdTraits< C_STRING<C_UTF16> >::StreamName();
			ClassMap["cstring32"] = TdTraits< C_STRING<C_UTF32> >::StreamName();
			ClassMap["fstring"  ] = TdTraits< FIXED_LEN<C_UTF8>  >::StreamName();
			ClassMap["fstring16"] = TdTraits< FIXED_LEN<C_UTF16> >::StreamName();
			ClassMap["fstring32"] = TdTraits< FIXED_LEN<C_UTF32> >::StreamName();


			// ==============================================================
			// R storage mode

			ClassMap["char"     ] = TdTraits< C_Int8  >::StreamName();
			ClassMap["raw"      ] = TdTraits< C_Int8  >::StreamName();
			ClassMap["int"      ] = TdTraits< C_Int32 >::StreamName();
			ClassMap["integer"  ] = TdTraits< C_Int32 >::StreamName();
			ClassMap["vl_int"   ] = TdTraits< TVL_Int >::StreamName();
			ClassMap["vl_uint"  ] = TdTraits< TVL_UInt >::StreamName();
			ClassMap["float"    ] = TdTraits< C_Float32 >::StreamName();
			ClassMap["numeric"  ] = TdTraits< C_Float64 >::StreamName();
			ClassMap["double"   ] = TdTraits< C_Float64 >::StreamName();
			ClassMap["character"] = TdTraits< VARIABLE_LEN<C_UTF8> >::StreamName();
			ClassMap["logical"  ] = TdTraits< C_Int32 >::StreamName();
			ClassMap["factor"   ] = TdTraits< C_Int32 >::StreamName();

			ClassMap["list"] = "$FOLDER$";
			ClassMap["folder"] = "$FOLDER$";
		}
	};
	
	static CInitNameObject Init;
}


using namespace std;
using namespace CoreArray;
using namespace pygds;


extern "C"
{

// error messages
static const char *ERR_NOT_FOLDER =
	"The GDS node is not a folder!";
static const char *ERR_READ_ONLY =
	"Read-only and please call 'compression.gdsn(node, \"\")' before writing.";
static const char *ERR_NO_DATA =
	"There is no data field.";


// ----------------------------------------------------------------------------
// Internal functions
// ----------------------------------------------------------------------------

/// convert file_id to (CdGDSFile*)
static PdGDSFile ID2File(int file_id)
{
	if ((file_id < 0) || (file_id >= PYGDS_MAX_NUM_GDS_FILES))
		throw ErrGDSFmt("The GDS ID (%d) is invalid.", file_id);

	PdGDSFile file = PYGDS_GDS_Files[file_id];
	if (file == NULL)
		throw ErrGDSFmt("The GDS file is closed or uninitialized.");

	return file;
}


// ----------------------------------------------------------------------------
// File Operations
// ----------------------------------------------------------------------------

/// Create a GDS file
static PyObject* gdsCreateGDS(PyObject *self, PyObject *args)
{
	const char *fn;
	int allow_dup;
	if (!PyArg_ParseTuple(args, "s" BSTR, &fn, &allow_dup))
		return NULL;

	int file_id = -1;
	COREARRAY_TRY
		if (!allow_dup)
		{
			UTF8String FName = UTF8Text(fn);
			for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
			{
				if (PYGDS_GDS_Files[i])
				{
					if (PYGDS_GDS_Files[i]->FileName() == FName)
					{
						throw ErrGDSFmt(
							"The file '%s' has been created or opened.", fn);
					}
				}
			}
		}

		CdGDSFile *file = GDS_File_Create(fn);
		file_id = GetFileIndex(file);
	COREARRAY_CATCH

	return Py_BuildValue("i", file_id);
}


/// Open an existing GDS file
static PyObject* gdsOpenGDS(PyObject *self, PyObject *args)
{
	const char *fn;
	int readonly, allow_dup;
	if (!PyArg_ParseTuple(args, "s" BSTR BSTR, &fn, &readonly, &allow_dup))
		return NULL;

	int file_id = -1;
	COREARRAY_TRY
		if (!allow_dup)
		{
			UTF8String FName = UTF8Text(fn);
			for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
			{
				if (PYGDS_GDS_Files[i])
				{
					if (PYGDS_GDS_Files[i]->FileName() == FName)
					{
						throw ErrGDSFmt(
							"The file '%s' has been created or opened.", fn);
					}
				}
			}
		}

		CdGDSFile *file = GDS_File_Open(fn, readonly, true);
		file_id = GetFileIndex(file);
	COREARRAY_CATCH

	return Py_BuildValue("i", file_id);
}


/// Close the GDS file
static PyObject* gdsCloseGDS(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;

	COREARRAY_TRY
		if (file_id >= 0)
			GDS_File_Close(ID2File(file_id));
	COREARRAY_CATCH
	Py_RETURN_NONE;
}


} // extern "C"






extern "C"
{

static PyObject* helloworld(PyObject* self)
{
    return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
    "helloworld(): Any message you want to put here!!\n";

static PyMethodDef module_methods[] = {
    { "hello", (PyCFunction)helloworld, METH_NOARGS, helloworld_docs },
    { "create_gds", (PyCFunction)gdsCreateGDS, METH_VARARGS, NULL },
    { "open_gds", (PyCFunction)gdsOpenGDS, METH_VARARGS, NULL },
    { "close_gds", (PyCFunction)gdsCloseGDS, METH_VARARGS, NULL },
    { NULL }
};


#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef ModStruct =
{
	PyModuleDef_HEAD_INIT,
	"pygds.ccall", /* name of module */
	"",          /* module documentation, may be NULL */
	-1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
	module_methods
};

// Module entry point Python3
PyMODINIT_FUNC PyInit_ccall()
{
    return PyModule_Create(&ModStruct);
}

#endif

}
