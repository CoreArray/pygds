// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// PyGDS2.h: C interface to pygds dynamic library
//
// Copyright (C) 2017    Xiuwen Zheng
//
// This file is part of pygds.
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


#ifndef _PYGDS_C_FILE_
#define _PYGDS_C_FILE_

#include <PyGDS.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef void (*TFUNC)();

static TFUNC *c_api = NULL;



// ===========================================================================
// Python function

#define IDX_PY    0
// (TFUNC)GDS_ID2File
// (TFUNC)GDS_ID2FileRoot

typedef PdGDSFile (*Type_ID2File)(int);
COREARRAY_DLL_LOCAL PdGDSFile GDS_ID2File(int file_id)
{
	return (*(Type_ID2File)c_api[IDX_PY+0])(file_id);
}

typedef PdGDSFolder (*Type_ID2FileRoot)(int);
COREARRAY_DLL_LOCAL PdGDSFolder GDS_ID2FileRoot(int file_id)
{
	return (*(Type_ID2FileRoot)c_api[IDX_PY+1])(file_id);
}

typedef C_BOOL (*Type_Is_RLogical)(PdGDSObj);
COREARRAY_DLL_LOCAL C_BOOL GDS_Is_RLogical(PdGDSObj Obj)
{
	return (*(Type_Is_RLogical)c_api[IDX_PY+2])(Obj);
}

typedef C_BOOL (*Type_Is_RFactor)(PdGDSObj);
COREARRAY_DLL_LOCAL C_BOOL GDS_Is_RFactor(PdGDSObj Obj)
{
	return (*(Type_Is_RFactor)c_api[IDX_PY+3])(Obj);
}

typedef PyObject* (*Type_PyArrayRead)(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length,
	const C_BOOL *const Selection[], enum C_SVType SV);
COREARRAY_DLL_LOCAL PyObject* GDS_Py_Array_Read(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length,
	const C_BOOL *const Selection[], enum C_SVType SV)
{
	return (*(Type_PyArrayRead)c_api[IDX_PY+4])(Obj, Start, Length,
		Selection, SV);
}



// ===========================================================================
// File structure

#define IDX_FILE    (IDX_PY + 5)
// (TFUNC)GDS_File_Create
// (TFUNC)GDS_File_Open
// (TFUNC)GDS_File_Close
// (TFUNC)GDS_File_Sync
// (TFUNC)GDS_File_Root
// (TFUNC)GDS_Node_File
// (TFUNC)GDS_Node_Delete
// (TFUNC)GDS_Node_GetClassName
// (TFUNC)GDS_Node_ChildCount
// (TFUNC)GDS_Node_Path

typedef PdGDSFile (*Type_File_Create)(const char *);
COREARRAY_DLL_LOCAL PdGDSFile GDS_File_Create(const char *FileName)
{
	return (*(Type_File_Create)c_api[IDX_FILE])(FileName);
}

typedef PdGDSFile (*Type_File_Open)(const char *, C_BOOL, C_BOOL);
COREARRAY_DLL_LOCAL PdGDSFile GDS_File_Open(const char *FileName,
	C_BOOL ReadOnly, C_BOOL ForkSupport)
{
	return (*(Type_File_Open)c_api[IDX_FILE+1])(FileName, ReadOnly, ForkSupport);
}

typedef void (*Type_File_Close)(PdGDSFile);
COREARRAY_DLL_LOCAL void GDS_File_Close(PdGDSFile File)
{
	(*(Type_File_Close)c_api[IDX_FILE+2])(File);
}

typedef void (*Type_File_Sync)(PdGDSFile);
COREARRAY_DLL_LOCAL void GDS_File_Sync(PdGDSFile File)
{
	(*(Type_File_Sync)c_api[IDX_FILE+3])(File);
}

typedef PdGDSFolder (*Type_File_Root)(PdGDSFile);
COREARRAY_DLL_LOCAL PdGDSFolder GDS_File_Root(PdGDSFile File)
{
	return (*(Type_File_Root)c_api[IDX_FILE+4])(File);
}

