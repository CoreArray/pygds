// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// dAllocator.h: Storage allocation
//
// Copyright (C) 2007-2017    Xiuwen Zheng
//
// This file is part of CoreArray.
//
// CoreArray is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License Version 3 as
// published by the Free Software Foundation.
//
// CoreArray is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with CoreArray.
// If not, see <http://www.gnu.org/licenses/>.

/**
 *	\file     dAllocator.h
 *	\author   Xiuwen Zheng [zhengx@u.washington.edu]
 *	\version  1.0
 *	\date     2007 - 2017
 *	\brief    Storage allocation
 *	\details
**/

#ifndef _HEADER_COREARRAY_ALLOC_
#define _HEADER_COREARRAY_ALLOC_

#include "dBase.h"
#include "dTrait.h"

#include <cmath>
#include <cstring>
#include <vector>

#ifdef COREARRAY_PLATFORM_UNIX
#  include <sys/types.h>
#  include <unistd.h>
#endif


namespace CoreArray
{
	using namespace std;


	// =====================================================================
	// Allocator and Iterator
	// =====================================================================

	/// Allocator class
	class COREARRAY_DLL_DEFAULT CdAllocator: public CdAbstract
	{
	public:
		CdAllocator();
		~CdAllocator();

		void Initialize();
		void Initialize(CdStream &Stream, bool CanRead, bool CanWrite);
		void Free();

		COREARRAY_FORCEINLINE SIZE64 GetSize()
			{ return (*_GetSize)(*this); }
		COREARRAY_FORCEINLINE void SetSize(SIZE64 NewSize)
			{ (*_SetSize)(*this, NewSize); }
		COREARRAY_FORCEINLINE SIZE64 Position()
			{ return (*_GetPos)(*this); }
		COREARRAY_FORCEINLINE void SetPosition(SIZE64 NewPos)
			{ (*_SetPos)(*this, NewPos); }

		/// read block of data
		COREARRAY_FORCEINLINE void ReadData(void *Buffer, ssize_t Count)
			{ (*_Read)(*this, Buffer, Count); }
		/// read a 8-bit integer with native endianness
		COREARRAY_FORCEINLINE C_UInt8 R8b()
			{ return (*_R8b)(*this); }
		/// read a 16-bit integer with native endianness
		COREARRAY_FORCEINLINE C_UInt16 R16b()
			{ return (*_R16b)(*this); }
		/// read a 32-bit integer with native endianness
		COREARRAY_FORCEINLINE C_UInt32 R32b()
			{ return (*_R32b)(*this); }
		/// read a 64-bit integer with native endianness
		COREARRAY_FORCEINLINE C_UInt64 R64b()
			{ return (*_R64b)(*this); }

		/// read a 8-bit integer with native endianness
		COREARRAY_FORCEINLINE void ReadVar(C_Int8 &val) { val = (*_R8b)(*this); }
		/// read a 8-bit integer with native endianness
		COREARRAY_FORCEINLINE void ReadVar(C_UInt8 &val) { val = (*_R8b)(*this); }
		/// read a 16-bit integer with native endianness
		COREARRAY_FORCEINLINE void ReadVar(C_Int16 &val) { val = (*_R16b)(*this); }
		/// read a 16-bit integer with native endianness
		COREARRAY_FORCEINLINE void ReadVar(C_UInt16 &val) { val = (*_R16b)(*this); }
		/// read a 32-bit integer with native endianness
		COREARRAY_FORCEINLINE void ReadVar(C_Int32 &val) { val = (*_R32b)(*this); }
		/// read a 32-bit integer with native endianness
		COREARRAY_FORCEINLINE void ReadVar(C_UInt32 &val) { val = (*_R32b)(*this); }


		/// write block of data
		COREARRAY_FORCEINLINE void WriteData(const void *Buffer, ssize_t Count)
			{ (*_Write)(*this, Buffer, Count); }
		/// write a 8-bit integer with native endianness
		COREARRAY_FORCEINLINE void W8b(C_UInt8 val)
			{ (*_W8b)(*this, val); }
		/// write a 16-bit integer with native endianness
		COREARRAY_FORCEINLINE void W16b(C_UInt16 val)
			{ (*_W16b)(*this, val); }
		/// write a 32-bit integer with native endianness
		COREARRAY_FORCEINLINE void W32b(C_UInt32 val)
			{ (*_W32b)(*this, val); }
		/// write a 64-bit integer with native endianness
		COREARRAY_FORCEINLINE void W64b(C_UInt64 val)
			{ (*_W64b)(*this, val); }

