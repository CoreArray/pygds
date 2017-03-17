// ===========================================================
//     _/_/_/   _/_/_/  _/_/_/_/    _/_/_/_/  _/_/_/   _/_/_/
//      _/    _/       _/             _/    _/    _/   _/   _/
//     _/    _/       _/_/_/_/       _/    _/    _/   _/_/_/
//    _/    _/       _/             _/    _/    _/   _/
// _/_/_/   _/_/_/  _/_/_/_/_/     _/     _/_/_/   _/_/
// ===========================================================
//
// PyGDS_CPP.h: C interface to pygds dynamic library + CPP classes
//
// Copyright (C) 2017    Xiuwen Zheng
//
// This file is part of CoreArray.
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


#ifndef _HEADER_PYGDS_CPP_
#define _HEADER_PYGDS_CPP_

#include <PyGDS.h>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <climits>


namespace CoreArray
{
	// ==================================================================
	// ==================================================================

	/// try block for CoreArray library
	#define CORE_TRY    \
		try {

	/// try block for CoreArray library
	#define COREARRAY_TRY    \
		int has_error = 0; \
		CORE_TRY


	/// catch block for CoreArray library
	#define CORE_CATCH(cmd)    \
		} \
		catch (std::exception &E) { \
			GDS_SetError(E.what()); cmd; \
		} \
		catch (const char *E) { \
			GDS_SetError(E); cmd; \
		} \
		catch (...) { \
			GDS_SetError("unknown error!"); cmd; \
		} \


	/// catch block for CoreArray library
	#define COREARRAY_CATCH    \
		} \
		catch (ErrGDSFmt &E) { \
			GDS_SetError(E.what()); has_error = 1; \
		} \
		catch (std::exception &E) { \
			GDS_SetError(E.what()); has_error = 2; \
		} \
		catch (const char *E) { \
			GDS_SetError(E); has_error = 2; \
		} \
		catch (...) { \
			GDS_SetError("unknown error!"); has_error = 2; \
		} \
		if (has_error) \
		{ \
			PyObject *type = (has_error==1) ? PyExc_ValueError : PyExc_IOError; \
			PyErr_SetString(type, GDS_GetError()); \
			return NULL; \
		}

	/// catch block for CoreArray library
	#define COREARRAY_CATCH_NONE    \
		COREARRAY_CATCH    \
		Py_RETURN_NONE;



	// ==================================================================
	// ==================================================================

	// [[ ********
	#ifndef COREARRAY_PYGDS_PACKAGE

	/// Error Macro
	#define _COREARRAY_ERRMACRO_(x) { \
		va_list args; va_start(args, x); \
		Init(x, args); \
		va_end(args); \
	}

	/// The root class of exception for CoreArray library
	class ErrCoreArray: public std::exception
	{
	public:
		ErrCoreArray() {}
		ErrCoreArray(const char *fmt, ...) { _COREARRAY_ERRMACRO_(fmt); }
		ErrCoreArray(const std::string &msg) { fMessage = msg; }

		virtual const char *what() const throw()
			{ return fMessage.c_str(); }
		virtual ~ErrCoreArray() throw() {};

	protected:
		std::string fMessage;

		void Init(const char *fmt, va_list arglist)
		{
			char buf[1024];
			vsnprintf(buf, sizeof(buf), fmt, arglist);
			fMessage = buf;
		}
	};


	/// automatic mutex object
	struct TdAutoMutex
	{
		PdThreadMutex mutex;

		TdAutoMutex(PdThreadMutex m)
			{ mutex = m; if (m) GDS_Parallel_LockMutex(m); }
		~TdAutoMutex()
			{ if (mutex) GDS_Parallel_UnlockMutex(mutex); }

		COREARRAY_INLINE void Reset(PdThreadMutex m)
		{
			if (m != mutex)
			{
				if (mutex) GDS_Parallel_UnlockMutex(mutex);
				mutex = m;
				if (m) GDS_Parallel_LockMutex(m);
			}
		}
	};


	/// the class of read array
	class CArrayRead
	{
	public:
		CArrayRead(PdAbstractArray Obj, int Margin, C_SVType SVType,
			const C_BOOL *const Selection[], bool buf_if_need=true)
		{
			_Obj = GDS_ArrayRead_Init(Obj, Margin, SVType, Selection,
				buf_if_need);
			if (!_Obj)
				throw ErrCoreArray("Error 'initialize CArrayRead'.");
		}
		~CArrayRead()
		{
			GDS_ArrayRead_Free(_Obj);
		}

		/// read data
		void Read(void *Buffer)
		{
			GDS_ArrayRead_Read(_Obj, Buffer);
		}

		/// return true, if it is of the end
		bool Eof()
		{
			return GDS_ArrayRead_Eof(_Obj);
		}

	protected:
		PdArrayRead _Obj;
	};

	#else  // COREARRAY_PYGDS_PACKAGE

	/// error exception for the pygds package
	class COREARRAY_DLL_EXPORT ErrGDSFmt: public ErrCoreArray
	{
	public:
		ErrGDSFmt() {}
		ErrGDSFmt(const char *fmt, ...) { _COREARRAY_ERRMACRO_(fmt); }
		ErrGDSFmt(const std::string &msg) { fMessage = msg; }
	};

	#endif  // COREARRAY_PYGDS_PACKAGE
	// ]] ********

}

#endif /* _HEADER_PYGDS_CPP_ */
