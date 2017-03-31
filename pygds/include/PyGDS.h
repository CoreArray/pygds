// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// PyGDS.h: C interface to pygds dynamic library
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


#ifndef _HEADER_PY_GDS_
#define _HEADER_PY_GDS_

#include <dType.h>
#include <Python.h>


#ifdef __cplusplus
#   ifdef COREARRAY_PYGDS_PACKAGE
#       include <CoreArray.h>
        using namespace CoreArray;
#   endif

extern "C" {
#endif

	// ==================================================================

	/// Version of package pygds: v0.1
	#define PYGDS_VERSION       0x000100


	// [[ ********
	#ifndef COREARRAY_PYGDS_PACKAGE

	// ====  the interface for CoreArray library  ====

	/// the pointer to a GDS file
	typedef void* PdGDSFile;
	/// the pionter to a GDS node
	typedef void* PdGDSObj;
	/// the pointer to a GDS folder
	typedef void* PdGDSFolder;
	/// the pointer to a container object
	typedef void* PdContainer;
	/// the pointer to a sequence object
	typedef void* PdAbstractArray;

	/// the class of mutex object
	typedef void* PdThreadMutex;
	/// the class of condition object
	typedef void* PdThreadCondition;
	/// the class of suspending object
	typedef void* PdThreadsSuspending;
	/// the class of thread object
	typedef void* PdThread;

	/// the class of block read
	typedef void* PdArrayRead;


	/// the iterator for CoreArray array-oriented container
	struct CdIterator
	{
		void *Allocator;
		C_Int64 Ptr;
		void *Container;
	};

	/// the pointer to an iterator
	typedef struct CdIterator *PdIterator;


	#endif  // COREARRAY_PYGDS_PACKAGE
	// ]] ********



	// ==================================================================
	// ==================================================================

	#if (PY_MAJOR_VERSION>3) || (PY_MAJOR_VERSION==3 && PY_MINOR_VERSION>=3)
	#   define BSTR    "p"
	#else
	#   define BSTR    "i"
	#endif

	#if (PY_MAJOR_VERSION >= 3)

    #   define PyInt_FromLong        PyLong_FromLong
    #   define PyInt_AsLong          PyLong_AsLong
	#   define PYSTR_SET(s)          PyUnicode_FromString(s)
	#   define PYSTR_SET2(s, len)    PyUnicode_FromStringAndSize(s, len)

	#else

	#   define PYSTR_SET(s)          PyString_FromString(s)
	#   define PYSTR_SET2(s, len)    PyString_FromStringAndSize(s, len)
	#   define PyCapsule_New(p, name, destructor)    (PyCObject_FromVoidPtr(p, destructor))
	#   define PyCapsule_CheckExact(p)    (PyCObject_Check(p))
	#   define PyCapsule_GetPointer(capsule, name)    (PyCObject_AsVoidPtr(capsule))

	#endif



	// ==================================================================
	// ==================================================================

	/// the maximum number of GDS files
	#define PKG_MAX_NUM_GDS_FILES    1024

	/// the maximun number of dimensions in GDS array (it has been specified in the library)
	#define GDS_MAX_NUM_DIMENSION    256



	// ==================================================================
	// Python objects

	/// convert "file_id  --> (CdGDSFile*)"
	extern PdGDSFile GDS_ID2File(int file_id);
	/// convert "file_id  --> (CdGDSFolder*)"
	extern PdGDSFolder GDS_ID2FileRoot(int file_id);

/*
	/// convert "SEXP  --> (CdGDSObj*)"
	extern PdGDSObj GDS_R_SEXP2Obj(SEXP Obj, C_BOOL ReadOnly);

	/// convert "(CdGDSObj*)  -->  SEXP", true for read-only
	extern SEXP GDS_R_Obj2SEXP(PdGDSObj Obj);
	/// convert "SEXP (ObjSrc)  -->  SEXP (ObjDst)", (requiring >= v1.5.8)
	extern void GDS_R_Obj_SEXP2SEXP(SEXP ObjDst, SEXP ObjSrc);
*/

	/// return true, if Obj is a logical object in R (equivalent to BOOL)
	extern C_BOOL GDS_Is_RLogical(PdGDSObj Obj);
	/// return true, if Obj is a factor variable in R
	extern C_BOOL GDS_Is_RFactor(PdGDSObj Obj);

/*
	/// return 1 used in UNPROTECT and set levels in 'Val' if Obj is a factor in R; otherwise return 0
	extern int GDS_R_Set_IfFactor(PdGDSObj Obj, SEXP Val);
*/

	/// return an R data object from a GDS object, allowing raw-type data
	extern PyObject* GDS_Py_Array_Read(PdAbstractArray Obj, const C_Int32 *Start,
		const C_Int32 *Length, const C_BOOL *const Selection[],
		enum C_SVType SV);

/*
	/// apply user-defined function margin by margin
	extern void GDS_R_Apply(int Num, PdAbstractArray ObjList[],
		int Margins[], const C_BOOL *const * const Selection[],
		void (*InitFunc)(SEXP Argument, C_Int32 Count,
			PdArrayRead ReadObjList[], void *_Param),
		void (*LoopFunc)(SEXP Argument, C_Int32 Idx, void *_Param),
		void *Param, C_BOOL IncOrDec, C_UInt32 UseMode);
	/// append R data
	extern void GDS_R_Append(PdAbstractArray Obj, SEXP Val);
	/// append R data with a range
	extern void GDS_R_AppendEx(PdAbstractArray Obj, SEXP Val, size_t Start,
		size_t Count);
	/// return whether the elements in SetEL
	extern void GDS_R_Is_Element(PdAbstractArray Obj, SEXP SetEL, C_BOOL Out[]);
*/


	// ==================================================================
	// File structure

	/// create a GDS file
	extern PdGDSFile GDS_File_Create(const char *FileName);
	/// open an existing GDS file
	extern PdGDSFile GDS_File_Open(const char *FileName, C_BOOL ReadOnly,
		C_BOOL ForkSupport);
	/// close the GDS file
	extern void GDS_File_Close(PdGDSFile File);
	/// synchronize the GDS file
	extern void GDS_File_Sync(PdGDSFile File);
	/// get the root folder of a GDS file
	extern PdGDSFolder GDS_File_Root(PdGDSFile File);
	/// get the GDS file from a GDS node
	extern PdGDSFile GDS_Node_File(PdGDSObj Node);
	/// delete the GDS variable (requiring >= 1.5.9)
	extern void GDS_Node_Delete(PdGDSObj Node, C_BOOL Force);
	/// get the class name of a GDS node
	extern void GDS_Node_GetClassName(PdGDSObj Node, char *OutStr, size_t OutSize);
	/// get the number of nodes in the folder
	extern int GDS_Node_ChildCount(PdGDSFolder Node);
	/// get a GDS file specified by a path
	extern PdGDSObj GDS_Node_Path(PdGDSFolder Node, const char *Path,
		C_BOOL MustExist);



	// ==================================================================
	// Attribute functions

	/// get the number of attributes
	extern int GDS_Attr_Count(PdGDSObj Node);
	/// get the attribute index with the name
	extern int GDS_Attr_Name2Index(PdGDSObj Node, const char *Name);



	// ==================================================================
	// CdAbstractArray methods

	/// get the number of dimensions
	extern int GDS_Array_DimCnt(PdAbstractArray Obj);
	/// get the dimension size
	extern void GDS_Array_GetDim(PdAbstractArray Obj, C_Int32 OutBuffer[],
		size_t N_Buf);
	/// get the total number of elements
	extern C_Int64 GDS_Array_GetTotalCount(PdAbstractArray Obj);
	/// get data type
	extern enum C_SVType GDS_Array_GetSVType(PdAbstractArray Obj);
	/// get the number of bits for an element
	extern unsigned GDS_Array_GetBitOf(PdAbstractArray Obj);
	/// read data
	/** \param Obj         GDS array object
	 *  \param Start       the starting positions (from ZERO), it could be NULL
	 *  \param Length      the lengths of each dimension, it could be NULL
	 *  \param OutBuffer   the pointer to the output buffer
	 *  \param OutSV       data type of output buffer
	**/
	extern void *GDS_Array_ReadData(PdAbstractArray Obj, const C_Int32 *Start,
		const C_Int32 *Length, void *OutBuf, enum C_SVType OutSV);
	/// read data with selection
	/** \param Obj         GDS array object
	 *  \param Start       the starting positions (from ZERO), it could be NULL
	 *  \param Length      the lengths of each dimension, it could be NULL
	 *  \param Selection   the array of selection, it could be NULL
	 *  \param OutBuffer   the pointer to the output buffer
	 *  \param OutSV       data type of output buffer
	**/
	extern void *GDS_Array_ReadDataEx(PdAbstractArray Obj, const C_Int32 *Start,
		const C_Int32 *Length, const C_BOOL *const Selection[], void *OutBuf,
		enum C_SVType OutSV);
	/// write data
	/** \param Obj         GDS array object
	 *  \param Start       the starting positions (from ZERO), it could be NULL
	 *  \param Length      the lengths of each dimension, it could be NULL
	 *  \param InBuf       the pointer to the input buffer
	 *  \param InSV        data type of input buffer
	**/
	extern const void *GDS_Array_WriteData(PdAbstractArray Obj,
		const C_Int32 *Start, const C_Int32 *Length, const void *InBuf,
		enum C_SVType InSV);
	/// append data
	extern const void *GDS_Array_AppendData(PdAbstractArray Obj, ssize_t Cnt,
		const void *InBuf, enum C_SVType InSV);
	/// append a string
	extern void GDS_Array_AppendString(PdAbstractArray Obj, const char *Text);
	/// append a string with maximum number of bytes
	extern void GDS_Array_AppendStrLen(PdAbstractArray Obj, const char *Text,
		size_t Len);



	// ==================================================================
	// TdIterator methods

	/// get the start iterator
	extern void GDS_Iter_GetStart(PdContainer Node, PdIterator Out);
	/// get the next iterator after the last one
	extern void GDS_Iter_GetEnd(PdContainer Node, PdIterator Out);
	/// get the GDS object with the iterator
	extern PdContainer GDS_Iter_GetHandle(PdIterator I);
	/// offset the iterator
	extern void GDS_Iter_Offset(PdIterator I, C_Int64 Offset);
	/// get the iterator according to offset from the beginning (requiring >= 1.5.11)
	extern void GDS_Iter_Position(PdContainer Node, PdIterator Out, C_Int64 Offset);
	/// get an integer according to the iterator
	extern C_Int64 GDS_Iter_GetInt(PdIterator I);
	/// get a numeric value according to the iterator
	extern C_Float64 GDS_Iter_GetFloat(PdIterator I);
	/// get a string according to the iterator
	extern void GDS_Iter_GetStr(PdIterator I, char *Out, size_t Size);
	/// set an integer according to the iterator
	extern void GDS_Iter_SetInt(PdIterator I, C_Int64 Val);
	/// set a numeric value according to the iterator
	extern void GDS_Iter_SetFloat(PdIterator I, C_Float64 Val);
	/// set a string according to the iterator
	extern void GDS_Iter_SetStr(PdIterator I, const char *Str);
	/// read data from the iterator
	extern void *GDS_Iter_RData(PdIterator I, void *OutBuf, size_t Cnt,
		enum C_SVType OutSV);
	/// read data from the iterator with a selection (requiring >= v1.5.11)
	extern void *GDS_Iter_RDataEx(PdIterator I, void *OutBuf, size_t Cnt,
		enum C_SVType OutSV, const C_BOOL Selection[]);
	/// write data to the iterator
	extern const void *GDS_Iter_WData(PdIterator I, const void *InBuf,
		size_t Cnt, enum C_SVType InSV);



	// ==================================================================
	// Error functions

	/// get the error message
	extern const char *GDS_GetError();
	/// set the error message
	extern void GDS_SetError(const char *Msg);



	// ==================================================================
	// Functions for machine

	/// return the number of available (logical) cores in the system
	extern int GDS_Mach_GetNumOfCores();
	/// return the size in byte of level-n cache memory
	extern C_UInt64 GDS_Mach_GetCPULevelCache(int level);
	/// return 1 if the value is finite, otherwise 0
	extern int GDS_Mach_Finite(double val);



	// ==================================================================
	// functions for reading block by block

	/// initialize the array object
	extern PdArrayRead GDS_ArrayRead_Init(PdAbstractArray Obj,
		int Margin, enum C_SVType SVType, const C_BOOL *const Selection[],
		C_BOOL buf_if_need);
	/// finalize the array object
	extern void GDS_ArrayRead_Free(PdArrayRead Obj);
	/// read data from the array object
	extern void GDS_ArrayRead_Read(PdArrayRead Obj, void *Buffer);
	/// return 1 if it is the end
	extern C_BOOL GDS_ArrayRead_Eof(PdArrayRead Obj);
	/// balance the buffers of multiple array objects according to the total buffer size
	extern void GDS_ArrayRead_BalanceBuffer(PdArrayRead array[], int n,
		C_Int64 buffer_size);



	// ==================================================================

	/// export pygds C API pointer
	extern void *PyGDS_API;

	#ifndef COREARRAY_PYGDS_PACKAGE

	/// initialize GDS interface, return -1 if fails
	extern int Init_GDS_Routines();

	#endif  // COREARRAY_PYGDS_PACKAGE


#ifdef __cplusplus
}
#endif

#endif /* _HEADER_PY_GDS_ */
