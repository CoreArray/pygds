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
// R objects

COREARRAY_DLL_EXPORT PdGDSFile GDS_ID2File(int file_id)
{
	if ((file_id < 0) || (file_id >= PYGDS_MAX_NUM_GDS_FILES))
		throw ErrGDSFmt("The GDS file ID (%d) is invalid.", file_id);

	PdGDSFile file = PKG_GDS_Files[file_id];
	if (file == NULL)
		throw ErrGDSFmt("The GDS file is closed or uninitialized.");

	return file;
}


COREARRAY_DLL_EXPORT PdGDSFolder GDS_ID2FileRoot(int file_id)
{
	PdGDSFile file = GDS_ID2File(file_id);
	return &(file->Root());
}






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


COREARRAY_DLL_EXPORT void GDS_File_Sync(PdGDSFile File)
{
	File->SyncFile();
}


COREARRAY_DLL_EXPORT PdGDSFolder GDS_File_Root(PdGDSFile File)
{
	return &File->Root();
}


COREARRAY_DLL_EXPORT PdGDSFile GDS_Node_File(PdGDSObj Node)
{
	return Node->GDSFile();
}


COREARRAY_DLL_EXPORT void GDS_Node_Delete(PdGDSObj Node, C_BOOL Force)
{
	if (Node != NULL)
	{
		vector<C_BOOL> DeleteArray;
		if (dynamic_cast<CdGDSAbsFolder*>(Node))
		{
			DeleteArray.resize(PYGDS_GDSObj_List.size(), false);
			size_t idx = 0;
			vector<PdGDSObj>::iterator p = PYGDS_GDSObj_List.begin();
			for (; p != PYGDS_GDSObj_List.end(); p++)
			{
				if (*p != NULL)
				{
					if (static_cast<CdGDSAbsFolder*>(Node)->HasChild(*p, true))
						DeleteArray[idx] = true;
				}
				idx ++;
			}
		}

		if (Node->Folder())
			Node->Folder()->DeleteObj(Node, Force != 0);
		else
			throw ErrGDSFmt("Can not delete the root.");

		// delete GDS objects in PYGDS_GDSObj_List and PYGDS_GDSObj_Map
		vector<PdGDSObj>::iterator p = PYGDS_GDSObj_List.begin();
		for (; p != PYGDS_GDSObj_List.end(); p++)
			if (*p == Node) *p = NULL;
		PYGDS_GDSObj_Map.erase(Node);

		if (!DeleteArray.empty())
		{
			size_t idx = 0;
			vector<C_BOOL>::iterator p = DeleteArray.begin();
			for (; p != DeleteArray.end(); p++)
			{
				if (*p)
				{
					PdGDSObj &Obj = PYGDS_GDSObj_List[idx];
					PYGDS_GDSObj_Map.erase(Obj);
					Obj = NULL;
				}
				idx ++;
			}
		}
	}
}


COREARRAY_DLL_EXPORT void GDS_Node_GetClassName(PdGDSObj Node, char *Out,
	size_t OutSize)
{
	string nm = Node->dName();
	if (Out)
		strncpy(Out, nm.c_str(), OutSize);
}


COREARRAY_DLL_EXPORT int GDS_Node_ChildCount(PdGDSFolder Node)
{
	return Node->NodeCount();
}


COREARRAY_DLL_EXPORT PdGDSObj GDS_Node_Path(PdGDSFolder Node,
	const char *Path, C_BOOL MustExist)
{
	if (MustExist)
		return Node->Path(UTF16Text(Path));
	else
		return Node->PathEx(UTF16Text(Path));
}



// ===========================================================================
// functions for attributes

COREARRAY_DLL_EXPORT int GDS_Attr_Count(PdGDSObj Node)
{
	return Node->Attribute().Count();
}

COREARRAY_DLL_EXPORT int GDS_Attr_Name2Index(PdGDSObj Node, const char *Name)
{
	return Node->Attribute().IndexName(ASC16(Name));
}



