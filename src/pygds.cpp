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
	extern PdGDSFile PKG_GDS_Files[];
	extern vector<PdGDSObj> PYGDS_GDSObj_List;
	extern map<PdGDSObj, int> PYGDS_GDSObj_Map;
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

/// file size to a string
static string fmt_size(double b)
{
	static const double TB = 1024.0*1024*1024*1024;
	static const double GB = 1024.0*1024*1024;
	static const double MB = 1024.0*1024;
	static const double KB = 1024.0;

	char s[256];
	if (b >= TB)
		FmtText(s, sizeof(s), "%.1fT", b/TB);
	else if (b >= GB)
		FmtText(s, sizeof(s), "%.1fG", b/GB);
	else if (b >= MB)
		FmtText(s, sizeof(s), "%.1fM", b/MB);
	else if (b >= KB)
		FmtText(s, sizeof(s), "%.1fK", b/KB);
	else
		FmtText(s, sizeof(s), "%gB", b);

	return string(s);
}


/// convert "(CdGDSObj*)  -->  PyObject*"
static void set_obj(CdGDSObj *Obj, int &outidx, Py_ssize_t &outptr)
{
	static const char *ERR_OBJLIST = "Internal error in GDS node list.";

	if (!Obj)
		throw ErrGDSFmt("Invalid GDS object [NULL].");

	map<PdGDSObj, int>::iterator it = PYGDS_GDSObj_Map.find(Obj);
	if (it != PYGDS_GDSObj_Map.end())
	{
		outidx = it->second;
		if ((outidx < 0) || (outidx >= (int)PYGDS_GDSObj_List.size()))
			throw ErrGDSFmt(ERR_OBJLIST);
		if (PYGDS_GDSObj_List[outidx] != Obj)
			throw ErrGDSFmt(ERR_OBJLIST);
	} else {
		vector<PdGDSObj>::iterator it =
			find(PYGDS_GDSObj_List.begin(), PYGDS_GDSObj_List.end(),
			(PdGDSObj)NULL);
		if (it != PYGDS_GDSObj_List.end())
		{
			outidx = it - PYGDS_GDSObj_List.begin();
			*it = Obj;
		} else {
			outidx = PYGDS_GDSObj_List.size();
			PYGDS_GDSObj_List.push_back(Obj);
		}
		PYGDS_GDSObj_Map[Obj] = outidx;
	}

	outptr = (Py_ssize_t)Obj;
}

static CdGDSObj* get_obj(int idx, Py_ssize_t ptr_int)
{
	static const char *ERR_GDS_OBJ  =
		"Invalid GDS node object!";
	static const char *ERR_GDS_OBJ2 =
		"Invalid GDS node object (it was closed or deleted).";

	CdGDSObj *ptr = (CdGDSObj *)ptr_int;
	// check
	if ((idx < 0) || (idx >= (int)PYGDS_GDSObj_List.size()))
		throw ErrGDSFmt(ERR_GDS_OBJ);
	if (ptr == NULL)
		throw ErrGDSFmt(ERR_GDS_OBJ);
	if (PYGDS_GDSObj_List[idx] != ptr)
		throw ErrGDSFmt(ERR_GDS_OBJ2);

	return ptr;
}



// ----------------------------------------------------------------------------
// File Operations
// ----------------------------------------------------------------------------

/// Create a GDS file
COREARRAY_DLL_EXPORT PyObject* gdsCreateGDS(PyObject *self, PyObject *args)
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
				if (PKG_GDS_Files[i])
				{
					if (PKG_GDS_Files[i]->FileName() == FName)
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

	return PyInt_FromLong(file_id);
}


/// Open an existing GDS file
COREARRAY_DLL_EXPORT PyObject* gdsOpenGDS(PyObject *self, PyObject *args)
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
				if (PKG_GDS_Files[i])
				{
					if (PKG_GDS_Files[i]->FileName() == FName)
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

	return PyInt_FromLong(file_id);
}


