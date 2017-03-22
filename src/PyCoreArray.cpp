// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// PyCoreArray.cpp: Export the C routines of CoreArray library
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
#include <numpy/arrayobject.h>


namespace pygds
{
	using namespace std;
	using namespace CoreArray;


	/// a list of GDS files in the gdsfmt package
	COREARRAY_DLL_LOCAL PdGDSFile PKG_GDS_Files[PYGDS_MAX_NUM_GDS_FILES];

	/// get the index in 'PKG_GDS_Files' for NULL
	COREARRAY_DLL_LOCAL int GetEmptyFileIndex(bool throw_error=true)
	{
		for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
		{
			if (PKG_GDS_Files[i] == NULL)
				return i;
		}
		if (throw_error)
		{
			throw ErrGDSFmt(
				"You have opened %d GDS files, and no more is allowed!",
				PYGDS_MAX_NUM_GDS_FILES);
		}
		return -1;
	}

	/// get the index in 'PKG_GDS_Files' for file
	COREARRAY_DLL_LOCAL int GetFileIndex(PdGDSFile file, bool throw_error=true)
	{
		for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
		{
			if (PKG_GDS_Files[i] == file)
				return i;
		}
		if (throw_error)
		{
			throw ErrGDSFmt(
				"The GDS file has been closed, or invalid.");
		}
		return -1;
	}


	/// a list of GDS objects
	COREARRAY_DLL_LOCAL vector<PdGDSObj> PYGDS_GDSObj_List;

	/// mapping from GDS objects to indices
	COREARRAY_DLL_LOCAL map<PdGDSObj, int> PYGDS_GDSObj_Map;


	/// initialization and finalization
	class COREARRAY_DLL_LOCAL CInitObject
	{
	public:
		/// initialization
		CInitObject()
		{
			memset(PKG_GDS_Files, 0, sizeof(PKG_GDS_Files));
			PYGDS_GDSObj_List.reserve(1024);
		}

		/// finalization
		~CInitObject()
		{
			PYGDS_GDSObj_List.clear();
			PYGDS_GDSObj_Map.clear();

			for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
			{
				PdGDSFile file = PKG_GDS_Files[i];
				if (file != NULL)
				{
					try {
						PKG_GDS_Files[i] = NULL;
						delete file;
					}
					catch (...) { }
				}
			}
		}
	};

	static CInitObject InitObject;
}

using namespace std;
using namespace CoreArray;
using namespace CoreArray::Parallel;
using namespace pygds;


