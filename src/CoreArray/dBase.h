// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// dBase.h: Fundamental classes for CoreArray library
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
 *	\file     dBase.h
 *	\author   Xiuwen Zheng [zhengx@u.washington.edu]
 *	\version  1.0
 *	\date     2007 - 2017
 *	\brief    Basic classes for CoreArray library
 *	\details
**/


#ifndef _HEADER_COREARRAY_BASE_
#define _HEADER_COREARRAY_BASE_

#include "dEndian.h"

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <limits>


namespace CoreArray
{
	/// A macro of CoreArray library version
	/** You can use COREARRAY_VERSION in $if expressions to test the runtime
	 *  library version level independently of the compiler version level.
	 *  For example:  #if (COREARRAY_VERSION >= 0x0100) ... #endif
	**/
	#define COREARRAY_VERSION        0x0100

	/// A string of CoreArray library version
	#define COREARRAY_VERSION_STR    "v1.00 (release)"

	/// A macro of CoreArray class version
	#define COREARRAY_CLASS_VERSION  0x0100


	// =====================================================================
	// CdRef
	// =====================================================================

	class CdRef;

	namespace _INTERNAL
	{
		/// increase the reference of Obj immediately, without any checking
		/** return Obj.fReference after increment
		 *  \param Obj    a CdRef object
		**/
		ssize_t DirectAddRef(CdRef &Obj);

		/// decrease the reference of Obj immediately, without any checking
		/** return Obj.fReference after decrease
		 *  \param Obj    a CdRef object
		**/
		ssize_t DirectDecRef(CdRef &Obj);
	}

	/// The class with a reference counter
	/** A reference counter is added to the class. **/
	class COREARRAY_DLL_DEFAULT CdRef
	{
	public:
		/// increase the reference of Obj immediately, without any checking
		friend ssize_t _INTERNAL::DirectAddRef(CdRef &Obj);
		/// decrease the reference of Obj immediately, without any checking
		friend ssize_t _INTERNAL::DirectDecRef(CdRef &Obj);

		/// constructor, setting Reference = 0
		CdRef();
		/// destructor
		virtual ~CdRef();

		/// increase the reference count, and return the count after increment
		/** indicating this object is being used.  **/
		ssize_t AddRef();
		/// decrease the reference count, and return the count after decrease
		/** if the count after decrease <= 0, free the object.  **/
		ssize_t Release();

		/// return the count
		COREARRAY_INLINE ssize_t Reference() const { return fReference; }

	private:
		ssize_t fReference;
	};



	// =====================================================================
	// CdAbstract
	// =====================================================================

	/// The abstract root class for all CoreArray classes
	class COREARRAY_DLL_DEFAULT CdAbstract {};

	/// The abstract root class for managing items
	class COREARRAY_DLL_DEFAULT CdAbstractManager: public CdAbstract {};

	/// The abstract root class for individual items used in CdAbstractManager
	class COREARRAY_DLL_DEFAULT CdAbstractItem: public CdAbstract {};



	// =====================================================================
	// CdObject
	// =====================================================================

	/// Class version
	/** TdVersion x:
	 *  (x) >> 8      is major version
	 *  (x) & 0xFF    is minor version
	**/
	typedef C_UInt16 TdVersion;

	// call load function
	#define COREARRAY_READER_CALL_SILENT(cmd)	\
		try {	\
			cmd;	\
		} catch (exception &E) {	\
			Reader.Log().Add(E.what());	\
		} catch (const char *E) {	\
			Reader.Log().Add(E);	\
		}


	// predefined classes
	class CdObject;
	class CdReader;
	class CdWriter;

	namespace _INTERNAL
	{
		/// Access CdObject::LoadStruct
		void CdObject_LoadStruct(CdObject &Obj, CdReader &Reader,
			TdVersion Version);
		/// Access CdObject::SaveStruct
		void CdObject_SaveStruct(CdObject &Obj, CdWriter &Writer,
			bool IncludeName);
	}



	/// The root class for CoreArray object
	/** CdObject contains a sub-system of class serialization, which allows to
	 *  load and save its data to a stream.
	**/
	class COREARRAY_DLL_DEFAULT CdObject: public CdAbstract
	{
	public:
		/// access CdObject::LoadStruct
		friend void _INTERNAL::CdObject_LoadStruct(CdObject &Obj,
			CdReader &Reader, TdVersion Version);
		/// access CdObject::SaveStruct
		friend void _INTERNAL::CdObject_SaveStruct(CdObject &Obj,
			CdWriter &Writer, bool IncludeName);