/// Close the GDS file
COREARRAY_DLL_EXPORT PyObject* gdsCloseGDS(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;

	COREARRAY_TRY
		if (file_id >= 0)
			GDS_File_Close(GDS_ID2File(file_id));
	COREARRAY_CATCH_NONE
}


/// Synchronize the GDS file
COREARRAY_DLL_EXPORT PyObject* gdsSyncGDS(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;

	COREARRAY_TRY
		GDS_ID2File(file_id)->SyncFile();
	COREARRAY_CATCH_NONE
}


/// Get the file size and check the file handler
COREARRAY_DLL_EXPORT PyObject* gdsFileSize(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;

	double sz;
	COREARRAY_TRY
		sz = GDS_ID2File(file_id)->GetFileSize();
	COREARRAY_CATCH
	return Py_BuildValue("d", sz);
}


/// Clean up fragments of a GDS file
COREARRAY_DLL_EXPORT PyObject* gdsTidyUp(PyObject *self, PyObject *args)
{
	const char *fn;
	int verbose;
	if (!PyArg_ParseTuple(args, "s" BSTR, &fn, &verbose))
		return NULL;

	COREARRAY_TRY

		CdGDSFile file(fn, CdGDSFile::dmOpenReadWrite);
		C_Int64 old_s = file.GetFileSize();
		if (verbose)
		{
			printf("Clean up the fragments of GDS file:\n");
			printf("    open the file '%s' (%s)\n", fn, fmt_size(old_s).c_str());
			printf("    # of fragments: %d\n", file.GetNumOfFragment());
			printf("    save to '%s.tmp'\n", fn);
			fflush(stdout);
		}
		file.TidyUp(false);
		if (verbose)
		{
			C_Int64 new_s = file.GetFileSize();
			printf("    rename '%s.tmp' (%s, reduced: %s)\n", fn,
				fmt_size(new_s).c_str(), fmt_size(old_s-new_s).c_str());
			printf("    # of fragments: %d\n", file.GetNumOfFragment());
			fflush(stdout);
		}

	COREARRAY_CATCH_NONE
}


/// Clean up fragments of a GDS file
COREARRAY_DLL_EXPORT PyObject* gdsRoot(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;

	int idx;
	Py_ssize_t ptr;
	COREARRAY_TRY
		set_obj(&GDS_ID2File(file_id)->Root(), idx, ptr);
	COREARRAY_CATCH

	return Py_BuildValue("in", idx, ptr);
}


/// Clean up fragments of a GDS file
COREARRAY_DLL_EXPORT PyObject* gdsIndex(PyObject *self, PyObject *args)
{
	int file_id;
	const char *path;
	int silent;
	if (!PyArg_ParseTuple(args, "is" BSTR, &file_id, &path, &silent))
		return NULL;

	int idx;
	Py_ssize_t ptr;
	COREARRAY_TRY
		CdGDSObj *Obj = GDS_ID2File(file_id)->Root().PathEx(UTF16Text(path));
		if (!Obj && !silent)
			throw ErrGDSObj("No such GDS node \"%s\"!", path);
		if (Obj)
		{
			set_obj(Obj, idx, ptr);
		} else {
			idx = -1; ptr = 0;
		}
	COREARRAY_CATCH

	return Py_BuildValue("in", idx, ptr);
}



// ----------------------------------------------------------------------------
// File Structure Operations
// ----------------------------------------------------------------------------

/// Enumerate the names of its child nodes
COREARRAY_DLL_EXPORT PyObject* gdsnListName(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	int inc_hidden;
	if (!PyArg_ParseTuple(args, "in" BSTR, &nidx, &ptr_int, &inc_hidden))
		return NULL;

	COREARRAY_TRY

		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		CdGDSAbsFolder *Dir = dynamic_cast<CdGDSAbsFolder*>(Obj);
		if (Dir)
		{
			vector<string> List;
			for (int i=0; i < Dir->NodeCount(); i++)
			{
				CdGDSObj *Obj = Dir->ObjItemEx(i);
				if (Obj)
				{
					if (inc_hidden)
					{
						List.push_back(RawText(Obj->Name()));
					} else {
						if (!Obj->GetHidden() &&
							!Obj->Attribute().HasName(ASC16("R.invisible")))
						{
							List.push_back(RawText(Obj->Name()));
						}
					}
				}
			}

			PyObject *rv_ans = PyList_New(List.size());
			for (size_t i=0; i < List.size(); i++)
			{
				PyObject *x = PYSTR_SET2(List[i].c_str(), List[i].size());
				PyList_SetItem(rv_ans, i, x);
			}

			return rv_ans;

		} else {
			throw ErrGDSObj("It is not a folder.");
		}

	COREARRAY_CATCH_NONE
}