// ===========================================================================
// functions for CdAbstractArray

COREARRAY_DLL_EXPORT int GDS_Array_DimCnt(PdAbstractArray Obj)
{
	return Obj->DimCnt();
}

COREARRAY_DLL_EXPORT void GDS_Array_GetDim(PdAbstractArray Obj,
	C_Int32 OutBuffer[], size_t N_Buf)
{
	if (Obj->DimCnt() > (int)N_Buf)
		throw ErrCoreArray("Insufficient buffer in 'GDS_Array_GetDim'.");
	Obj->GetDim(OutBuffer);
}

COREARRAY_DLL_EXPORT C_Int64 GDS_Array_GetTotalCount(PdAbstractArray Obj)
{
	return Obj->TotalCount();
}

COREARRAY_DLL_EXPORT enum C_SVType GDS_Array_GetSVType(PdAbstractArray Obj)
{
	return Obj->SVType();
}

COREARRAY_DLL_EXPORT unsigned GDS_Array_GetBitOf(PdAbstractArray Obj)
{
	return Obj->BitOf();
}

COREARRAY_DLL_EXPORT void *GDS_Array_ReadData(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length, void *OutBuf,
	enum C_SVType OutSV)
{
	return Obj->ReadData(Start, Length, OutBuf, OutSV);
}

COREARRAY_DLL_EXPORT void *GDS_Array_ReadDataEx(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length,
	const C_BOOL *const Selection[], void *OutBuf, enum C_SVType OutSV)
{
	return Obj->ReadDataEx(Start, Length, Selection, OutBuf, OutSV);
}

COREARRAY_DLL_EXPORT const void *GDS_Array_WriteData(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length, const void *InBuf,
	enum C_SVType InSV)
{
	return Obj->WriteData(Start, Length, InBuf, InSV);
}

COREARRAY_DLL_EXPORT const void *GDS_Array_AppendData(PdAbstractArray Obj,
	ssize_t Cnt, const void *InBuf, enum C_SVType InSV)
{
	return Obj->Append(InBuf, Cnt, InSV);
}

COREARRAY_DLL_EXPORT void GDS_Array_AppendString(PdAbstractArray Obj,
	const char *Text)
{
	UTF8String Val = Text;
	Obj->Append(&Val, 1, svStrUTF8);
}

COREARRAY_DLL_EXPORT void GDS_Array_AppendStrLen(PdAbstractArray Obj,
	const char *Text, size_t Len)
{
	UTF8String Val = UTF8String(Text, Len);
	Obj->Append(&Val, 1, svStrUTF8);
}



// ===========================================================================
// Functions for CdContainer - CdIterator

COREARRAY_DLL_EXPORT void GDS_Iter_GetStart(PdContainer Node, PdIterator Out)
{
	*Out = Node->IterBegin();
}

COREARRAY_DLL_EXPORT void GDS_Iter_GetEnd(PdContainer Node, PdIterator Out)
{
	*Out = Node->IterEnd();
}

COREARRAY_DLL_EXPORT PdContainer GDS_Iter_GetHandle(PdIterator I)
{
	return I->Handler;
}

COREARRAY_DLL_EXPORT void GDS_Iter_Offset(PdIterator I, C_Int64 Offset)
{
	*I += Offset;
}

COREARRAY_DLL_EXPORT void GDS_Iter_Position(PdContainer Node, PdIterator Out, C_Int64 Offset)
{
	*Out = Node->IterBegin();
	*Out += Offset;
}

COREARRAY_DLL_EXPORT C_Int64 GDS_Iter_GetInt(PdIterator I)
{
	return I->GetInteger();
}

COREARRAY_DLL_EXPORT C_Float64 GDS_Iter_GetFloat(PdIterator I)
{
	return I->GetFloat();
}

COREARRAY_DLL_EXPORT void GDS_Iter_GetStr(PdIterator I, char *Out, size_t Size)
{
	string s = RawText(I->GetString());
	if (Out)
		strncpy(Out, s.c_str(), Size);
}