		/// copies the values of Size bytes from Src to Dst
		void Move(SIZE64 Src, SIZE64 Dst, SIZE64 Size);

		/// fill zero from the current position
		void ZeroFill(SIZE64 Size);
		/// fill zero from Pos
		void ZeroFill(SIZE64 Pos, SIZE64 Size);

		/// copy to a buffer
		void CopyTo(CdBufStream &Obj, SIZE64 Pos, SIZE64 Count);


		COREARRAY_FORCEINLINE CdBufStream *BufStream()
			{ return _BufStream; }

	protected:
		typedef void (*TAllocFree)(CdAllocator &Obj);
		typedef SIZE64 (*TAllocGetSize)(CdAllocator &Obj);
		typedef void (*TAllocSetSize)(CdAllocator &Obj, SIZE64 NewSize);
		typedef SIZE64 (*TAllocGetPos)(CdAllocator &Obj);
		typedef void (*TAllocSetPos)(CdAllocator &Obj, SIZE64 NewPos);
		typedef void (*TAllocRead)(CdAllocator &Obj, void *Buffer, ssize_t Count);
		typedef C_UInt8 (*TAllocR8b)(CdAllocator &Obj);
		typedef C_UInt16 (*TAllocR16b)(CdAllocator &Obj);
		typedef C_UInt32 (*TAllocR32b)(CdAllocator &Obj);
		typedef C_UInt64 (*TAllocR64b)(CdAllocator &Obj);
		typedef void (*TAllocWrite)(CdAllocator &Obj, const void *Buffer, ssize_t Count);
		typedef void (*TAllocW8b)(CdAllocator &Obj, C_UInt8 val);
		typedef void (*TAllocW16b)(CdAllocator &Obj, C_UInt16 val);
		typedef void (*TAllocW32b)(CdAllocator &Obj, C_UInt32 val);
		typedef void (*TAllocW64b)(CdAllocator &Obj, C_UInt64 val);

		TAllocFree    _Free;
		TAllocGetSize _GetSize;
		TAllocSetSize _SetSize;
		TAllocGetPos  _GetPos;
		TAllocSetPos  _SetPos;

		TAllocRead    _Read;
		TAllocR8b     _R8b;
		TAllocR16b    _R16b;
		TAllocR32b    _R32b;
		TAllocR64b    _R64b;
		TAllocWrite   _Write;
		TAllocW8b     _W8b;
		TAllocW16b    _W16b;
		TAllocW32b    _W32b;
		TAllocW64b    _W64b;

		CdBufStream *_BufStream;

		static void _InitFree(CdAllocator &Obj);
		static SIZE64 _InitGetSize(CdAllocator &Obj);
		static void _InitSetSize(CdAllocator &Obj, SIZE64 NewSize);
		static SIZE64 _InitGetPos(CdAllocator &Obj);
		static void _InitSetPos(CdAllocator &Obj, SIZE64 NewPos);
		static void _InitRead(CdAllocator &Obj, void *Buffer, ssize_t Count);
		static C_UInt8 _InitR8b(CdAllocator &Obj);
		static C_UInt16 _InitR16b(CdAllocator &Obj);
		static C_UInt32 _InitR32b(CdAllocator &Obj);
		static C_UInt64 _InitR64b(CdAllocator &Obj);
		static void _InitWrite(CdAllocator &Obj, const void *Buffer, ssize_t Count);
		static void _InitW8b(CdAllocator &Obj, C_UInt8 val);
		static void _InitW16b(CdAllocator &Obj, C_UInt16 val);
		static void _InitW32b(CdAllocator &Obj, C_UInt32 val);
		static void _InitW64b(CdAllocator &Obj, C_UInt64 val);

		static void _BufFree(CdAllocator &Obj);
		static SIZE64 _BufGetSize(CdAllocator &Obj);
		static void _BufSetSize(CdAllocator &Obj, SIZE64 NewSize);
		static SIZE64 _BufGetPos(CdAllocator &Obj);
		static void _BufSetPos(CdAllocator &Obj, SIZE64 NewPos);
		static void _BufRead(CdAllocator &Obj, void *Buffer, ssize_t Count);
		static C_UInt8 _BufR8b(CdAllocator &Obj);
		static C_UInt16 _BufR16b(CdAllocator &Obj);
		static C_UInt32 _BufR32b(CdAllocator &Obj);
		static C_UInt64 _BufR64b(CdAllocator &Obj);
		static void _BufWrite(CdAllocator &Obj, const void *Buffer, ssize_t Count);
		static void _BufW8b(CdAllocator &Obj, C_UInt8 val);
		static void _BufW16b(CdAllocator &Obj, C_UInt16 val);
		static void _BufW32b(CdAllocator &Obj, C_UInt32 val);
		static void _BufW64b(CdAllocator &Obj, C_UInt64 val);