/// Get the GDS node with a given path
COREARRAY_DLL_EXPORT PyObject* gdsnIndex(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	const char *path;
	int silent;
	if (!PyArg_ParseTuple(args, "ins" BSTR, &nidx, &ptr_int, &path, &silent))
		return NULL;

	int idx;
	Py_ssize_t ptr;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		CdGDSAbsFolder *Dir = dynamic_cast<CdGDSAbsFolder*>(Obj);
		if (Dir)
		{
			Obj = Dir->PathEx(UTF16Text(path));
			if (!Obj && !silent)
				throw ErrGDSObj("No such GDS node \"%s\"!", path);
			if (Obj)
			{
				set_obj(Obj, idx, ptr);
			} else {
				idx = -1; ptr = 0;
			}
		} else {
			throw ErrGDSObj("It is not a folder.");
		}
	COREARRAY_CATCH

	return Py_BuildValue("in", idx, ptr);
}


/// Get the name of a GDS node
COREARRAY_DLL_EXPORT PyObject* gdsnName(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	int full;
	if (!PyArg_ParseTuple(args, "in" BSTR, &nidx, &ptr_int, &full))
		return NULL;

	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		string nm;
		if (full)
			nm = RawText(Obj->FullName());
		else
			nm = RawText(Obj->Name());
		return Py_BuildValue("s", nm.c_str());
	COREARRAY_CATCH_NONE
}


/// Get the name of a GDS node
COREARRAY_DLL_EXPORT PyObject* gdsnRename(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	const char *newname;
	if (!PyArg_ParseTuple(args, "ins", &nidx, &ptr_int, &newname))
		return NULL;

	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		Obj->SetName(UTF16Text(newname));
	COREARRAY_CATCH_NONE
}