typedef PdGDSFile (*Type_Node_File)(PdGDSObj);
COREARRAY_DLL_LOCAL PdGDSFile GDS_Node_File(PdGDSObj Node)
{
	return (*(Type_Node_File)c_api[IDX_FILE+5])(Node);
}

typedef void (*Type_Node_Delete)(PdGDSObj, C_BOOL);
COREARRAY_DLL_LOCAL void GDS_Node_Delete(PdGDSObj Node, C_BOOL Force)
{
	(*(Type_Node_Delete)c_api[IDX_FILE+6])(Node, Force);
}

typedef void (*Type_Node_GetClassName)(PdGDSObj, char *, size_t);
COREARRAY_DLL_LOCAL void GDS_Node_GetClassName(PdGDSObj Node, char *Out,
	size_t OutSize)
{
	(*(Type_Node_GetClassName)c_api[IDX_FILE+7])(Node, Out, OutSize);
}

typedef int (*Type_Node_ChildCount)(PdGDSFolder);
COREARRAY_DLL_LOCAL int GDS_Node_ChildCount(PdGDSFolder Node)
{
	return (*(Type_Node_ChildCount)c_api[IDX_FILE+8])(Node);
}

typedef PdGDSObj (*Type_Node_Path)(PdGDSFolder, const char *, C_BOOL);
COREARRAY_DLL_LOCAL PdGDSObj GDS_Node_Path(PdGDSFolder Node,
	const char *Path, C_BOOL MustExist)
{
	return (*(Type_Node_Path)c_api[IDX_FILE+9])(Node, Path, MustExist);
}



// ===========================================================================
// functions for attributes

#define IDX_ATTR    (IDX_FILE + 10)
// (TFUNC)GDS_Attr_Count
// (TFUNC)GDS_Attr_Name2Index

typedef int (*Type_Attr_Count)(PdGDSObj);
COREARRAY_DLL_EXPORT int GDS_Attr_Count(PdGDSObj Node)
{
	return (*(Type_Attr_Count)c_api[IDX_ATTR+0])(Node);
}

typedef int (*Type_Attr_Name2Index)(PdGDSObj, const char *);
COREARRAY_DLL_EXPORT int GDS_Attr_Name2Index(PdGDSObj Node, const char *Name)
{
	return (*(Type_Attr_Name2Index)c_api[IDX_ATTR+1])(Node, Name);
}



// ===========================================================================
// functions for CdAbstractArray

#define IDX_ARRAY    (IDX_ATTR + 2)
// (TFUNC)GDS_Array_DimCnt
// (TFUNC)GDS_Array_GetDim
// (TFUNC)GDS_Array_GetTotalCount
// (TFUNC)GDS_Array_GetSVType
// (TFUNC)GDS_Array_GetBitOf
// (TFUNC)GDS_Array_ReadData
// (TFUNC)GDS_Array_ReadDataEx
// (TFUNC)GDS_Array_WriteData
// (TFUNC)GDS_Array_AppendData
// (TFUNC)GDS_Array_AppendString
// (TFUNC)GDS_Array_AppendStrLen

typedef int (*Type_Array_DimCnt)(PdAbstractArray);
COREARRAY_DLL_LOCAL int GDS_Array_DimCnt(PdAbstractArray Obj)
{
	return (*(Type_Array_DimCnt)c_api[IDX_ARRAY+0])(Obj);
}

typedef void (*Type_Array_GetDim)(PdAbstractArray, C_Int32 [], size_t);
COREARRAY_DLL_LOCAL void GDS_Array_GetDim(PdAbstractArray Obj,
	C_Int32 OutBuffer[], size_t N_Buf)
{
	return (*(Type_Array_GetDim)c_api[IDX_ARRAY+1])(Obj, OutBuffer, N_Buf);
}

typedef C_Int64 (*Type_Array_GetTotalCount)(PdAbstractArray);
COREARRAY_DLL_LOCAL C_Int64 GDS_Array_GetTotalCount(PdAbstractArray Obj)
{
	return (*(Type_Array_GetTotalCount)c_api[IDX_ARRAY+2])(Obj);
}