extern "C"
{

static const char *ERR_WRITE_ONLY =
	"Writable only and please call 'readmode()' before reading.";

// ===========================================================================
// Functions for file structure

COREARRAY_DLL_EXPORT PdGDSFile GDS_File_Create(const char *FileName)
{
	int gds_idx = GetEmptyFileIndex();
	PdGDSFile file = NULL;

	try {
		file = new CdGDSFile;
		file->SaveAsFile(FileName);
		PKG_GDS_Files[gds_idx] = file;
	}
	catch (std::exception &E) {
		if (file) delete file;
		throw;
	}
	catch (const char *E) {
		if (file) delete file;
		throw;
	}
	catch (...) {
		if (file) delete file;
		throw;
	}
	return file;
}


COREARRAY_DLL_EXPORT PdGDSFile GDS_File_Open(const char *FileName,
	C_BOOL ReadOnly, C_BOOL ForkSupport)
{
	int gds_idx = GetEmptyFileIndex();
	PdGDSFile file = NULL;

	try {
		file = new CdGDSFile;
		if (!ForkSupport)
			file->LoadFile(FileName, ReadOnly);
		else
			file->LoadFileFork(FileName, ReadOnly);

		PKG_GDS_Files[gds_idx] = file;
	}
	catch (std::exception &E) {
		string Msg = E.what();
		if ((file!=NULL) && !file->Log().List().empty())
		{
			Msg.append(sLineBreak);
			Msg.append("Log:");
			for (size_t i=0; i < file->Log().List().size(); i++)
			{
				Msg.append(sLineBreak);
				Msg.append(RawText(file->Log().List()[i].Msg));
			}
		}
		if (file) delete file;
		throw ErrGDSFmt(Msg);
	}
	catch (const char *E) {
		string Msg = E;
		if ((file!=NULL) && !file->Log().List().empty())
		{
			Msg.append(sLineBreak);
			Msg.append("Log:");
			for (size_t i=0; i < file->Log().List().size(); i++)
			{
				Msg.append(sLineBreak);
				Msg.append(RawText(file->Log().List()[i].Msg));
			}
		}
		if (file) delete file;
		throw ErrGDSFmt(Msg);
	}
	catch (...) {
		if (file) delete file;
		throw;
	}
	return file;
}


COREARRAY_DLL_EXPORT void GDS_File_Close(PdGDSFile File)
{
	int gds_idx = GetFileIndex(File, false);
	if (gds_idx >= 0)
	{
		PKG_GDS_Files[gds_idx] = NULL;

		// delete GDS objects in PYGDS_GDSObj_List and PYGDS_GDSObj_Map
		vector<PdGDSObj>::iterator p = PYGDS_GDSObj_List.begin();
		for (; p != PYGDS_GDSObj_List.end(); p++)
		{
			if (*p != NULL)
			{
				// for a virtual folder
				PdGDSObj Obj = *p;
				PdGDSFolder Folder = Obj->Folder();
				while (Folder != NULL)
				{
					Obj = Folder;
					Folder = Obj->Folder();
				}
				// Obj is the root, and then get the GDS file
				if (Obj->GDSFile() == File)
				{
					PYGDS_GDSObj_Map.erase(*p);
					*p = NULL;
				}
			}
		}
	}
	if (File) delete File;
}



// ===========================================================================
// R/Python objects

/// return true, if Obj is a logical object in R
COREARRAY_DLL_EXPORT C_BOOL GDS_R_Is_Logical(PdGDSObj Obj)
{
	return Obj->Attribute().HasName(ASC16("R.logical"));
}


/// return true, if Obj is a factor variable
COREARRAY_DLL_EXPORT C_BOOL GDS_R_Is_Factor(PdGDSObj Obj)
{
	if (Obj->Attribute().HasName(ASC16("R.class")) &&
		Obj->Attribute().HasName(ASC16("R.levels")))
	{
		return (Obj->Attribute()[ASC16("R.class")].GetStr8() == "factor");
	} else
		return false;
}


/// return a Python/NumPy object from a GDS object
COREARRAY_DLL_EXPORT PyObject* GDS_Py_Array_Read(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length,
	const C_BOOL *const Selection[], C_SVType SV)
{
	static NPY_TYPES sv2npy[] = {
		NPY_VOID,       // svCustom
		NPY_VOID,       // svCustomInt
		NPY_VOID,       // svCustomUInt
		NPY_VOID,       // svCustomFloat
		NPY_VOID,       // svCustomStr
		NPY_INT8,       // svInt8
		NPY_UINT8,      // svUInt8
		NPY_INT16,      // svInt16
		NPY_UINT16,     // svUInt16
		NPY_INT32,      // svInt32
		NPY_UINT32,     // svUInt32
		NPY_INT64,      // svInt64
		NPY_UINT64,     // svUInt64
		NPY_FLOAT,      // svFloat32
		NPY_DOUBLE,     // svFloat64
		NPY_VOID,       // svStrUTF8
		NPY_VOID        // svStrUTF16
	};

	try
	{
		if (SV == svCustom)
		{
			SV = Obj->SVType();
			if (SV == svCustomInt)
				SV = svInt64;
			else if (SV == svCustomUInt)
				SV = svUInt64;
			else if (SV == svCustomFloat)
				SV = svFloat64;
			else if (SV == svCustomStr)
				SV = svStrUTF8;
		}

		NPY_TYPES npy_type = NPY_VOID;
		if ((0 <= SV) && (SV < sizeof(sv2npy)/sizeof(NPY_TYPES)))
			npy_type = sv2npy[SV];
		if (npy_type == NPY_VOID)
			throw ErrGDSFmt("Data type is not supported.");

		CdAbstractArray::TArrayDim St, Cnt;
		if (Start == NULL)
		{
			memset(St, 0, sizeof(St));
			Start = St;
		}
		if (Length == NULL)
		{
			Obj->GetDim(Cnt);
			Length = Cnt;
		}

		CdAbstractArray::TArrayDim ValidCnt;
		Obj->GetInfoSelection(Start, Length, Selection, NULL, NULL, ValidCnt);

		int ndim = Obj->DimCnt();
		npy_intp dims[ndim];
		for (int i=0; i < ndim; i++) dims[i] = ValidCnt[i];

		// create a numpy array object
		PyObject *rv_ans = PyArray_SimpleNew(ndim, dims, npy_type);

		// read
		if (COREARRAY_SV_NUMERIC(SV))
		{
			void *datptr = PyArray_DATA(rv_ans);
			if (!Selection)
				Obj->ReadData(Start, Length, datptr, SV);
			else
				Obj->ReadDataEx(Start, Length, Selection, datptr, SV);
		}

		return rv_ans;
	}
	catch (ErrAllocRead &E)
	{
		throw ErrGDSFmt(ERR_WRITE_ONLY);
	}
	catch (EZLibError &E)
	{
		throw ErrGDSFmt(ERR_WRITE_ONLY);
	}

	return NULL;
}



// ===========================================================================
// Functions for error

/// the last error message
COREARRAY_DLL_LOCAL string Py_CoreArray_Error_Msg;

COREARRAY_DLL_EXPORT const char *GDS_GetError()
{
	return Py_CoreArray_Error_Msg.c_str();
}

COREARRAY_DLL_EXPORT void GDS_SetError(const char *Msg)
{
	if (Msg)
	{
		if (Msg != GDS_GetError())
			Py_CoreArray_Error_Msg = Msg;
	} else {
		Py_CoreArray_Error_Msg.clear();
	}
}



// ===========================================================================
// Initialization

/// the last error message
COREARRAY_DLL_LOCAL PyObject *Py_CoreArray_Init()
{
	// register CoreArray classes and objects
	RegisterClass();
	// import numpy functions
	import_array();
	// output
	return NULL;
}

}