/// Get the description of a GDS node
COREARRAY_DLL_EXPORT PyObject* gdsnDesp(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	if (!PyArg_ParseTuple(args, "in", &nidx, &ptr_int))
		return NULL;

	COREARRAY_TRY

		CdGDSObj *Obj = get_obj(nidx, ptr_int);

		string nm  = RawText(Obj->Name());
		string nm2 = RawText(Obj->FullName());
		string ste = Obj->dName();

		string tra = Obj->dTraitName();
		if (dynamic_cast<CdGDSVirtualFolder*>(Obj))
			tra = RawText(((CdGDSVirtualFolder*)Obj)->LinkFileName());

		string type = "Unknown";
		if (dynamic_cast<CdGDSLabel*>(Obj))
			type = "Label";
		else if (dynamic_cast<CdGDSFolder*>(Obj))
			type = "Folder";
		else if (dynamic_cast<CdGDSVirtualFolder*>(Obj))
			type = "VFolder";
		else if (dynamic_cast<CdGDSStreamContainer*>(Obj))
			type = "Raw";
		else if (dynamic_cast<CdContainer*>(Obj))
		{
			CdContainer* nn = static_cast<CdContainer*>(Obj);
			C_SVType sv = nn->SVType();
			if (COREARRAY_SV_INTEGER(sv))
			{
				if (GDS_R_Is_Factor(Obj))
					type = "Factor";
				else if (GDS_R_Is_Logical(Obj))
					type = "Logical";
				else
					type = "Integer";
			} else if (COREARRAY_SV_FLOAT(sv))
				type = "Real";
			else if (COREARRAY_SV_STRING(sv))
				type = "String";
		}

		// dim, the dimension of data field
		PyObject *dim;
		string encoder, compress;
		double cpratio = NaN;
		if (dynamic_cast<CdAbstractArray*>(Obj))
		{
			CdAbstractArray *_Obj = (CdAbstractArray*)Obj;

			dim = PyList_New(_Obj->DimCnt());
			for (int i=0; i < _Obj->DimCnt(); i++)
				PyList_SetItem(dim, i, PyInt_FromLong(_Obj->GetDLen(i)));

			if (_Obj->PipeInfo())
			{
				encoder = _Obj->PipeInfo()->Coder();
				compress = _Obj->PipeInfo()->CoderParam();
				if (_Obj->PipeInfo()->StreamTotalIn() > 0)
				{
					cpratio = (double)_Obj->PipeInfo()->StreamTotalOut() /
						_Obj->PipeInfo()->StreamTotalIn();
				}
			}
		} else {
			dim = Py_None; Py_INCREF(dim);
		}

		// object size
		double size = NaN;
		if (dynamic_cast<CdContainer*>(Obj))
		{
			CdContainer* p = static_cast<CdContainer*>(Obj);
			p->Synchronize();
			size = p->GDSStreamSize();
		} else if (dynamic_cast<CdGDSStreamContainer*>(Obj))
		{
			CdGDSStreamContainer *_Obj = (CdGDSStreamContainer*)Obj;
			if (_Obj->PipeInfo())
				size = _Obj->PipeInfo()->StreamTotalIn();
			else
				size = _Obj->GetSize();
		}

		// good
		int GoodFlag = 1;
		if (dynamic_cast<CdGDSVirtualFolder*>(Obj))
		{
			CdGDSVirtualFolder *v = (CdGDSVirtualFolder*)Obj;
			GoodFlag = v->IsLoaded(true) ? 1 : 0;
		} else if (dynamic_cast<CdGDSUnknown*>(Obj))
		{
			GoodFlag = 0;
		}

		// hidden
		int hidden_flag = Obj->GetHidden() ||
			Obj->Attribute().HasName(ASC16("R.invisible"));

		// message
		string msg;
		if (dynamic_cast<CdGDSVirtualFolder*>(Obj))
		{
			CdGDSVirtualFolder *v = (CdGDSVirtualFolder*)Obj;
			v->IsLoaded(true);
			msg = v->ErrMsg().c_str();
		}

		return Py_BuildValue(
			"{s:s,s:s,s:s,s:s,s:s,s:N,s:s,s:s,s:d,s:d,s:N,s:N,s:s}",
			"name",     nm.c_str(),
			"fullname", nm2.c_str(),
			"storage",  ste.c_str(),
			"trait",    tra.c_str(),
			"type",     type.c_str(),
			"dim",      dim,
			"encoder",  encoder.c_str(),
			"compress", compress.c_str(),
			"cpratio",  cpratio,
			"size",     size,
			"good",     PyBool_FromLong(GoodFlag),
			"hidden",   PyBool_FromLong(hidden_flag),
			"message",  msg.c_str()
		);

	COREARRAY_CATCH_NONE
}



// ----------------------------------------------------------------------------
// Data Operations
// ----------------------------------------------------------------------------