typedef enum C_SVType (*Type_Array_GetSVType)(PdAbstractArray);
COREARRAY_DLL_LOCAL enum C_SVType GDS_Array_GetSVType(PdAbstractArray Obj)
{
	return (*(Type_Array_GetSVType)c_api[IDX_ARRAY+3])(Obj);
}

typedef unsigned (*Type_Array_GetBitOf)(PdAbstractArray);
COREARRAY_DLL_LOCAL unsigned GDS_Array_GetBitOf(PdAbstractArray Obj)
{
	return (*(Type_Array_GetBitOf)c_api[IDX_ARRAY+4])(Obj);
}

typedef void* (*Type_Array_ReadData)(PdAbstractArray, C_Int32 const *,
	C_Int32 const *, void *, enum C_SVType);
COREARRAY_DLL_LOCAL void *GDS_Array_ReadData(PdAbstractArray Obj,
	C_Int32 const* Start, C_Int32 const* Length, void *OutBuf,
	enum C_SVType OutSV)
{
	return (*(Type_Array_ReadData)c_api[IDX_ARRAY+5])(Obj,
		Start, Length, OutBuf, OutSV);
}

typedef void* (*Type_Array_ReadDataEx)(PdAbstractArray, C_Int32 const *,
	C_Int32 const *, const C_BOOL *const [], void *, enum C_SVType OutSV);
COREARRAY_DLL_LOCAL void *GDS_Array_ReadDataEx(PdAbstractArray Obj,
	C_Int32 const* Start, C_Int32 const* Length,
	const C_BOOL *const Selection[], void *OutBuf, enum C_SVType OutSV)
{
	return (*(Type_Array_ReadDataEx)c_api[IDX_ARRAY+6])(Obj,
		Start, Length, Selection, OutBuf, OutSV);
}

typedef const void* (*Type_Array_WriteData)(PdAbstractArray, C_Int32 const *,
	C_Int32 const *, const void *, enum C_SVType);
COREARRAY_DLL_LOCAL const void *GDS_Array_WriteData(PdAbstractArray Obj,
	C_Int32 const* Start, C_Int32 const* Length, const void *InBuf,
	enum C_SVType InSV)
{
	return (*(Type_Array_WriteData)c_api[IDX_ARRAY+7])(Obj,
		Start, Length, InBuf, InSV);
}

typedef const void* (*Type_Array_AppendData)(PdAbstractArray, ssize_t, const void *,
	enum C_SVType);
COREARRAY_DLL_LOCAL const void *GDS_Array_AppendData(PdAbstractArray Obj, ssize_t Cnt,
	const void *InBuf, enum C_SVType InSV)
{
	return (*(Type_Array_AppendData)c_api[IDX_ARRAY+8])(Obj, Cnt, InBuf, InSV);
}

typedef void (*Type_Array_AppendString)(PdAbstractArray, const char *);
COREARRAY_DLL_LOCAL void GDS_Array_AppendString(PdAbstractArray Obj,
	const char *Text)
{
	(*(Type_Array_AppendString)c_api[IDX_ARRAY+9])(Obj, Text);
}

typedef void (*Type_Array_AppendStrLen)(PdAbstractArray, const char *, size_t);
COREARRAY_DLL_LOCAL void GDS_Array_AppendStrLen(PdAbstractArray Obj,
	const char *Text, size_t Len)
{
	(*(Type_Array_AppendStrLen)c_api[IDX_ARRAY+10])(Obj, Text, Len);
}



// ===========================================================================
// functions for TdIterator

#define IDX_ITER    (IDX_ARRAY + 11)
// (TFUNC)GDS_Iter_GetStart
// (TFUNC)GDS_Iter_GetEnd
// (TFUNC)GDS_Iter_GetHandle
// (TFUNC)GDS_Iter_Offset
// (TFUNC)GDS_Iter_Position
// (TFUNC)GDS_Iter_GetInt
// (TFUNC)GDS_Iter_GetFloat
// (TFUNC)GDS_Iter_GetStr
// (TFUNC)GDS_Iter_SetInt
// (TFUNC)GDS_Iter_SetFloat
// (TFUNC)GDS_Iter_SetStr
// (TFUNC)GDS_Iter_RData
// (TFUNC)GDS_Iter_RDataEx
// (TFUNC)GDS_Iter_WData