		static SIZE64 _NoGetSize(CdAllocator &Obj);
		static void _NoSetSize(CdAllocator &Obj, SIZE64 NewSize);
		static void _NoRead(CdAllocator &Obj, void *Buffer, ssize_t Count);
		static C_UInt8 _NoR8b(CdAllocator &Obj);
		static C_UInt16 _NoR16b(CdAllocator &Obj);
		static C_UInt32 _NoR32b(CdAllocator &Obj);
		static C_UInt64 _NoR64b(CdAllocator &Obj);
		static void _NoWrite(CdAllocator &Obj, const void *Buffer, ssize_t Count);
		static void _NoW8b(CdAllocator &Obj, C_UInt8 val);
		static void _NoW16b(CdAllocator &Obj, C_UInt16 val);
		static void _NoW32b(CdAllocator &Obj, C_UInt32 val);
		static void _NoW64b(CdAllocator &Obj, C_UInt64 val);
	};


	/// Iterator for CoreArray allocator object
	class COREARRAY_DLL_DEFAULT CdBaseIterator
	{
	public:
		/// the handler of this iterator
		CdAllocator *Allocator;
        /// a pointer
		SIZE64 Ptr;
	};

	/// The pointer to an iterator
	typedef CdBaseIterator *PdBaseIterator;



	// =====================================================================
	// Data Conversion
	// =====================================================================

	// Type Convert

	template<typename DestT, typename SourceT,
		int DTrait = TdTraits<DestT>::trVal,
		int STrait = TdTraits<SourceT>::trVal>
	struct COREARRAY_DLL_DEFAULT VAL_CONV
	{
		typedef DestT Type;

		COREARRAY_INLINE static DestT *Cvt(DestT *p, const SourceT *s, ssize_t n)
		{
			// loop unrolling
			for (; n >= 4; n -= 4, p += 4, s += 4)
			{
				p[0] = s[0]; p[1] = s[1];
				p[2] = s[2]; p[3] = s[3];
			}
			for (; n > 0; n--) *p++ = *s++;
			return p;
		}
		COREARRAY_INLINE static DestT *CvtSub(DestT *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			// loop unrolling
			for (; n >= 4; n -= 4, s += 4, sel += 4)
			{
				if (sel[0]) *p++ = s[0];
				if (sel[1]) *p++ = s[1];
				if (sel[2]) *p++ = s[2];
				if (sel[3]) *p++ = s[3];
			}
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = *s;
			return p;
		}
	};


	// Same type

	template<typename TYPE> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<TYPE, TYPE, COREARRAY_TR_INTEGER, COREARRAY_TR_INTEGER>
	{
		typedef TYPE Type;

		COREARRAY_INLINE static TYPE *Cvt(TYPE *p, const TYPE *s, ssize_t n)
		{
			memcpy(p, s, sizeof(TYPE)*n);
			return p + n;
		}
		COREARRAY_INLINE static TYPE *CvtSub(TYPE *p, const TYPE *s, ssize_t n, const C_BOOL sel[])
		{
			// loop unrolling
			for (; n >= 4; n -= 4, s += 4, sel += 4)
			{
				if (sel[0]) *p++ = s[0];
				if (sel[1]) *p++ = s[1];
				if (sel[2]) *p++ = s[2];
				if (sel[3]) *p++ = s[3];
			}
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = *s;
			return p;
		}
	};

	template<typename TYPE> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<TYPE, TYPE, COREARRAY_TR_FLOAT, COREARRAY_TR_FLOAT>
	{
		typedef TYPE Type;

		COREARRAY_INLINE static TYPE *Cvt(TYPE *p, const TYPE *s, ssize_t n)
		{
			memcpy(p, s, sizeof(TYPE)*n);
			return p + n;
		}
		COREARRAY_INLINE static TYPE *CvtSub(TYPE *p, const TYPE *s, ssize_t n, const C_BOOL sel[])
		{
			// loop unrolling
			for (; n >= 4; n -= 4, s += 4, sel += 4)
			{
				if (sel[0]) *p++ = s[0];
				if (sel[1]) *p++ = s[1];
				if (sel[2]) *p++ = s[2];
				if (sel[3]) *p++ = s[3];
			}
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = *s;
			return p;
		}
	};