/// Read data from a GDS node
COREARRAY_DLL_EXPORT PyObject* gdsnRead(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	PyObject *start, *count;
	const char *cvt;
	if (!PyArg_ParseTuple(args, "inOOs", &nidx, &ptr_int, &start, &count, &cvt))
		return NULL;

	// check the argument 'cvt'
	C_SVType sv;
	if (strcmp(cvt, "") == 0)
		sv = svCustom;
	else if (strcmp(cvt, "int8") == 0)
		sv = svInt8;
	else if (strcmp(cvt, "uint8") == 0)
		sv = svUInt8;
	else if (strcmp(cvt, "int16") == 0)
		sv = svInt16;
	else if (strcmp(cvt, "uint16") == 0)
		sv = svUInt16;
	else if (strcmp(cvt, "int32") == 0)
		sv = svInt32;
	else if (strcmp(cvt, "uint32") == 0)
		sv = svUInt32;
	else if (strcmp(cvt, "int64") == 0)
		sv = svInt64;
	else if (strcmp(cvt, "uint64") == 0)
		sv = svUInt64;
	else if (strcmp(cvt, "float32") == 0)
		sv = svFloat32;
	else if (strcmp(cvt, "float64") == 0)
		sv = svFloat64;
	else if (strcmp(cvt, "utf8") == 0)
		sv = svStrUTF8;
	else if (strcmp(cvt, "utf16") == 0)
		sv = svStrUTF16;
	else {
		PyErr_SetString(PyExc_ValueError, "Invalid 'cvt'.");
		return NULL;
	}

	// check the argument 'start'
	CdAbstractArray::TArrayDim dm_st;
	int dm_st_n = 0;
	if (PyList_Check(start))
	{
		dm_st_n = PyList_Size(start);
		for(int i=0; i < dm_st_n; i++)
			dm_st[i] = PyInt_AsLong(PyList_GetItem(start, i));
	} else if (start != Py_None)
	{
		PyErr_SetString(PyExc_ValueError, "'start' should be None or a list.");
		return NULL;
	}

	// check the argument 'count'
	CdAbstractArray::TArrayDim dm_cnt;
	int dm_cnt_n = 0;
	if (PyList_Check(count))
	{
		dm_cnt_n = PyList_Size(count);
		for(int i=0; i < dm_cnt_n; i++)
			dm_cnt[i] = PyInt_AsLong(PyList_GetItem(count, i));
	} else if (count != Py_None)
	{
		PyErr_SetString(PyExc_ValueError, "'count' should be None or a list.");
		return NULL;
	}

	if ((dm_st_n==0 && dm_cnt_n>0) || (dm_st_n>0 && dm_cnt_n==0))
	{
		PyErr_SetString(PyExc_ValueError, "'start' and 'count' should be both None.");
		return NULL;
	}


	COREARRAY_TRY

		CdGDSObj *obj = get_obj(nidx, ptr_int);
		CdAbstractArray *Obj = dynamic_cast<CdAbstractArray*>(obj);
		if (Obj == NULL)
			throw ErrGDSFmt(ERR_NO_DATA);

		C_Int32 *pDS=NULL, *pDL=NULL;
		if (dm_st_n>0 && dm_cnt_n>0)
		{
			int Len = Obj->DimCnt();
			CdAbstractArray::TArrayDim DCnt;
			Obj->GetDim(DCnt);

			if (dm_st_n != Len)
				throw ErrGDSFmt("The length of 'start' is invalid.");
			for (int i=0; i < Len; i++)
			{
				if ((dm_st[i] < 0) || (dm_st[i] >= DCnt[i]))
					throw ErrGDSFmt("'start' is invalid.");
			}
			pDS = dm_st;

			if (dm_cnt_n != Len)
				throw ErrGDSFmt("The length of 'count' is invalid.");
			for (int i=0; i < Len; i++)
			{
				int &v = dm_cnt[i];
				if (v == -1)
					v = DCnt[i] - dm_st[i];
				if ((v <= 0) || ((dm_st[i]+v) >= DCnt[i]))
					throw ErrGDSFmt("'count' is invalid.");
			}
			pDL = dm_cnt;
		}

		PyObject *rv = GDS_Py_Array_Read(Obj, pDS, pDL, NULL, sv);
		return rv;

	COREARRAY_CATCH_NONE
	return NULL;
}



// ----------------------------------------------------------------------------
// Attribute Operations
// ----------------------------------------------------------------------------

COREARRAY_DLL_EXPORT PyObject* any2obj(CdAny &Obj)
{
	if (Obj.IsInt())
	{
		return PyInt_FromLong(Obj.GetInt32());
	} else if (Obj.IsFloat())
	{
		return PyFloat_FromDouble(Obj.GetFloat64());
	} else if (Obj.IsString())
	{
		const UTF8String &s = Obj.GetStr8();
		return PYSTR_SET2(s.c_str(), s.size());
	} else if (Obj.IsBool())
	{
		return PyBool_FromLong(Obj.GetBool() ? 1 : 0);
	} else if (Obj.IsArray())
	{
		const size_t n = Obj.GetArrayLength();
		CdAny *p = Obj.GetArray();
		PyObject *rv_ans = PyList_New(n);
		for (size_t i=0; i < n; i++)
			PyList_SetItem(rv_ans, i, any2obj(*p++));
		return rv_ans;
	} else
		Py_RETURN_NONE;
}