		/// constructor
		CdObject(): CdAbstract() {}
		/// destructor
		virtual ~CdObject() {}

		/// return a string specifying the class name in stream
		virtual const char *dName();
		/// return a string specifying the class name
		virtual const char *dTraitName();

		/// return version of the class
		virtual TdVersion dVersion();
		/// return version of the class, saved in stream
		virtual TdVersion SaveVersion();

	protected:
		// load functions, calling ::Loading
		/** \param Reader    the reader for serialization
		 *  \param Version   the version of object
		**/
		virtual void LoadStruct(CdReader &Reader, TdVersion Version);

		/// method of serializing the object
		/** \param Reader    the reader for serialization
		 *  \param Version   the version of object
		**/
		virtual void Loading(CdReader &Reader, TdVersion Version);

		// Save functions
		/// prepare Writer, and call ::Saving
		virtual void SaveStruct(CdWriter &Writer, bool IncludeName);
		virtual void Saving(CdWriter &Writer);

	#ifdef COREARRAY_CODE_USING_LOG
		virtual string LogValue();
	#endif
	};



	// =====================================================================
	// TdObjRef
	// =====================================================================

	/// Combination of CdObject and CdRef
	/** Allowing CoreArray object to have a reference counter. **/
	class COREARRAY_DLL_DEFAULT CdObjRef: public CdRef, public CdObject
	{
	public:
		CdObjRef(): CdRef(), CdObject() { }
	};

	/// A structure automatically calling reference counter
	/** This structure template provides a limited garbage collection facility
	 *  for a CdRef pointer, by allowing CdRef object to automatically call
	 *  CdRef::Release when the TdAutoRef object is itself destroyed.
	 *  \tparam TYPE    CdRef or a class inherited from CdRef
	**/
	template<class TYPE> struct COREARRAY_DLL_DEFAULT TdAutoRef
	{
	public:
		/// constructor
		COREARRAY_INLINE TdAutoRef()
		{
			fObj = NULL;
		}
		/// constructor
		COREARRAY_INLINE TdAutoRef(TYPE *vObj)
		{
			fObj = vObj;
			if (vObj) vObj->AddRef();
		}
		/// destructor
		COREARRAY_INLINE ~TdAutoRef()
		{
			if (fObj)
				fObj->Release();
		}

		/// get the object pointer
		COREARRAY_INLINE TYPE *get()
		{
			return fObj;
		}
		/// assignment
		COREARRAY_INLINE TYPE* operator=(TYPE *_Right)
		{
			if (_Right != fObj)
			{
				if (fObj) fObj->Release();
				fObj = _Right;
				if (_Right) _Right->AddRef();
			}
			return _Right;
		}
		COREARRAY_INLINE TYPE &operator*() const
		{
			return *fObj;
		}
		COREARRAY_INLINE TYPE *operator->() const
		{
			return fObj;
		}

	private:
		TYPE *fObj;
	};



	// =====================================================================
	// CdObjMsg
	// =====================================================================

	class CdObjMsg;

	/// A structure of notification for CdObject
	template<class TSender=CdObject> struct COREARRAY_DLL_DEFAULT TdOnNotify
	{
	public:
		/// class method type
		typedef void (CdObjMsg::*TEvent)(TSender *);

		/// constructor
		COREARRAY_INLINE TdOnNotify()
		{
			Obj = NULL; Event = NULL;
		}

		/// whether it is valid
		COREARRAY_INLINE operator bool() const
		{
			return (Obj!=NULL) && (Event!=NULL);
		}

		/// clear the existing event
		COREARRAY_INLINE void Clear()
		{
			Obj = NULL; Event = NULL;
		}

		/// set an event
		template<class TYPE> COREARRAY_INLINE void Set(TYPE *vObj,
			void (TYPE::*vEvent)(TSender*))
		{
			Obj = vObj;
			Event = (TEvent)vEvent;
		}

		/// call
		COREARRAY_INLINE void Notify(TSender *Sender)
		{
			if ((Obj!=NULL) && (Event!=NULL))
				(Obj->*Event)(Sender);
		}

	private:
		CdObjMsg *Obj;
		TEvent Event;
	};

