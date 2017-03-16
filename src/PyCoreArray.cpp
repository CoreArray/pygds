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


namespace pygds
{
	using namespace std;
	using namespace CoreArray;

	/// the number of preserved bytes for a pointer to a GDS object
	#define GDSFMT_NUM_BYTE_FOR_GDSOBJ    (4 + 16)


	/// a list of GDS files in the gdsfmt package
	COREARRAY_DLL_LOCAL PdGDSFile PYGDS_GDS_Files[PYGDS_MAX_NUM_GDS_FILES];

	/// get the index in 'PYGDS_GDS_Files' for NULL
	COREARRAY_DLL_LOCAL int GetEmptyFileIndex(bool throw_error=true)
	{
		for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
		{
			if (PYGDS_GDS_Files[i] == NULL)
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

	/// get the index in 'PYGDS_GDS_Files' for file
	COREARRAY_DLL_LOCAL int GetFileIndex(PdGDSFile file, bool throw_error=true)
	{
		for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
		{
			if (PYGDS_GDS_Files[i] == file)
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
			memset(PYGDS_GDS_Files, 0, sizeof(PYGDS_GDS_Files));
			PYGDS_GDSObj_List.reserve(1024);
		}

		/// finalization
		~CInitObject()
		{
			PYGDS_GDSObj_List.clear();
			PYGDS_GDSObj_Map.clear();

			for (int i=0; i < PYGDS_MAX_NUM_GDS_FILES; i++)
			{
				PdGDSFile file = PYGDS_GDS_Files[i];
				if (file != NULL)
				{
					try {
						PYGDS_GDS_Files[i] = NULL;
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

// ===========================================================================
// functions for file structure

COREARRAY_DLL_EXPORT PdGDSFile GDS_File_Create(const char *FileName)
{
	// to register CoreArray classes and objects
	RegisterClass();

	int gds_idx = GetEmptyFileIndex();
	PdGDSFile file = NULL;

	try {
		file = new CdGDSFile;
		file->SaveAsFile(FileName);
		PYGDS_GDS_Files[gds_idx] = file;
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
	// to register CoreArray classes and objects
	RegisterClass();

	int gds_idx = GetEmptyFileIndex();
	PdGDSFile file = NULL;

	try {
		file = new CdGDSFile;
		if (!ForkSupport)
			file->LoadFile(FileName, ReadOnly);
		else
			file->LoadFileFork(FileName, ReadOnly);

		PYGDS_GDS_Files[gds_idx] = file;
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
		PYGDS_GDS_Files[gds_idx] = NULL;

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
// functions for error

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

}