	// Type Convert: float to integer

	template<typename DestT, typename SourceT>
		struct COREARRAY_DLL_DEFAULT
		VAL_CONV<DestT, SourceT, COREARRAY_TR_INTEGER, COREARRAY_TR_FLOAT>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = DestT(round(val)); }
			COREARRAY_INLINE operator DestT() const { return value; }
		private:
			DestT value;
		} Type;

		COREARRAY_INLINE static DestT *Cvt(DestT *p, const SourceT *s, ssize_t n)
		{
			// loop unrolling
			for (; n >= 4; n -= 4, p += 4, s += 4)
			{
				p[0] = DestT(round(s[0]));
				p[1] = DestT(round(s[1]));
				p[2] = DestT(round(s[2]));
				p[3] = DestT(round(s[3]));
			}
			for (; n > 0; n--) *p++ = DestT(*s++);
			return p;
		}
		COREARRAY_INLINE static DestT *CvtSub(DestT *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			// loop unrolling
			for (; n >= 4; n -= 4, s += 4, sel += 4)
			{
				if (sel[0]) *p++ = DestT(round(s[0]));
				if (sel[1]) *p++ = DestT(round(s[1]));
				if (sel[2]) *p++ = DestT(round(s[2]));
				if (sel[3]) *p++ = DestT(round(s[3]));
			}
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = DestT(round(*s++));
			return p;
		}
	};


	// Type Convert: string to integer

	template<typename DestT, typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<DestT, SourceT, COREARRAY_TR_INTEGER, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = StrToInt(RawText(val).c_str()); }
			COREARRAY_INLINE operator DestT() const { return value; }
		private:
			DestT value;
		} Type;

		COREARRAY_INLINE static DestT *Cvt(DestT *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = StrToInt(RawText(*s++).c_str());
			return p;
		}
		COREARRAY_INLINE static DestT *CvtSub(DestT *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = StrToInt(RawText(*s).c_str());
			return p;
		}
	};


	// Type Convert: string to float

	template<typename DestT, typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<DestT, SourceT, COREARRAY_TR_FLOAT, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = StrToFloat(RawText(val).c_str()); }
			COREARRAY_INLINE operator DestT() const { return value; }
		private:
			DestT value;
		} Type;

		COREARRAY_INLINE static DestT *Cvt(DestT *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = StrToFloat(RawText(*s++).c_str());
			return p;
		}
		COREARRAY_INLINE static DestT *CvtSub(DestT *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = StrToFloat(RawText(*s).c_str());
			return p;
		}
	};


	// Type Convert: integer to UTF8String

	template<typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF8String, SourceT, COREARRAY_TR_STRING, COREARRAY_TR_INTEGER>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = val; }
			COREARRAY_INLINE operator UTF8String() const { return ASC(IntToStr(value)); }
		private:
			SourceT value;
		} Type;

		COREARRAY_INLINE static UTF8String *Cvt(UTF8String *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = ASC(IntToStr(*s++));
			return p;
		}
		COREARRAY_INLINE static UTF8String *CvtSub(UTF8String *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = ASC(IntToStr(*s));
			return p;
		}
	};

	// Type Convert: float to UTF8String

	template<typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF8String, SourceT, COREARRAY_TR_STRING, COREARRAY_TR_FLOAT>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = val; }
			COREARRAY_INLINE operator UTF8String() const { return ASC(FloatToStr(value)); }
		private:
			SourceT value;
		} Type;

		COREARRAY_INLINE static UTF8String *Cvt(UTF8String *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = ASC(FloatToStr(*s++));
			return p;
		}
		COREARRAY_INLINE static UTF8String *CvtSub(UTF8String *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = ASC(FloatToStr(*s));
			return p;
		}
	};

	// Type Convert: UTF8String to UTF16String

	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF16String, UTF8String, COREARRAY_TR_STRING, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const UTF8String &val): value(val) { }
			COREARRAY_INLINE operator UTF16String() const { return UTF8ToUTF16(value); }
		private:
			const UTF8String &value;
		} Type;

		COREARRAY_INLINE static UTF16String *Cvt(UTF16String *p, const UTF8String *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = UTF8ToUTF16(*s++);
			return p;
		}
		COREARRAY_INLINE static UTF16String *CvtSub(UTF16String *p, const UTF8String *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = UTF8ToUTF16(*s);
			return p;
		}
	};

	// Type Convert: UTF8String to UTF32String

	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF32String, UTF8String, COREARRAY_TR_STRING, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const UTF8String &val): value(val) { }
			COREARRAY_INLINE operator UTF32String() const { return UTF8ToUTF32(value); }
		private:
			const UTF8String &value;
		} Type;

		COREARRAY_INLINE static UTF32String *Cvt(UTF32String *p, const UTF8String *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = UTF8ToUTF32(*s++);
			return p;
		}
		COREARRAY_INLINE static UTF32String *CvtSub(UTF32String *p, const UTF8String *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = UTF8ToUTF32(*s);
			return p;
		}
	};

	// Type Convert: integer to UTF16String

	template<typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF16String, SourceT, COREARRAY_TR_STRING, COREARRAY_TR_INTEGER>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = val; }
			COREARRAY_INLINE operator UTF16String() const { return ASC16(IntToStr(value)); }
		private:
			SourceT value;
		} Type;

		COREARRAY_INLINE static UTF16String *Cvt(UTF16String *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = ASC16(IntToStr(*s++));
			return p;
		}
		COREARRAY_INLINE static UTF16String *CvtSub(UTF16String *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = ASC16(IntToStr(*s));
			return p;
		}
	};

	// Type Convert: float to UTF16String

	template<typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF16String, SourceT, COREARRAY_TR_STRING, COREARRAY_TR_FLOAT>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = val; }
			COREARRAY_INLINE operator UTF16String() const { return ASC16(FloatToStr(value)); }
		private:
			SourceT value;
		} Type;

		COREARRAY_INLINE static UTF16String *Cvt(UTF16String *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = ASC16(FloatToStr(*s++));
			return p;
		}
		COREARRAY_INLINE static UTF16String *CvtSub(UTF16String *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = ASC16(FloatToStr(*s));
			return p;
		}
	};

	// Type Convert: UTF16String to UTF8String

	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF8String, UTF16String, COREARRAY_TR_STRING, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const UTF16String &val): value(val) { }
			COREARRAY_INLINE operator UTF8String() const { return UTF16ToUTF8(value); }
		private:
			const UTF16String &value;
		} Type;

		COREARRAY_INLINE static UTF8String *Cvt(UTF8String *p, const UTF16String *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = UTF16ToUTF8(*s++);
			return p;
		}
		COREARRAY_INLINE static UTF8String *CvtSub(UTF8String *p, const UTF16String *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = UTF16ToUTF8(*s);
			return p;
		}
	};

	// Type Convert: UTF16String to UTF32String

	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF32String, UTF16String, COREARRAY_TR_STRING, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const UTF16String &val): value(val) { }
			COREARRAY_INLINE operator UTF32String() const { return UTF16ToUTF32(value); }
		private:
			const UTF16String &value;
		} Type;

		COREARRAY_INLINE static UTF32String *Cvt(UTF32String *p, const UTF16String *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = UTF16ToUTF32(*s++);
			return p;
		}
		COREARRAY_INLINE static UTF32String *CvtSub(UTF32String *p, const UTF16String *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = UTF16ToUTF32(*s);
			return p;
		}
	};

	// Type Convert: integer to UTF32String

	template<typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF32String, SourceT, COREARRAY_TR_STRING, COREARRAY_TR_INTEGER>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = val; }
			COREARRAY_INLINE operator UTF32String() const { return ASC32(IntToStr(value)); }
		private:
			SourceT value;
		} Type;

		COREARRAY_INLINE static UTF32String *Cvt(UTF32String *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = ASC32(IntToStr(*s++));
			return p;
		}
		COREARRAY_INLINE static UTF32String *CvtSub(UTF32String *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = ASC32(IntToStr(*s));
			return p;
		}
	};

	// Type Convert: float to UTF32String

	template<typename SourceT> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF32String, SourceT, COREARRAY_TR_STRING, COREARRAY_TR_FLOAT>
	{
		typedef struct TType
		{
			TType(const SourceT &val) { value = val; }
			COREARRAY_INLINE operator UTF32String() const { return ASC32(FloatToStr(value)); }
		private:
			SourceT value;
		} Type;

		COREARRAY_INLINE static UTF32String *Cvt(UTF32String *p, const SourceT *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = ASC32(FloatToStr(*s++));
			return p;
		}
		COREARRAY_INLINE static UTF32String *CvtSub(UTF32String *p, const SourceT *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = ASC32(FloatToStr(*s));
			return p;
		}
	};

	// Type Convert: UTF32String to UTF8String

	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF8String, UTF32String, COREARRAY_TR_STRING, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const UTF32String &val): value(val) { }
			COREARRAY_INLINE operator UTF8String() const { return UTF32ToUTF8(value); }
		private:
			const UTF32String &value;
		} Type;

		COREARRAY_INLINE static UTF8String *Cvt(UTF8String *p, const UTF32String *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = UTF32ToUTF8(*s++);
			return p;
		}
		COREARRAY_INLINE static UTF8String *CvtSub(UTF8String *p, const UTF32String *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = UTF32ToUTF8(*s);
			return p;
		}
	};

	// Type Convert: UTF32String to UTF16String

	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<UTF16String, UTF32String, COREARRAY_TR_STRING, COREARRAY_TR_STRING>
	{
		typedef struct TType
		{
			TType(const UTF32String &val): value(val) { }
			COREARRAY_INLINE operator UTF16String() const { return UTF32ToUTF16(value); }
		private:
			const UTF32String &value;
		} Type;

		COREARRAY_INLINE static UTF16String *Cvt(UTF16String *p, const UTF32String *s, ssize_t n)
		{
			for (; n > 0; n--) *p++ = UTF32ToUTF16(*s++);
			return p;
		}
		COREARRAY_INLINE static UTF16String *CvtSub(UTF16String *p, const UTF32String *s, ssize_t n, const C_BOOL sel[])
		{
			for (; n > 0; n--, s++, sel++)
				if (*sel) *p++ = UTF32ToUTF16(*s);
			return p;
		}
	};


	// =====================================================================