	/// A structure of broadcast for CdObjMsg
	struct TdOnBroadcast
	{
	public:
		/// constructor
		TdOnBroadcast();

		COREARRAY_INLINE void Clear() { Obj = NULL; Event = NULL; }
		template<class T> COREARRAY_INLINE TdOnBroadcast &Set(T *const vObj,
				void (T::*vEvent)(CdObjMsg*, C_Int32, void*))
			{ Obj = (CdObjMsg*)vObj; Event = (TdOnDo)vEvent; return *this; }

		void Notify(CdObjMsg *Sender, C_Int32 MsgCode, void *Param);

		COREARRAY_INLINE operator bool() const { return (Event!=NULL); }
		COREARRAY_INLINE bool operator== (const TdOnBroadcast &v) const
			{ return ((Obj==v.Obj) && (Event==v.Event)); }
		COREARRAY_INLINE bool operator!= (const TdOnBroadcast &v) const
			{ return ((Obj!=v.Obj) || (Event!=v.Event)); }
	private:
		typedef void (CdObjMsg::*TdOnDo)(CdObjMsg *, C_Int32, void *);
		CdObjMsg *Obj;
		TdOnDo Event;
	};


	/// The CoreArray class with broadcast ability
	/** The class allow users to add multiple broadcast event handlers. **/
	class COREARRAY_DLL_DEFAULT CdObjMsg: public CdObjRef
	{
	public:
		/// constructor
		CdObjMsg();
		/// destructor
		virtual ~CdObjMsg();

		/// add a message receiver
		void AddMsg(const TdOnBroadcast &MsgObj);
		/// add a message receiver, template
		template<class T> COREARRAY_INLINE void AddMsgEx(T *const vObj,
			void (T::*vEvent)(CdObjMsg*, C_Int32, void*))
		{
			TdOnBroadcast Do;
			Do.Set<T>(vObj, vEvent);
			AddMsg(Do);
		}

		/// remove a message receiver
		void RemoveMsg(const TdOnBroadcast &MsgObj);
		/// remove a message receiver, template
		template<class T> COREARRAY_INLINE void RemoveMsgEx(T *const vObj,
			void (T::*vEvent)(CdObjMsg*, C_Int32, void*))
		{
			TdOnBroadcast Do;
			Do.Set<T>(vObj, vEvent);
			RemoveMsg(Do);
		}

		/// begin to block all messages
		void BeginMsg();
		/// finish the section of message block
		/** If any message has been blocked after calling ::BeginMsg(), then
         *  a NULL message (calling ::Notify(0, NULL)) will be sent out.
        **/
		void EndMsg();

		/// broadcast a message
		void Notify(C_Int32 MsgCode, void *Param = NULL);

		/// broadcast a message of C_Int32 value
		COREARRAY_INLINE void Notify32(C_Int32 MsgCode, const C_Int32 Value)
			{ Notify(MsgCode, (void*)&Value); }
		/// broadcast a message of C_Int64 value
		COREARRAY_INLINE void Notify64(C_Int32 MsgCode, const C_Int64 Value)
			{ Notify(MsgCode, (void*)&Value); }
		/// broadcast a message of an array of C_Int32 value
		COREARRAY_INLINE void Notify32(C_Int32 MsgCode, C_Int32 const *Param)
			{ Notify(MsgCode, (void*)Param); }
		/// broadcast a message of an array of C_Int64 value
		COREARRAY_INLINE void Notify64(C_Int32 MsgCode, C_Int64 const *Param)
			{ Notify(MsgCode, (void*)Param); }

    	/// return a vector of message receivers
		COREARRAY_INLINE const std::vector<TdOnBroadcast> &MsgList() const
			{ return fMsgList; }

	protected:
		/// determine messages to be sent (if return true), or blocked
		virtual bool MsgFilter(C_Int32 MsgCode, void *Param);

	private:
		std::vector<TdOnBroadcast> fMsgList;
		int vMsgRef;
		bool vMsgFlag;
	};