/// Get the attribute(s) of a GDS node
COREARRAY_DLL_EXPORT PyObject* gdsnGetAttr(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	if (!PyArg_ParseTuple(args, "in", &nidx, &ptr_int))
		return NULL;

	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		if (Obj->Attribute().Count() > 0)
		{
			const size_t n = Obj->Attribute().Count();
			PyObject *rv_ans = PyDict_New();
			for (size_t i=0; i < n; i++)
			{
				PyObject *x = any2obj(Obj->Attribute()[i]);
				PyDict_SetItemString(rv_ans,
					RawText(Obj->Attribute().Names(i)).c_str(), x);
				Py_DECREF(x);
			}
			return rv_ans;
		}
	COREARRAY_CATCH_NONE
}



// ----------------------------------------------------------------------------
// Register routines
// ----------------------------------------------------------------------------

extern COREARRAY_DLL_LOCAL void pygds_init();

static PyMethodDef module_methods[] = {
	// file operations
    { "create_gds", (PyCFunction)gdsCreateGDS, METH_VARARGS, NULL },
    { "open_gds", (PyCFunction)gdsOpenGDS, METH_VARARGS, NULL },
    { "close_gds", (PyCFunction)gdsCloseGDS, METH_VARARGS, NULL },
    { "sync_gds", (PyCFunction)gdsSyncGDS, METH_VARARGS, NULL },
    { "filesize", (PyCFunction)gdsFileSize, METH_VARARGS, NULL },
    { "tidy_up", (PyCFunction)gdsTidyUp, METH_VARARGS, NULL },
    { "root_gds", (PyCFunction)gdsRoot, METH_VARARGS, NULL },
    { "index_gds", (PyCFunction)gdsIndex, METH_VARARGS, NULL },

	// file structure operations
    { "ls_gdsn", (PyCFunction)gdsnListName, METH_VARARGS, NULL },
    { "index_gdsn", (PyCFunction)gdsnIndex, METH_VARARGS, NULL },
    { "name_gdsn", (PyCFunction)gdsnName, METH_VARARGS, NULL },
    { "rename_gdsn", (PyCFunction)gdsnRename, METH_VARARGS, NULL },
    { "desp_gdsn", (PyCFunction)gdsnDesp, METH_VARARGS, NULL },

	// data operations
    { "read_gdsn", (PyCFunction)gdsnRead, METH_VARARGS, NULL },

	// attribute operations
    { "getattr_gdsn", (PyCFunction)gdsnGetAttr, METH_VARARGS, NULL },

	// end
	{ NULL, NULL, 0, NULL }
};


// Module entry point Python

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef ModStruct =
{
	PyModuleDef_HEAD_INIT,
	"pygds.ccall",  // name of module
	"C functions for data manipulation",  // module documentation
	-1,  // size of per-interpreter state of the module, or -1 if the module keeps state in global variables
	module_methods
};

PyMODINIT_FUNC PyInit_ccall()
#else
PyMODINIT_FUNC initccall()
#endif
{
	pygds_init();

	// create the module and add the functions
	PyObject *mod;
#if PY_MAJOR_VERSION >= 3
	mod = PyModule_Create(&ModStruct);
#else
	mod = Py_InitModule("pygds.ccall", module_methods);
#endif

	if (mod)
	{
		// add some symbolic constants to the module
		PyObject *dict = PyModule_GetDict(mod);
		// add api pointer
		PyObject *c_api = PyCapsule_New((void*)PyGDS_API, NULL, NULL);
		if (c_api)
		{
			PyDict_SetItemString(dict, "_GDS_C_API", c_api);
			Py_DECREF(c_api);
		} else {
			mod = NULL;
		}
	}

	return mod;
}

}