typedef void (*Type_Iter_GetStart)(PdContainer, PdIterator);
COREARRAY_DLL_LOCAL void GDS_Iter_GetStart(PdContainer Node, PdIterator Out)
{
	(*(Type_Iter_GetStart)c_api[IDX_ITER+0])(Node, Out);
}

typedef void (*Type_Iter_GetEnd)(PdContainer, PdIterator);
COREARRAY_DLL_LOCAL void GDS_Iter_GetEnd(PdContainer Node, PdIterator Out)
{
	(*(Type_Iter_GetEnd)c_api[IDX_ITER+1])(Node, Out);
}

typedef PdContainer (*Type_Iter_GetHandle)(PdIterator);
COREARRAY_DLL_LOCAL PdContainer GDS_Iter_GetHandle(PdIterator I)
{
	return (*(Type_Iter_GetHandle)c_api[IDX_ITER+2])(I);
}

typedef void (*Type_Iter_Offset)(PdIterator, C_Int64);
COREARRAY_DLL_LOCAL void GDS_Iter_Offset(PdIterator I, C_Int64 Offset)
{
	(*(Type_Iter_Offset)c_api[IDX_ITER+3])(I, Offset);
}

typedef void (*Type_Iter_Position)(PdContainer, PdIterator, C_Int64);
COREARRAY_DLL_LOCAL void GDS_Iter_Position(PdContainer Node, PdIterator Out, C_Int64 Offset)
{
	(*(Type_Iter_Position)c_api[IDX_ITER+4])(Node, Out, Offset);
}

typedef C_Int64 (*Type_Iter_GetInt)(PdIterator);
COREARRAY_DLL_LOCAL C_Int64 GDS_Iter_GetInt(PdIterator I)
{
	return (*(Type_Iter_GetInt)c_api[IDX_ITER+5])(I);
}

typedef C_Float64 (*Type_Iter_GetFloat)(PdIterator);
COREARRAY_DLL_LOCAL C_Float64 GDS_Iter_GetFloat(PdIterator I)
{
	return (*(Type_Iter_GetFloat)c_api[IDX_ITER+6])(I);
}

typedef void (*Type_Iter_GetStr)(PdIterator, char *, size_t);
COREARRAY_DLL_LOCAL void GDS_Iter_GetStr(PdIterator I, char *Out, size_t Size)
{
	(*(Type_Iter_GetStr)c_api[IDX_ITER+7])(I, Out, Size);
}

typedef void (*Type_Iter_SetInt)(PdIterator, C_Int64);
COREARRAY_DLL_LOCAL void GDS_Iter_SetInt(PdIterator I, C_Int64 Val)
{
	(*(Type_Iter_SetInt)c_api[IDX_ITER+8])(I, Val);
}

typedef void (*Type_Iter_SetFloat)(PdIterator, C_Float64);
COREARRAY_DLL_LOCAL void GDS_Iter_SetFloat(PdIterator I, C_Float64 Val)
{
	(*(Type_Iter_SetFloat)c_api[IDX_ITER+9])(I, Val);
}

typedef void (*Type_Iter_SetStr)(PdIterator, const char *);
COREARRAY_DLL_LOCAL void GDS_Iter_SetStr(PdIterator I, const char *Str)
{
	(*(Type_Iter_SetStr)c_api[IDX_ITER+10])(I, Str);
}

typedef void* (*Type_Iter_RData)(PdIterator, void *, size_t, enum C_SVType);
COREARRAY_DLL_LOCAL void *GDS_Iter_RData(PdIterator I, void *OutBuf,
	size_t Cnt, enum C_SVType OutSV)
{
	return (*(Type_Iter_RData)c_api[IDX_ITER+11])(I, OutBuf, Cnt, OutSV);
}