	/// An object of message, automatically calling BeginMsg and EndMsg 
	struct COREARRAY_DLL_DEFAULT TdAutoObjMsg
	{
	public:
		TdAutoObjMsg(CdObjMsg *vObj)
		{
			Obj = vObj;
			if (Obj) Obj->BeginMsg();
		}
		~TdAutoObjMsg()
		{
			if (Obj) Obj->EndMsg();
			Obj = NULL;
		}
	private:
		CdObjMsg *Obj;
	};


	/// Log record
	class COREARRAY_DLL_DEFAULT CdLogRecord: public CdObjRef
	{
	public:
		static const int logCustom  =  -1;  ///< user-customized
		static const int logInfo    =   0;  ///< information
		static const int logError   =   1;  ///< error
		static const int logWarn    =   2;  ///< warning
		static const int logHint    =   3;  ///< hint

		/// constructor
		CdLogRecord();
		
		/// record item
		struct TdItem
		{
			UTF8String Msg;  ///< the message
			C_Int32 Type;      ///< the type of message
			TdItem() { Type = logCustom; }
		};

		/// add a message
		void Add(const char *const str, C_Int32 vType=logError);
		/// add a message
		void Add(std::string &str, C_Int32 vType=logError);
		/// add a message
		void Add(C_Int32 vType, const char *fmt, ...);

		/// return a vector of TdItem (record item)
		COREARRAY_INLINE std::vector<TdItem> &List() { return fList; }

	protected:
		std::vector<TdItem> fList;

		virtual void Loading(CdReader &Reader, TdVersion Version);
		virtual void Saving(CdWriter &Writer);
	};



	// =====================================================================
	// Memory Object
	// =====================================================================

	/// The memory object
	/** Provides ::R8b, ::W8b, ::ReadData and ::WriteData for
	 *  BIT_LE<> and BYTE_LE<>.
	**/
	class COREARRAY_DLL_DEFAULT CdMemory
	{
	public:
		/// constructor
		CdMemory(void *Ptr);

		/// read block of data
		void ReadData(void *Buffer, ssize_t Count);
		/// read a 8-bit integer with native endianness
		C_UInt8 R8b();

		/// write block of data
		void WriteData(const void *Buffer, ssize_t Count);
		/// write a 8-bit integer with native endianness
		void W8b(C_UInt8 val);

		/// return the current position
		SIZE64 Position();
		/// reset the current position
		void SetPosition(const SIZE64 pos);

		COREARRAY_INLINE void *Base() { return _Base; }
		COREARRAY_INLINE void *CurPtr() { return _CurPtr; }

	protected:
		void *_Base, *_CurPtr;
	};



	// =====================================================================
	// Stream Object
	// =====================================================================

	class COREARRAY_DLL_DEFAULT CdBufStream;

	/// Define the size of stream buffer for saving, loading and copying (64K) (allocated in stack)
	const size_t COREARRAY_STREAM_BUFFER = 0x10000;
	/// Define the large size of stream buffer for copying (512K) (allocated from the heap)
	const size_t COREARRAY_LARGE_STREAM_BUFFER = 0x80000;

	/// The root class of CoreArray stream object
	/** CdStream provides basic interface for stream. **/
	class COREARRAY_DLL_DEFAULT CdStream: public CdRef
	{
	public:
		/// constructor
		CdStream();
		/// destructor
		virtual ~CdStream();

		/// read block of data, and return number of read in bytes
		virtual ssize_t Read(void *Buffer, ssize_t Count) = 0;
		/// write block of data, and return number of write in bytes
		virtual ssize_t Write(const void *Buffer, ssize_t Count) = 0;
		/// set position in input sequence
		virtual SIZE64 Seek(SIZE64 Offset, TdSysSeekOrg Origin) = 0;
		/// get the total size of stream
		virtual SIZE64 GetSize();
		/// set or terminate the size of stream
		virtual void SetSize(SIZE64 NewSize) = 0;

		/// return the current position
		SIZE64 Position();
		/// reset the current position
		void SetPosition(const SIZE64 pos);

		/// read block of data, or throw an exception if fail
		void ReadData(void *Buffer, ssize_t Count);
		/// read a 8-bit integer with native endianness
		C_UInt8 R8b();
		/// read a 16-bit integer with native endianness
		C_UInt16 R16b();
		/// read a 32-bit integer with native endianness
		C_UInt32 R32b();
		/// read a 64-bit integer with native endianness
		C_UInt64 R64b();