COREARRAY_DLL_EXPORT void GDS_Iter_SetInt(PdIterator I, C_Int64 Val)
{
	I->SetInteger(Val);
}

COREARRAY_DLL_EXPORT void GDS_Iter_SetFloat(PdIterator I, C_Float64 Val)
{
	I->SetFloat(Val);
}

COREARRAY_DLL_EXPORT void GDS_Iter_SetStr(PdIterator I, const char *Str)
{
	I->SetString(UTF16Text(Str));
}

COREARRAY_DLL_EXPORT void *GDS_Iter_RData(PdIterator I, void *OutBuf,
	size_t Cnt, enum C_SVType OutSV)
{
	return I->ReadData(OutBuf, Cnt, OutSV);
}

COREARRAY_DLL_EXPORT void *GDS_Iter_RDataEx(PdIterator I, void *OutBuf,
	size_t Cnt, enum C_SVType OutSV, const C_BOOL Selection[])
{
	return I->ReadDataEx(OutBuf, Cnt, OutSV, Selection);
}

COREARRAY_DLL_EXPORT const void *GDS_Iter_WData(PdIterator I, const void *InBuf,
	size_t Cnt, enum C_SVType InSV)
{
	return I->WriteData(InBuf, Cnt, InSV);
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
/// functions for machine

// Return the number of available CPU cores in the system
COREARRAY_DLL_EXPORT int GDS_Mach_GetNumOfCores()
{
	return Mach::GetCPU_NumOfCores();
}

/// Return the size in byte of level-1 cache memory
COREARRAY_DLL_EXPORT C_UInt64 GDS_Mach_GetCPULevelCache(int level)
{
	return Mach::GetCPU_LevelCache(level);
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
		NPY_OBJECT,     // svStrUTF8
		NPY_VOID        // svStrUTF16
	};

	try
	{
		bool bool_flag = GDS_R_Is_Logical(Obj);
		NPY_TYPES npy_type = NPY_VOID;

		if (SV==svCustom && bool_flag)
		{
			SV = svInt8;
			npy_type = NPY_BOOL;
		} else {
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
			if ((0 <= SV) && (SV < sizeof(sv2npy)/sizeof(NPY_TYPES)))
				npy_type = sv2npy[SV];
		}

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
		} else if (SV == svStrUTF8)
		{
			const size_t n = PyArray_SIZE(rv_ans);
			vector<UTF8String> strbuf(n);
			if (!Selection)
				Obj->ReadData(Start, Length, &strbuf[0], SV);
			else
				Obj->ReadDataEx(Start, Length, Selection, &strbuf[0], SV);
			PyObject** p = (PyObject**)PyArray_DATA(rv_ans);
			for (size_t i=0; i < strbuf.size(); i++)
			{
				UTF8String &s = strbuf[i];
				PyArray_SETITEM(rv_ans, p++, PYSTR_SET2(&s[0], s.size()));
			}
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
// Initialization

typedef void (*TFUNC)();

void *PyGDS_API = NULL;

static TFUNC c_api[] = {
	// Python objects
	(TFUNC)GDS_ID2File,
	(TFUNC)GDS_ID2FileRoot,
	// functions for file structure
	(TFUNC)GDS_File_Create,
	(TFUNC)GDS_File_Open,
	(TFUNC)GDS_File_Close,
	(TFUNC)GDS_File_Sync,
	(TFUNC)GDS_File_Root,
	(TFUNC)GDS_Node_File,
	(TFUNC)GDS_Node_Delete,
	(TFUNC)GDS_Node_GetClassName,
	(TFUNC)GDS_Node_ChildCount,
	(TFUNC)GDS_Node_Path,
	// functions for attributes
	(TFUNC)GDS_Attr_Count,
	(TFUNC)GDS_Attr_Name2Index,
	// functions for CdAbstractArray
	(TFUNC)GDS_Array_DimCnt,
	(TFUNC)GDS_Array_GetDim,
	(TFUNC)GDS_Array_GetTotalCount,
	(TFUNC)GDS_Array_GetSVType,
	(TFUNC)GDS_Array_GetBitOf,
	(TFUNC)GDS_Array_ReadData,
	(TFUNC)GDS_Array_ReadDataEx,
	(TFUNC)GDS_Array_WriteData,
	(TFUNC)GDS_Array_AppendData,
	(TFUNC)GDS_Array_AppendString,
	(TFUNC)GDS_Array_AppendStrLen,
	// functions for CdIterator
	(TFUNC)GDS_Iter_GetStart,
	(TFUNC)GDS_Iter_GetEnd,
	(TFUNC)GDS_Iter_GetHandle,
	(TFUNC)GDS_Iter_Offset,
	(TFUNC)GDS_Iter_Position,
	(TFUNC)GDS_Iter_GetInt,
	(TFUNC)GDS_Iter_GetFloat,
	(TFUNC)GDS_Iter_GetStr,
	(TFUNC)GDS_Iter_SetInt,
	(TFUNC)GDS_Iter_SetFloat,
	(TFUNC)GDS_Iter_SetStr,
	(TFUNC)GDS_Iter_RData,
	(TFUNC)GDS_Iter_RDataEx,
	(TFUNC)GDS_Iter_WData,
	// functions for error
	(TFUNC)GDS_GetError,
	(TFUNC)GDS_SetError,
	// functions for machine
	(TFUNC)GDS_Mach_GetNumOfCores,
	(TFUNC)GDS_Mach_GetCPULevelCache
};


/*
	// R objects
	(TFUNC)GDS_R_SEXP2File);
	(TFUNC)GDS_R_SEXP2FileRoot);
	(TFUNC)GDS_R_SEXP2Obj);
	(TFUNC)GDS_R_Obj2SEXP);
	(TFUNC)GDS_R_Obj_SEXP2SEXP);
	(TFUNC)GDS_R_Is_Logical);
	(TFUNC)GDS_R_Is_Factor);
	(TFUNC)GDS_R_Set_IfFactor);
	(TFUNC)GDS_R_Array_Read);
	(TFUNC)GDS_R_Apply);
	(TFUNC)GDS_R_Append);
	(TFUNC)GDS_R_AppendEx);
	(TFUNC)GDS_R_Is_Element);



	// functions for parallel computing
	(TFUNC)GDS_Parallel_InitMutex);
	(TFUNC)GDS_Parallel_DoneMutex);
	(TFUNC)GDS_Parallel_LockMutex);
	(TFUNC)GDS_Parallel_TryLockMutex);
	(TFUNC)GDS_Parallel_UnlockMutex);
	(TFUNC)GDS_Parallel_InitCondition);
	(TFUNC)GDS_Parallel_DoneCondition);
	(TFUNC)GDS_Parallel_SignalCondition);
	(TFUNC)GDS_Parallel_BroadcastCondition);
	(TFUNC)GDS_Parallel_WaitCondition);
	(TFUNC)GDS_Parallel_InitSuspend);
	(TFUNC)GDS_Parallel_DoneSuspend);
	(TFUNC)GDS_Parallel_Suspend);
	(TFUNC)GDS_Parallel_WakeUp);
	(TFUNC)GDS_Parallel_RunThreads);

	// functions for reading block by block
	(TFUNC)GDS_ArrayRead_Init);
	(TFUNC)GDS_ArrayRead_Free);
	(TFUNC)GDS_ArrayRead_Read);
	(TFUNC)GDS_ArrayRead_Eof);
	(TFUNC)GDS_ArrayRead_BalanceBuffer);
*/


/// the last error message
COREARRAY_DLL_LOCAL PyObject *pygds_init()
{
	PyGDS_API = (void*)c_api;
	// register CoreArray classes and objects
	RegisterClass();
	// import numpy functions
	import_array();
	// output
	return NULL;
}

}