#ifdef COREARRAY_SIMD_SSE2

	C_Int8* vec_simd_i32_to_i8(C_Int8 *p, const C_Int32 *s, size_t n);
	C_Int8* vec_simd_i32_to_i8_sel(C_Int8 *p, const C_Int32 *s, size_t n, const C_BOOL sel[]);

	/// Type Convert: int32 to int8
	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<C_Int8, C_Int32, COREARRAY_TR_INTEGER, COREARRAY_TR_INTEGER>
	{
		typedef C_Int8 Type;
		COREARRAY_INLINE static C_Int8 *Cvt(C_Int8 *p, const C_Int32 *s, ssize_t n)
			{ return vec_simd_i32_to_i8(p, s, n); }
		COREARRAY_INLINE static C_Int8 *CvtSub(C_Int8 *p, const C_Int32 *s, ssize_t n, const C_BOOL sel[])
			{ return vec_simd_i32_to_i8_sel(p, s, n, sel); }
	};

	/// Type Convert: int32 to uint8
	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<C_UInt8, C_Int32, COREARRAY_TR_INTEGER, COREARRAY_TR_INTEGER>
	{
		typedef C_UInt8 Type;
		COREARRAY_INLINE static C_UInt8 *Cvt(C_UInt8 *p, const C_Int32 *s, ssize_t n)
			{ return (C_UInt8*)vec_simd_i32_to_i8((C_Int8*)p, s, n); }
		COREARRAY_INLINE static C_UInt8 *CvtSub(C_UInt8 *p, const C_Int32 *s, ssize_t n, const C_BOOL sel[])
			{ return (C_UInt8*)vec_simd_i32_to_i8_sel((C_Int8*)p, s, n, sel); }
	};

	/// Type Convert: uint32 to int8
	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<C_Int8, C_UInt32, COREARRAY_TR_INTEGER, COREARRAY_TR_INTEGER>
	{
		typedef C_Int8 Type;
		COREARRAY_INLINE static C_Int8 *Cvt(C_Int8 *p, const C_UInt32 *s, ssize_t n)
			{ return vec_simd_i32_to_i8(p, (C_Int32*)s, n); }
		COREARRAY_INLINE static C_Int8 *CvtSub(C_Int8 *p, const C_UInt32 *s, ssize_t n, const C_BOOL sel[])
			{ return vec_simd_i32_to_i8_sel(p, (C_Int32*)s, n, sel); }
	};

	/// Type Convert: uint32 to uint8
	template<> struct COREARRAY_DLL_DEFAULT
		VAL_CONV<C_UInt8, C_UInt32, COREARRAY_TR_INTEGER, COREARRAY_TR_INTEGER>
	{
		typedef C_UInt8 Type;
		COREARRAY_INLINE static C_UInt8 *Cvt(C_UInt8 *p, const C_UInt32 *s, ssize_t n)
			{ return (C_UInt8*)vec_simd_i32_to_i8((C_Int8*)p, (C_Int32*)s, n); }
		COREARRAY_INLINE static C_UInt8 *CvtSub(C_UInt8 *p, const C_UInt32 *s, ssize_t n, const C_BOOL sel[])
			{ return (C_UInt8*)vec_simd_i32_to_i8_sel((C_Int8*)p, (C_Int32*)s, n, sel); }
	};