		/// write block of data, or throw an exception if fail
		void WriteData(const void *Buffer, ssize_t Count);
		/// write a 8-bit integer with native endianness
		void W8b(C_UInt8 val);
		/// write a 16-bit integer with native endianness
		void W16b(C_UInt16 val);
		/// write a 32-bit integer with native endianness
		void W32b(C_UInt32 val);
		/// write a 64-bit integer with native endianness
		void W64b(C_UInt64 val);

		/// copy from a CdStream object
		/** \param Source  a stream object
		 *  \param Pos     the starting position
		 *  \param Count   the number of bytes, -1 for all data starting from Pos
		**/
		virtual void CopyFrom(CdStream &Source, SIZE64 Pos, SIZE64 Count);

		/// copy from a CdBufStream object
		void CopyFromBuf(CdBufStream &Source, SIZE64 Pos, SIZE64 Count);

	private:
		CdStream& operator= (const CdStream& m);
		CdStream& operator= (CdStream& m);
	};



	// =====================================================================
	// Stream Buffer Object
	// =====================================================================

	/// Default buffer size in TBufdStream, 4K
	const ssize_t STREAM_BUFFER_SIZE       = 4*1024;
	/// Default small size of buffer in TBufdStream, 1K
	const ssize_t STREAM_BUFFER_SMALL_SIZE = 1024;
	/// Default large size of buffer in TBufdStream, 128K
	const ssize_t STREAM_BUFFER_LARGE_SIZE = 128*1024;

	/// Aligned bytes of stream buffer
	const size_t BufStreamAlign = 4;  // 2^4 = 16 bytes aligned


	// CdBufStream

	class COREARRAY_DLL_DEFAULT CdBufStream;

	/// The root class of stream pipe
	class COREARRAY_DLL_DEFAULT CdStreamPipe: public CdAbstractItem
	{
	public:
		friend class CdBufStream;

		CdStreamPipe(): CdAbstractItem() {}
		virtual ~CdStreamPipe() {}

	protected:
		virtual CdStream *InitPipe(CdBufStream *BufStream) = 0;
		virtual CdStream *FreePipe() = 0;
	};


	/// The class adds a buffer to a stream
	class COREARRAY_DLL_DEFAULT CdBufStream: public CdRef
	{
	public:
		friend class CdObject;

		/// constructor
		/** \param vStream    a stream object (it can be NULL)
		 *  \param vBufSize   buffer size
		**/
		CdBufStream(CdStream *vStream, ssize_t vBufSize=STREAM_BUFFER_SIZE);
		/// destructor
		virtual ~CdBufStream();

		void FlushBuffer();
		void FlushWrite();

		/// Copy from a CdStream object
		void CopyFrom(CdStream &Source, SIZE64 Pos, SIZE64 Count);
		/// Copy from a CdBufStream object
		void CopyFromBuf(CdBufStream &Source, SIZE64 Pos, SIZE64 Count);
		/// Truncate the stream
		void Truncate();

		/// Read block of data, or throw an exception if fail
		void ReadData(void *Buffer, ssize_t Count);
		/// Read a 8-bit integer with native endianness
		C_UInt8 R8b();
		/// Read a 16-bit integer with native endianness
		C_UInt16 R16b();
		/// Read a 32-bit integer with native endianness
		C_UInt32 R32b();
		/// Read a 64-bit integer with native endianness
		C_UInt64 R64b();

		/// Write block of data, or throw an exception if fail
		void WriteData(const void *Buffer, ssize_t Count);
		/// Write a 8-bit integer with native endianness
		void W8b(C_UInt8 val);
		/// Write a 16-bit integer with native endianness
		void W16b(C_UInt16 val);
		/// Write a 32-bit integer with native endianness
		void W32b(C_UInt32 val);
		/// Write a 64-bit integer with native endianness
		void W64b(C_UInt64 val);

		void ClearPipe();
		void PushPipe(CdStreamPipe* APipe);
		void PopPipe();

		COREARRAY_INLINE SIZE64 Position() { return _Position; }
		COREARRAY_INLINE void SetPosition(const SIZE64 pos) { _Position = pos; }