typedef void* (*Type_Iter_RDataEx)(PdIterator, void *, size_t, enum C_SVType, const C_BOOL*);
COREARRAY_DLL_LOCAL void *GDS_Iter_RDataEx(PdIterator I, void *OutBuf,
	size_t Cnt, enum C_SVType OutSV, const C_BOOL Selection[])
{
	return (*(Type_Iter_RDataEx)c_api[IDX_ITER+12])(I, OutBuf, Cnt, OutSV, Selection);
}

typedef const void* (*Type_Iter_WData)(PdIterator, const void *, size_t, enum C_SVType);
COREARRAY_DLL_LOCAL const void *GDS_Iter_WData(PdIterator I, const void *InBuf,
	size_t Cnt, enum C_SVType InSV)
{
	return (*(Type_Iter_WData)c_api[IDX_ITER+13])(I, InBuf, Cnt, InSV);
}



// ===========================================================================
// functions for error

#define IDX_ERR    (IDX_ITER + 14)
// (TFUNC)GDS_GetError
// (TFUNC)GDS_SetError

typedef const char *(*Type_GetError)();
COREARRAY_DLL_LOCAL const char *GDS_GetError()
{
	return (*(Type_GetError)c_api[IDX_ERR+0])();
}

typedef void (*Type_SetError)(const char *);
COREARRAY_DLL_LOCAL void GDS_SetError(const char *Msg)
{
	(*(Type_SetError)c_api[IDX_ERR+1])(Msg);
}


// ===========================================================================
// functions for machine

#define IDX_MACH    (IDX_ERR + 2)
// (TFUNC)GDS_Mach_GetNumOfCores,
// (TFUNC)GDS_Mach_GetCPULevelCache

typedef int (*Type_Mach_GetNumOfCores)();
COREARRAY_DLL_EXPORT int GDS_Mach_GetNumOfCores()
{
	return (*(Type_Mach_GetNumOfCores)c_api[IDX_MACH+0])();
}

typedef size_t (*Type_Mach_GetCPULevelCache)(int);
COREARRAY_DLL_EXPORT C_UInt64 GDS_Mach_GetCPULevelCache(int level)
{
	return (*(Type_Mach_GetCPULevelCache)c_api[IDX_MACH+1])(level);
}

typedef size_t (*Type_Mach_Finite)(double);
COREARRAY_DLL_EXPORT int GDS_Mach_Finite(double val)
{
	return (*(Type_Mach_Finite)c_api[IDX_MACH+2])(val);
}