#endif


	// =====================================================================

	/// Conversion from SRC_TYPE to DEST_TYPE
	#define VAL_CONVERT(DEST_TYPE, SRC_TYPE, VAL)  (typename VAL_CONV<DEST_TYPE, SRC_TYPE>::Type(VAL))

	/// Conversion from C_UInt8 to DEST_TYPE
	#define VAL_CONV_FROM_U8(DEST_TYPE, VAL)  (typename VAL_CONV<DEST_TYPE, C_UInt8>::Type(VAL))
	/// Conversion from SRC_TYPE to C_UInt8
	#define VAL_CONV_TO_U8(SRC_TYPE, VAL)  (typename VAL_CONV<C_UInt8, SRC_TYPE>::Type(VAL))

	/// Conversion from C_UInt64 to DEST_TYPE
	#define VAL_CONV_FROM_U64(DEST_TYPE, VAL)  (typename VAL_CONV<DEST_TYPE, C_UInt64>::Type(VAL))
	/// Conversion from SRC_TYPE to C_UInt64
	#define VAL_CONV_TO_U64(SRC_TYPE, VAL)  (typename VAL_CONV<C_UInt64, SRC_TYPE>::Type(VAL))

	/// Conversion from C_Int64 to DEST_TYPE
	#define VAL_CONV_FROM_I64(DEST_TYPE, VAL)  (typename VAL_CONV<DEST_TYPE, C_Int64>::Type(VAL))
	/// Conversion from SRC_TYPE to C_Int64
	#define VAL_CONV_TO_I64(SRC_TYPE, VAL)  (typename VAL_CONV<C_Int64, SRC_TYPE>::Type(VAL))

	/// Conversion from C_Float64 to DEST_TYPE
	#define VAL_CONV_FROM_F64(DEST_TYPE, VAL)  (typename VAL_CONV<DEST_TYPE, C_Float64>::Type(VAL))
	/// Conversion from SRC_TYPE to C_Float64
	#define VAL_CONV_TO_F64(SRC_TYPE, VAL)  (typename VAL_CONV<C_Float64, SRC_TYPE>::Type(VAL))



	/// Conversion from SourceT to DestT
	/** \tparam  DestT    type of destination
	 *  \tparam  SourceT  type of source
	**/
	template<typename DestT, typename SourceT>
		static COREARRAY_INLINE DestT ValCvt(const SourceT &val)
	{
		return VAL_CONVERT(DestT, SourceT, val);
	}

	/// Conversion from SourceT to DestT
	/** \tparam  DestT    type of destination
	 *  \tparam  SourceT  type of source
	**/
	template<typename DestT, typename SourceT>
		static COREARRAY_INLINE DestT *ValCvtArray(DestT *p, const SourceT *s, ssize_t n)
	{
		return VAL_CONV<DestT, SourceT>::Cvt(p, s, n);
	}



	// =====================================================================
	// Template for Allocator
	// =====================================================================

	/// Define the size of buffer for ALLOC_FUNC
	const size_t COREARRAY_ALLOC_FUNC_BUFFER = 0x10000;

	/// Template functions for allocator
	template<typename ALLOC_TYPE, typename MEM_TYPE>
		struct COREARRAY_DLL_DEFAULT ALLOC_FUNC
	{
		/// read an array from CdAllocator
		static MEM_TYPE *Read(CdBaseIterator &I, MEM_TYPE *p, ssize_t n)
		{
			const ssize_t N = COREARRAY_ALLOC_FUNC_BUFFER / sizeof(ALLOC_TYPE);
			ALLOC_TYPE Buf[N];
			BYTE_LE<CdAllocator> ss(I.Allocator);
			I.Allocator->SetPosition(I.Ptr);
			I.Ptr += n * sizeof(ALLOC_TYPE);
			while (n > 0)
			{
				ssize_t Cnt = (n >= N) ? N : n;
				ss.R(Buf, Cnt);
				p = VAL_CONV<MEM_TYPE, ALLOC_TYPE>::Cvt(p, Buf, Cnt);
				n -= Cnt;
			}
			return p;
		}

		/// read an array from CdAllocator with selection
		static MEM_TYPE *ReadEx(CdBaseIterator &I, MEM_TYPE *p, ssize_t n, const C_BOOL Sel[])
		{
			const ssize_t N = COREARRAY_ALLOC_FUNC_BUFFER / sizeof(ALLOC_TYPE);
			ALLOC_TYPE Buf[N];
			BYTE_LE<CdAllocator> ss(I.Allocator);
			I.Allocator->SetPosition(I.Ptr);
			I.Ptr += n * sizeof(ALLOC_TYPE);
			while (n > 0)
			{
				ssize_t m = (n <= N) ? n : N;
				ss.R(Buf, m);
				p = VAL_CONV<MEM_TYPE, ALLOC_TYPE>::CvtSub(
					p, Buf, m, Sel);
				Sel += m;
				n -= m;
			}
			return p;
		}

		/// write an array to CdAllocator
		static const MEM_TYPE *Write(CdBaseIterator &I, const MEM_TYPE *p, ssize_t n)
		{
			const ssize_t N = COREARRAY_ALLOC_FUNC_BUFFER / sizeof(ALLOC_TYPE);
			ALLOC_TYPE Buf[N];
			I.Allocator->SetPosition(I.Ptr);
			I.Ptr += n * sizeof(ALLOC_TYPE);
			while (n > 0)
			{
				ssize_t Cnt = (n >= N) ? N : n;
				VAL_CONV<ALLOC_TYPE, MEM_TYPE>::Cvt(Buf, p, Cnt);
				p += Cnt;
				COREARRAY_ENDIAN_NT_TO_LE_ARRAY(Buf, Cnt);
				I.Allocator->WriteData(Buf, Cnt*sizeof(ALLOC_TYPE));
				n -= Cnt;
			}
			return p;
		}
	};

	/// Template functions for allocator
	template<typename TYPE>
		struct COREARRAY_DLL_DEFAULT ALLOC_FUNC<TYPE, TYPE>
	{
		/// read an array from CdAllocator
		static TYPE *Read(CdBaseIterator &I, TYPE *p, ssize_t n)
		{
			BYTE_LE<CdAllocator> ss(I.Allocator);
			I.Allocator->SetPosition(I.Ptr);
			I.Ptr += n * sizeof(TYPE);
			ss.R(p, n);
			return p + n;
		}

		/// read an array from CdAllocator with selection
		static TYPE *ReadEx(CdBaseIterator &I, TYPE *p, ssize_t n, const C_BOOL Sel[])
		{
			const ssize_t N = COREARRAY_ALLOC_FUNC_BUFFER / sizeof(TYPE);
			TYPE Buf[N];
			BYTE_LE<CdAllocator> ss(I.Allocator);
			I.Allocator->SetPosition(I.Ptr);
			I.Ptr += n * sizeof(TYPE);
			while (n > 0)
			{
				ssize_t m = (n <= N) ? n : N;
				ss.R(Buf, m);
				p = VAL_CONV<TYPE, TYPE>::CvtSub(p, Buf, m, Sel);
				Sel += m;
				n -= m;
			}
			return p;
		}

		/// write an array to CdAllocator
		static const TYPE *Write(CdBaseIterator &I, const TYPE *p, ssize_t n)
		{
			BYTE_LE<CdAllocator> ss(I.Allocator);
			I.Allocator->SetPosition(I.Ptr);
			I.Ptr += n * sizeof(TYPE);
			ss.W(p, n);
			return p + n;
		}
	};



	// =====================================================================
	// Exception for Allocator
	// =====================================================================

	/// Exception for CdAllocator
	class COREARRAY_DLL_EXPORT ErrAllocator: public ErrCoreArray
	{
	public:
		ErrAllocator(): ErrCoreArray()
			{ }
		ErrAllocator(const char *fmt, ...): ErrCoreArray()
			{ _COREARRAY_ERRMACRO_(fmt); }
		ErrAllocator(const std::string &msg): ErrCoreArray()
			{ fMessage = msg; }
	};

	class COREARRAY_DLL_EXPORT ErrAllocRead: public ErrAllocator
	{
	public:
		ErrAllocRead();
	};

	class COREARRAY_DLL_EXPORT ErrAllocWrite: public ErrAllocator
	{
	public:
		ErrAllocWrite();
	};
}

#endif /* _HEADER_COREARRAY_ALLOC_ */