		COREARRAY_INLINE CdStream *Stream() const { return _Stream; }
		void SetStream(CdStream *Value);
		COREARRAY_INLINE CdStream *BaseStream() const { return _BaseStream; }

		COREARRAY_INLINE ssize_t BufSize() const { return _BufSize; }
		void SetBufSize(const ssize_t NewBufSize);
		virtual SIZE64 GetSize();
		virtual void SetSize(SIZE64 Value);

		TdOnNotify<CdBufStream> OnFlush;

	protected:
		CdStream *_Stream, *_BaseStream;
		ssize_t _BufSize;
		SIZE64 _Position;
		SIZE64 _BufStart, _BufEnd;

		C_UInt8 *_Buffer;
		bool _BufWriteFlag;
		std::vector<CdStreamPipe*> _PipeItems;
	};


	/// Template for stream pipe
	template<typename CLASS>
		class COREARRAY_DLL_DEFAULT CdStreamPipe2: public CdStreamPipe
	{
	protected:
		virtual CdStream *InitPipe(CdBufStream *BufStream)
		{
			fStream = BufStream->Stream();
			fPStream = new CLASS(*BufStream->Stream());
			return fPStream;
		}
		virtual CdStream *FreePipe()
		{
			if (fPStream) fPStream->Release();
			return fStream;
		}

	private:
		CdStream *fStream;
		CLASS *fPStream;
	};



	// =====================================================================
	// Indexing object for random access
	// =====================================================================

	class COREARRAY_DLL_EXPORT CdStreamIndex
	{
	public:
		static const int IndexSize = 32768;

		/// constructor
		CdStreamIndex();

		void Reset(C_Int64 count);
		void Initialize();

		/// current index plus one with stream_pos
		COREARRAY_FORCEINLINE void Forward(SIZE64 stream_pos)
		{
			if (!fHasInit) _Init();
			fCurIndex ++;
			if (fCurIndex == fNextHit) _Hit(stream_pos);
		}

		void Set(C_Int64 index, C_Int64 &close_index, SIZE64 &stream_pos);

		COREARRAY_INLINE C_Int64 Count() const { return fCount; } 

	protected:
		/// 
		struct TPair
		{
			C_Int64 Index;
			SIZE64 StreamPos;
			TPair(C_Int64 i, SIZE64 p) { Index = i; StreamPos = p; }
		};

		/// 
		vector<TPair> fList;
		/// the total count
		C_Int64 fCount;
		///
		double fScale, fInvScale;

		bool fHasInit;      ///< whether it has been initialized 
		C_Int64 fCurIndex;  ///<
		C_Int64 fNextHit;   ///<
		size_t fNextHitIndex;

	private:
		void _Init();
		void _Hit(SIZE64 stream_pos);
	};



	// =====================================================================
	// CoreArray Exception
	// =====================================================================

	/// Check memory allocation
	#define COREARRAY_ALLOCCHECK(x)	{ if (!x) throw bad_alloc(); }

	/// Exception for CoreArray object
	class COREARRAY_DLL_EXPORT ErrObject: public ErrCoreArray
	{
	public:
		ErrObject(): ErrCoreArray()
			{ }
		ErrObject(const char *fmt, ...): ErrCoreArray()
			{ _COREARRAY_ERRMACRO_(fmt); }
		ErrObject(const std::string &msg): ErrCoreArray()
			{ fMessage = msg; }
	};


	/// Exception for CoreArray stream
	class COREARRAY_DLL_EXPORT ErrStream: public ErrCoreArray
	{
	public:
		ErrStream(): ErrCoreArray()
			{ }
		ErrStream(const char *fmt, ...): ErrCoreArray()
			{ _COREARRAY_ERRMACRO_(fmt); }
		ErrStream(const std::string &msg): ErrCoreArray()
			{ fMessage = msg; }
	};


	/// Exception for CoreArray buffer object
	class COREARRAY_DLL_EXPORT ErrBufStream: public ErrCoreArray
	{
	public:
        ErrBufStream(): ErrCoreArray()
        	{ }
		ErrBufStream(const char *fmt, ...): ErrCoreArray()
			{ _COREARRAY_ERRMACRO_(fmt); }
		ErrBufStream(const std::string &msg): ErrCoreArray()
			{ fMessage = msg; }
	};
}

#endif /* _HEADER_COREARRAY_BASE_ */