/*
// ===========================================================================
// functions for parallel computing

typedef PdThreadMutex (*Type_Parallel_InitMutex)();
static Type_Parallel_InitMutex func_Parallel_InitMutex = NULL;
COREARRAY_DLL_LOCAL PdThreadMutex GDS_Parallel_InitMutex()
{
	return (*func_Parallel_InitMutex)();
}

typedef void (*Type_Parallel_DoneMutex)(PdThreadMutex);
static Type_Parallel_DoneMutex func_Parallel_DoneMutex = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_DoneMutex(PdThreadMutex Obj)
{
	(*func_Parallel_DoneMutex)(Obj);
}

typedef void (*Type_Parallel_LockMutex)(PdThreadMutex);
static Type_Parallel_LockMutex func_Parallel_LockMutex = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_LockMutex(PdThreadMutex Obj)
{
	(*func_Parallel_LockMutex)(Obj);
}

typedef C_BOOL (*Type_Parallel_TryLockMutex)(PdThreadMutex);
static Type_Parallel_TryLockMutex func_Parallel_TryLockMutex = NULL;
COREARRAY_DLL_LOCAL C_BOOL GDS_Parallel_TryLockMutex(PdThreadMutex Obj)
{
	return (*func_Parallel_TryLockMutex)(Obj);
}

typedef void (*Type_Parallel_UnlockMutex)(PdThreadMutex);
static Type_Parallel_UnlockMutex func_Parallel_UnlockMutex = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_UnlockMutex(PdThreadMutex Obj)
{
	(*func_Parallel_UnlockMutex)(Obj);
}

typedef PdThreadCondition (*Type_Parallel_InitCondition)();
static Type_Parallel_InitCondition func_Parallel_InitCondition = NULL;
COREARRAY_DLL_LOCAL PdThreadCondition GDS_Parallel_InitCondition()
{
	return (*func_Parallel_InitCondition)();
}

typedef void (*Type_Parallel_Condition)(PdThreadCondition);
static Type_Parallel_Condition func_Parallel_DoneCondition = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_DoneCondition(PdThreadCondition Obj)
{
	(*func_Parallel_DoneCondition)(Obj);
}

static Type_Parallel_Condition func_Parallel_SignalCondition = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_SignalCondition(PdThreadCondition Obj)
{
	(*func_Parallel_SignalCondition)(Obj);
}

static Type_Parallel_Condition func_Parallel_BroadcastCondition = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_BroadcastCondition(PdThreadCondition Obj)
{
	(*func_Parallel_BroadcastCondition)(Obj);
}

typedef void (*Type_Parallel_WaitCondition)(PdThreadCondition, PdThreadMutex);
static Type_Parallel_WaitCondition func_Parallel_WaitCondition = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_WaitCondition(PdThreadCondition Obj,
	PdThreadMutex Mutex)
{
	(*func_Parallel_WaitCondition)(Obj, Mutex);
}

typedef PdThreadsSuspending (*Type_Parallel_InitSuspend)();
static Type_Parallel_InitSuspend func_Parallel_InitSuspend = NULL;
COREARRAY_DLL_LOCAL PdThreadsSuspending GDS_Parallel_InitSuspend()
{
	return (*func_Parallel_InitSuspend)();
}

typedef void (*Type_Parallel_DoneSuspend)(PdThreadsSuspending);
static Type_Parallel_DoneSuspend func_Parallel_DoneSuspend = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_DoneSuspend(PdThreadsSuspending Obj)
{
	(*func_Parallel_DoneSuspend)(Obj);
}

typedef void (*Type_Parallel_Suspend)(PdThreadsSuspending);
static Type_Parallel_Suspend func_Parallel_Suspend = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_Suspend(PdThreadsSuspending Obj)
{
	(*func_Parallel_Suspend)(Obj);
}

typedef void (*Type_Parallel_WakeUp)(PdThreadsSuspending);
static Type_Parallel_WakeUp func_Parallel_WakeUp = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_WakeUp(PdThreadsSuspending Obj)
{
	(*func_Parallel_WakeUp)(Obj);
}

typedef void (*Type_Parallel_RunThreads)(void (*)(PdThread, int, void*),
	void *, int);
static Type_Parallel_RunThreads func_Parallel_RunThreads = NULL;
COREARRAY_DLL_LOCAL void GDS_Parallel_RunThreads(
	void (*Proc)(PdThread, int, void*), void *Param, int nThread)
{
	(*func_Parallel_RunThreads)(Proc, Param, nThread);
}


// ===========================================================================
// functions for reading block by block

typedef PdArrayRead (*Type_ArrayRead_Init)(PdAbstractArray, int, enum C_SVType,
	const C_BOOL *const [], C_BOOL);
static Type_ArrayRead_Init func_ArrayRead_Init = NULL;
COREARRAY_DLL_LOCAL PdArrayRead GDS_ArrayRead_Init(PdAbstractArray Obj,
	int Margin, enum C_SVType SVType, const C_BOOL *const Selection[],
	C_BOOL buf_if_need)
{
	return (*func_ArrayRead_Init)(Obj, Margin, SVType, Selection, buf_if_need);
}

typedef void (*Type_ArrayRead_Free)(PdArrayRead);
static Type_ArrayRead_Free func_ArrayRead_Free = NULL;
COREARRAY_DLL_LOCAL void GDS_ArrayRead_Free(PdArrayRead Obj)
{
	(*func_ArrayRead_Free)(Obj);
}

typedef void (*Type_ArrayRead_Read)(PdArrayRead, void *);
static Type_ArrayRead_Read func_ArrayRead_Read = NULL;
COREARRAY_DLL_LOCAL void GDS_ArrayRead_Read(PdArrayRead Obj, void *Buffer)
{
	(*func_ArrayRead_Read)(Obj, Buffer);
}

typedef C_BOOL (*Type_ArrayRead_Eof)(PdArrayRead);
static Type_ArrayRead_Eof func_ArrayRead_Eof = NULL;
COREARRAY_DLL_LOCAL C_BOOL GDS_ArrayRead_Eof(PdArrayRead Obj)
{
	return (*func_ArrayRead_Eof)(Obj);
}

typedef void (*Type_ArrayRead_BalanceBuffer)(PdArrayRead[], int, C_Int64);
static Type_ArrayRead_BalanceBuffer func_ArrayRead_BalanceBuffer = NULL;
COREARRAY_DLL_LOCAL void GDS_ArrayRead_BalanceBuffer(PdArrayRead array[],
	int n, C_Int64 buffer_size)
{
	(*func_ArrayRead_BalanceBuffer)(array, n, buffer_size);
}





// ===========================================================================
// R objects

typedef PdGDSFile (*Type_R_SEXP2File)(SEXP);
static Type_R_SEXP2File func_R_SEXP2File = NULL;
COREARRAY_DLL_LOCAL PdGDSFile GDS_R_SEXP2File(SEXP File)
{
	return (*func_R_SEXP2File)(File);
}

typedef PdGDSFolder (*Type_R_SEXP2FileRoot)(SEXP);
static Type_R_SEXP2FileRoot func_R_SEXP2FileRoot = NULL;
COREARRAY_DLL_LOCAL PdGDSFolder GDS_R_SEXP2FileRoot(SEXP File)
{
	return (*func_R_SEXP2FileRoot)(File);
}

typedef PdGDSObj (*Type_R_SEXP2Obj)(SEXP, C_BOOL);
static Type_R_SEXP2Obj func_R_SEXP2Obj = NULL;
COREARRAY_DLL_LOCAL PdGDSObj GDS_R_SEXP2Obj(SEXP Obj, C_BOOL ReadOnly)
{
	return (*func_R_SEXP2Obj)(Obj, ReadOnly);
}

typedef SEXP (*Type_R_Obj2SEXP)(PdGDSObj);
static Type_R_Obj2SEXP func_R_Obj2SEXP = NULL;
COREARRAY_DLL_LOCAL SEXP GDS_R_Obj2SEXP(PdGDSObj Obj)
{
	return (*func_R_Obj2SEXP)(Obj);
}

typedef void (*Type_R_Obj_SEXP2SEXP)(SEXP, SEXP);
static Type_R_Obj_SEXP2SEXP func_R_Obj_SEXP2SEXP = NULL;
COREARRAY_DLL_LOCAL void GDS_R_Obj_SEXP2SEXP(SEXP ObjDst, SEXP ObjSrc)
{
	(*func_R_Obj_SEXP2SEXP)(ObjDst, ObjSrc);
}

typedef C_BOOL (*Type_R_Is_Logical)(PdGDSObj);
static Type_R_Is_Logical func_R_Is_Logical = NULL;
COREARRAY_DLL_LOCAL C_BOOL GDS_R_Is_Logical(PdGDSObj Obj)
{
	return (*func_R_Is_Logical)(Obj);
}

typedef C_BOOL (*Type_R_Is_Factor)(PdGDSObj);
static Type_R_Is_Factor func_R_Is_Factor = NULL;
COREARRAY_DLL_LOCAL C_BOOL GDS_R_Is_Factor(PdGDSObj Obj)
{
	return (*func_R_Is_Factor)(Obj);
}

typedef int (*Type_R_Set_IfFactor)(PdGDSObj, SEXP);
static Type_R_Set_IfFactor func_R_Set_IfFactor = NULL;
COREARRAY_DLL_LOCAL int GDS_R_Set_IfFactor(PdGDSObj Obj, SEXP Val)
{
	return (*func_R_Set_IfFactor)(Obj, Val);
}

typedef SEXP (*Type_R_Array_Read)(PdAbstractArray, const C_Int32 *,
	const C_Int32 *, const C_BOOL *const [], C_UInt32);
static Type_R_Array_Read func_R_Array_Read = NULL;
COREARRAY_DLL_LOCAL SEXP GDS_R_Array_Read(PdAbstractArray Obj,
	const C_Int32 *Start, const C_Int32 *Length,
	const C_BOOL *const Selection[], C_UInt32 UseMode)
{
	return (*func_R_Array_Read)(Obj, Start, Length, Selection, UseMode);
}

typedef void (*Type_R_Apply)(int, PdAbstractArray [], int [],
	const C_BOOL *const * const [],
	void (*)(SEXP, C_Int32, PdArrayRead [], void *),
	void (*)(SEXP, C_Int32, void *), void *, C_BOOL, C_UInt32 UseMode);
static Type_R_Apply func_R_Apply = NULL;
COREARRAY_DLL_LOCAL void GDS_R_Apply(int Num, PdAbstractArray ObjList[],
	int Margins[], const C_BOOL *const * const Selection[],
	void (*InitFunc)(SEXP Argument, C_Int32 Count, PdArrayRead ReadObjList[],
		void *_Param),
	void (*LoopFunc)(SEXP Argument, C_Int32 Idx, void *_Param),
	void *Param, C_BOOL IncOrDec, C_UInt32 UseMode)
{
	(*func_R_Apply)(Num, ObjList, Margins, Selection, InitFunc, LoopFunc,
		Param, IncOrDec, UseMode);
}

typedef void (*Type_R_Append)(PdAbstractArray, SEXP);
static Type_R_Append func_R_Append = NULL;
COREARRAY_DLL_LOCAL void GDS_R_Append(PdAbstractArray Obj, SEXP Data)
{
	(*func_R_Append)(Obj, Data);
}

typedef void (*Type_R_AppendEx)(PdAbstractArray, SEXP, size_t, size_t);
static Type_R_AppendEx func_R_AppendEx = NULL;
COREARRAY_DLL_LOCAL void GDS_R_AppendEx(PdAbstractArray Obj, SEXP Data,
	size_t Start, size_t Count)
{
	(*func_R_AppendEx)(Obj, Data, Start, Count);
}

typedef void (*Type_R_Is_Element)(PdAbstractArray, SEXP, C_BOOL[]);
static Type_R_Is_Element func_R_Is_Element = NULL;
COREARRAY_DLL_LOCAL void GDS_R_Is_Element(PdAbstractArray Obj, SEXP SetEL,
	C_BOOL Out[])
{
	(*func_R_Is_Element)(Obj, SetEL, Out);
}

*/



// ===========================================================================

/// initialize the GDS routines
int Init_GDS_Routines()
{
	PyObject *pkg = PyImport_ImportModule("pygds.ccall");
	if (pkg == NULL)
	{
		PyErr_SetString(PyExc_ImportError, "pygds.ccall failed to import");
		return -1;
	}

	PyObject *api = NULL;
	api = PyObject_GetAttrString(pkg, "_GDS_C_API");
	Py_DECREF(pkg);
	if (api == NULL)
	{
		PyErr_SetString(PyExc_AttributeError, "_GDS_C_API not found");
		return -1;
	}

	if (!PyCapsule_CheckExact(api))
	{
		PyErr_SetString(PyExc_RuntimeError, "_GDS_C_API is not PyCapsule object");
		Py_DECREF(api);
		return -1;
	}
	c_api = (TFUNC*)PyCapsule_GetPointer(api, NULL);

	Py_DECREF(api);
	if (c_api == NULL)
	{
		PyErr_SetString(PyExc_RuntimeError, "_GDS_C_API is NULL pointer");
		return -1;
	}

	return 0;
}


#ifdef __cplusplus
}
#endif

#endif /* _PYGDS_C_FILE_ */
