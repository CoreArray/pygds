// ===========================================================
//
// pygds.cpp: Python Interface to CoreArray Genomic Data Structure (GDS) Files
//
// Copyright (C) 2017-2026    Xiuwen Zheng
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


#define PY_EXPORT    static


namespace pygds
{
	extern PdGDSFile PKG_GDS_Files[];
	extern vector<PdGDSObj> PKG_GDSObj_List;
	extern map<PdGDSObj, int> PKG_GDSObj_Map;
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
			ClassMap["packedreal8"]  = TdTraits< TReal8  >::StreamName();
			ClassMap["packedreal16"] = TdTraits< TReal16 >::StreamName();
			ClassMap["packedreal24"] = TdTraits< TReal24 >::StreamName();
			ClassMap["packedreal32"] = TdTraits< TReal32 >::StreamName();


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

	map<PdGDSObj, int>::iterator it = PKG_GDSObj_Map.find(Obj);
	if (it != PKG_GDSObj_Map.end())
	{
		outidx = it->second;
		if ((outidx < 0) || (outidx >= (int)PKG_GDSObj_List.size()))
			throw ErrGDSFmt(ERR_OBJLIST);
		if (PKG_GDSObj_List[outidx] != Obj)
			throw ErrGDSFmt(ERR_OBJLIST);
	} else {
		vector<PdGDSObj>::iterator it =
			find(PKG_GDSObj_List.begin(), PKG_GDSObj_List.end(),
			(PdGDSObj)NULL);
		if (it != PKG_GDSObj_List.end())
		{
			outidx = it - PKG_GDSObj_List.begin();
			*it = Obj;
		} else {
			outidx = PKG_GDSObj_List.size();
			PKG_GDSObj_List.push_back(Obj);
		}
		PKG_GDSObj_Map[Obj] = outidx;
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
	if ((idx < 0) || (idx >= (int)PKG_GDSObj_List.size()))
		throw ErrGDSFmt(ERR_GDS_OBJ);
	if (ptr == NULL)
		throw ErrGDSFmt(ERR_GDS_OBJ);
	if (PKG_GDSObj_List[idx] != ptr)
		throw ErrGDSFmt(ERR_GDS_OBJ2);

	return ptr;
}



// ----------------------------------------------------------------------------
// File Operations
// ----------------------------------------------------------------------------

/// Create a GDS file
PY_EXPORT PyObject* gdsCreateGDS(PyObject *self, PyObject *args)
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
			for (int i=0; i < PKG_MAX_NUM_GDS_FILES; i++)
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
PY_EXPORT PyObject* gdsOpenGDS(PyObject *self, PyObject *args)
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
			for (int i=0; i < PKG_MAX_NUM_GDS_FILES; i++)
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
PY_EXPORT PyObject* gdsCloseGDS(PyObject *self, PyObject *args)
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
PY_EXPORT PyObject* gdsSyncGDS(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;

	COREARRAY_TRY
		GDS_ID2File(file_id)->SyncFile();
	COREARRAY_CATCH_NONE
}


/// Get the file size and check the file handler
PY_EXPORT PyObject* gdsFileSize(PyObject *self, PyObject *args)
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
PY_EXPORT PyObject* gdsTidyUp(PyObject *self, PyObject *args)
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
PY_EXPORT PyObject* gdsRoot(PyObject *self, PyObject *args)
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
PY_EXPORT PyObject* gdsIndex(PyObject *self, PyObject *args)
{
	int file_id;
	const char *path;
	int silent;
	if (!PyArg_ParseTuple(args, "is" BSTR, &file_id, &path, &silent))
		return NULL;

	int idx;
	Py_ssize_t ptr;
	COREARRAY_TRY
		CdGDSObj *Obj = GDS_ID2File(file_id)->Root().PathEx(UTF8Text(path));
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
PY_EXPORT PyObject* gdsnListName(PyObject *self, PyObject *args)
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
							!Obj->Attribute().HasName(UTF8Text("R.invisible")))
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
PY_EXPORT PyObject* gdsnIndex(PyObject *self, PyObject *args)
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
			Obj = Dir->PathEx(UTF8Text(path));
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
PY_EXPORT PyObject* gdsnName(PyObject *self, PyObject *args)
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
PY_EXPORT PyObject* gdsnRename(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	const char *newname;
	if (!PyArg_ParseTuple(args, "ins", &nidx, &ptr_int, &newname))
		return NULL;

	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		Obj->SetName(UTF8Text(newname));
	COREARRAY_CATCH_NONE
}


/// Get the description of a GDS node
PY_EXPORT PyObject* gdsnDesp(PyObject *self, PyObject *args)
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
				if (GDS_Is_RFactor(Obj))
					type = "Factor";
				else if (GDS_Is_RLogical(Obj))
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
			Obj->Attribute().HasName(UTF8Text("R.invisible"));

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

static bool cvt2sv(const char *cvt, C_SVType &sv)
{
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
		return true;
	}
	return false;
}

/// Read data from a GDS node
PY_EXPORT PyObject* gdsnRead(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	PyObject *start, *count;
	const char *cvt;
	if (!PyArg_ParseTuple(args, "inOOs", &nidx, &ptr_int, &start, &count, &cvt))
		return NULL;

	// check the argument 'cvt'
	C_SVType sv;
	if (cvt2sv(cvt, sv)) return NULL;

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
					throw ErrGDSFmt("'start[%d]=%d' is invalid.", i, dm_st[i]);
			}
			pDS = dm_st;

			if (dm_cnt_n != Len)
				throw ErrGDSFmt("The length of 'count' is invalid.");
			for (int i=0; i < Len; i++)
			{
				int &v = dm_cnt[i];
				if (v == -1)
					v = DCnt[i] - dm_st[i];
				if ((v <= 0) || ((dm_st[i]+v) > DCnt[i]))
					throw ErrGDSFmt("'count[%d]=%d' is invalid.", i, v);
			}
			pDL = dm_cnt;
		}

		PyObject *rv = GDS_Py_Array_Read(Obj, pDS, pDL, NULL, sv);
		return rv;

	COREARRAY_CATCH_NONE
}


/// Read data from a GDS node
PY_EXPORT PyObject* gdsnRead2(PyObject *self, PyObject *args)
{
	int nidx;
	Py_ssize_t ptr_int;
	PyObject *selection;
	const char *cvt;
	if (!PyArg_ParseTuple(args, "inOs", &nidx, &ptr_int, &selection, &cvt))
		return NULL;

	// check the argument 'cvt'
	C_SVType sv;
	if (cvt2sv(cvt, sv)) return NULL;

	// check the argument 'sel'
	if (!PyList_Check(selection))
	{
		PyErr_SetString(PyExc_ValueError, "'sel' should be a list.");
		return NULL;
	}

	COREARRAY_TRY

		CdGDSObj *obj = get_obj(nidx, ptr_int);
		CdAbstractArray *Obj = dynamic_cast<CdAbstractArray*>(obj);
		if (Obj == NULL)
			throw ErrGDSFmt(ERR_NO_DATA);
		if (PyList_Size(selection) != Obj->DimCnt())
			throw ErrGDSFmt("The dimension of 'sel' is not correct.");

		// set the selection
		vector< vector<C_BOOL> > tmpSel(Obj->DimCnt());
		vector<C_BOOL*> SelList(Obj->DimCnt());

		for (size_t i=0; i < SelList.size(); i++)
		{
			PyObject *sel = PyList_GET_ITEM(selection, i);
			if (sel == Py_None)
			{
				tmpSel[i].resize(Obj->GetDLen(i), 1);
				SelList[i] = &(tmpSel[i][0]);
			} else {
				extern C_BOOL *numpy_get_bool(PyObject *obj, size_t &num);
				size_t n = 0;
				C_BOOL *bs = numpy_get_bool(sel, n);
				if (bs == NULL)
					throw ErrGDSFmt("'sel[%d]' should be a bool numpy vector or None.", (int)i);
				if (n != (size_t)Obj->GetDLen(i))
					throw ErrGDSFmt("The length of 'sel[%d]' is not correct.", (int)i);
				SelList[i] = bs;
			}
		}

		// read data
		PyObject *rv = GDS_Py_Array_Read(Obj, NULL, NULL, &(SelList[0]), sv);
		return rv;

	COREARRAY_CATCH_NONE
}




// ----------------------------------------------------------------------------
// Attribute Operations
// ----------------------------------------------------------------------------

PY_EXPORT PyObject* any2obj(CdAny &Obj)
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
PY_EXPORT PyObject* gdsnGetAttr(PyObject *self, PyObject *args)
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
// Node creation, deletion and data writing
// ----------------------------------------------------------------------------

// defined in PyCoreArray.cpp: contiguous numpy buffer + element count + sv type
extern void *numpy_get_data(PyObject *obj, size_t &num, int &sv_out);


/// map a storage name through the class-name table
static const char *map_storage(const char *stm)
{
	map<const char*, const char*, CInitNameObject::strCmp>::iterator it =
		Init.ClassMap.find(stm);
	return (it != Init.ClassMap.end()) ? it->second : stm;
}


/// append numeric (numpy array) or string (python list) data to an array node
static void append_value(CdAbstractArray *Obj, PyObject *val)
{
	if (PyList_Check(val))
	{
		// a list of strings
		Py_ssize_t n = PyList_Size(val);
		vector<UTF8String> buf(n);
		for (Py_ssize_t i=0; i < n; i++)
		{
			PyObject *s = PyList_GetItem(val, i);
			if (PYSTR_IS(s))
				buf[i] = UTF8Text(PYSTR_CHAR(s));
			else if (s != Py_None)
				throw ErrGDSFmt("'val[%d]' should be a string.", (int)i);
		}
		if (n > 0)
			Obj->Append(&buf[0], n, svStrUTF8);
	} else {
		size_t num = 0; int sv = -1;
		void *p = numpy_get_data(val, num, sv);
		if (p == NULL)
			throw ErrGDSFmt("'val' should be a C-contiguous numpy array of a "
				"supported numeric dtype, or a list of strings.");
		if (num > 0)
			Obj->Append(p, num, (C_SVType)sv);
	}
}


/// Add a new GDS node (array or label); returns (idx, ptr)
PY_EXPORT PyObject* gdsnAddNode(PyObject *self, PyObject *args)
{
	int pidx; Py_ssize_t pptr;
	const char *name, *storage, *compress;
	int visible, replace, maxlen;
	double offset, scale;
	if (!PyArg_ParseTuple(args, "insss" BSTR BSTR "idd",
			&pidx, &pptr, &name, &storage, &compress,
			&visible, &replace, &maxlen, &offset, &scale))
		return NULL;

	int idx; Py_ssize_t ptr;
	COREARRAY_TRY

		CdGDSObj *obj = get_obj(pidx, pptr);
		if (!dynamic_cast<CdGDSAbsFolder*>(obj))
			throw ErrGDSFmt(ERR_NOT_FOLDER);
		CdGDSAbsFolder &Dir = *((CdGDSAbsFolder*)obj);

		int IdxReplace = -1;
		if (replace)
		{
			CdGDSObj *tmp = Dir.ObjItemEx(UTF8Text(name));
			if (tmp)
			{
				IdxReplace = Dir.IndexObj(tmp);
				GDS_Node_Delete(tmp, true);
			}
		}

		const char *stm = map_storage(storage);
		CdGDSObj *rv_obj = NULL;
		if (strcmp(stm, "$FOLDER$") == 0)
			rv_obj = new CdGDSFolder();
		else if ((strcmp(stm, "NULL")==0) || (stm[0]==0))
			rv_obj = new CdGDSLabel();
		else {
			CdObjClassMgr::TdOnObjCreate OnCreate =
				dObjManager().NameToClass(stm);
			if (OnCreate)
			{
				CdObjRef *o = (*OnCreate)();
				if (dynamic_cast<CdGDSObj*>(o))
					rv_obj = static_cast<CdGDSObj*>(o);
				else
					delete o;
			}
		}
		if (rv_obj == NULL)
			throw ErrGDSFmt("No support of the storage mode '%s'.", storage);

		Dir.InsertObj(IdxReplace, UTF8Text(name), rv_obj);

		// hidden flag
		if (!visible)
		{
			rv_obj->SetHidden(true);
			rv_obj->Attribute().Add(UTF8Text("R.invisible"));
		}

		// fixed-length string max length
		if (maxlen > 0)
		{
			if (dynamic_cast<CdFStr8*>(rv_obj))
				static_cast<CdFStr8*>(rv_obj)->SetMaxLength(maxlen);
			else if (dynamic_cast<CdFStr16*>(rv_obj))
				static_cast<CdFStr16*>(rv_obj)->SetMaxLength(maxlen);
			else if (dynamic_cast<CdFStr32*>(rv_obj))
				static_cast<CdFStr32*>(rv_obj)->SetMaxLength(maxlen);
		}

		// packed-real offset / scale
		if (CoreArray::IsFinite(offset) || CoreArray::IsFinite(scale))
		{
			#define SET_PACKED(CLASS) \
				else if (dynamic_cast<CLASS*>(rv_obj)) { \
					CLASS *o = static_cast<CLASS*>(rv_obj); \
					if (CoreArray::IsFinite(offset)) o->SetOffset(offset); \
					if (CoreArray::IsFinite(scale))  o->SetScale(scale); }
			if (false) {}
			SET_PACKED(CdPackedReal8)  SET_PACKED(CdPackedReal8U)
			SET_PACKED(CdPackedReal16) SET_PACKED(CdPackedReal16U)
			SET_PACKED(CdPackedReal24) SET_PACKED(CdPackedReal24U)
			SET_PACKED(CdPackedReal32) SET_PACKED(CdPackedReal32U)
			#undef SET_PACKED
		}

		// compression mode
		if (compress[0] && dynamic_cast<CdContainer*>(rv_obj))
			static_cast<CdContainer*>(rv_obj)->SetPackedMode(compress);

		set_obj(rv_obj, idx, ptr);

	COREARRAY_CATCH
	return Py_BuildValue("in", idx, ptr);
}


/// Add a new (sub) folder; returns (idx, ptr)
PY_EXPORT PyObject* gdsnAddFolder(PyObject *self, PyObject *args)
{
	int pidx; Py_ssize_t pptr;
	const char *name, *type, *gds_fn;
	int visible, replace;
	if (!PyArg_ParseTuple(args, "insss" BSTR BSTR,
			&pidx, &pptr, &name, &type, &gds_fn, &visible, &replace))
		return NULL;

	int idx; Py_ssize_t ptr;
	COREARRAY_TRY

		CdGDSObj *obj = get_obj(pidx, pptr);
		if (!dynamic_cast<CdGDSAbsFolder*>(obj))
			throw ErrGDSFmt(ERR_NOT_FOLDER);
		CdGDSAbsFolder &Dir = *((CdGDSAbsFolder*)obj);

		int IdxReplace = -1;
		if (replace)
		{
			CdGDSObj *tmp = Dir.ObjItemEx(UTF8Text(name));
			if (tmp)
			{
				IdxReplace = Dir.IndexObj(tmp);
				GDS_Node_Delete(tmp, true);
			}
		}

		CdGDSObj *vObj = NULL;
		if (strcmp(type, "directory") == 0)
		{
			vObj = Dir.AddFolder(UTF8Text(name));
		} else if (strcmp(type, "virtual") == 0)
		{
			CdGDSVirtualFolder *F = new CdGDSVirtualFolder;
			Dir.InsertObj(IdxReplace, UTF8Text(name), F);
			F->SetLinkFile(UTF8Text(gds_fn));
			vObj = F;
		} else
			throw ErrGDSFmt("Invalid 'type = %s'.", type);

		if (!visible)
		{
			vObj->SetHidden(true);
			vObj->Attribute().Add(UTF8Text("R.invisible"));
		}

		set_obj(vObj, idx, ptr);

	COREARRAY_CATCH
	return Py_BuildValue("in", idx, ptr);
}


/// Delete a GDS node
PY_EXPORT PyObject* gdsnDelete(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; int force;
	if (!PyArg_ParseTuple(args, "in" BSTR, &nidx, &ptr_int, &force))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		// drop it from the tracking list as well
		map<PdGDSObj, int>::iterator it = PKG_GDSObj_Map.find(Obj);
		if (it != PKG_GDSObj_Map.end())
		{
			PKG_GDSObj_List[it->second] = NULL;
			PKG_GDSObj_Map.erase(it);
		}
		GDS_Node_Delete(Obj, force);
	COREARRAY_CATCH_NONE
}


/// Append data to an array node
PY_EXPORT PyObject* gdsnAppend(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; PyObject *val;
	if (!PyArg_ParseTuple(args, "inO", &nidx, &ptr_int, &val))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *obj = get_obj(nidx, ptr_int);
		CdAbstractArray *Obj = dynamic_cast<CdAbstractArray*>(obj);
		if (Obj == NULL) throw ErrGDSFmt(ERR_NO_DATA);
		append_value(Obj, val);
		if (Obj->PipeInfo()) Obj->PipeInfo()->UpdateStreamSize();
	COREARRAY_CATCH_NONE
}


/// Write a whole array: reset the dimensions then append all data
PY_EXPORT PyObject* gdsnWriteAll(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; PyObject *val, *dim;
	if (!PyArg_ParseTuple(args, "inOO", &nidx, &ptr_int, &val, &dim))
		return NULL;
	if (!PyList_Check(dim))
	{
		PyErr_SetString(PyExc_ValueError, "'dim' should be a list.");
		return NULL;
	}
	COREARRAY_TRY
		CdGDSObj *obj = get_obj(nidx, ptr_int);
		CdAbstractArray *Obj = dynamic_cast<CdAbstractArray*>(obj);
		if (Obj == NULL) throw ErrGDSFmt(ERR_NO_DATA);

		// numpy C-order shape maps directly to CoreArray dimensions
		int ndim = (int)PyList_Size(dim);
		if (ndim <= 0) ndim = 1;
		CdAbstractArray::TArrayDim DDim;
		if ((int)PyList_Size(dim) <= 0)
			DDim[0] = 0;
		else {
			for (int i=0; i < ndim; i++)
				DDim[i] = PyInt_AsLong(PyList_GetItem(dim, i));
		}
		DDim[0] = 0;  // grow the outermost dim while appending
		Obj->ResetDim(DDim, ndim);

		append_value(Obj, val);
		if (Obj->PipeInfo()) Obj->PipeInfo()->UpdateStreamSize();
	COREARRAY_CATCH_NONE
}


/// Write data to a sub-region of an array node (start/count are 0-based lists)
PY_EXPORT PyObject* gdsnWrite(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int;
	PyObject *val, *start, *count;
	if (!PyArg_ParseTuple(args, "inOOO", &nidx, &ptr_int, &val, &start, &count))
		return NULL;

	CdAbstractArray::TArrayDim dm_st, dm_cnt;
	int dm_st_n = 0, dm_cnt_n = 0;
	if (PyList_Check(start))
	{
		dm_st_n = PyList_Size(start);
		for (int i=0; i < dm_st_n; i++)
			dm_st[i] = PyInt_AsLong(PyList_GetItem(start, i));
	}
	if (PyList_Check(count))
	{
		dm_cnt_n = PyList_Size(count);
		for (int i=0; i < dm_cnt_n; i++)
			dm_cnt[i] = PyInt_AsLong(PyList_GetItem(count, i));
	}

	COREARRAY_TRY
		CdGDSObj *obj = get_obj(nidx, ptr_int);
		CdAbstractArray *Obj = dynamic_cast<CdAbstractArray*>(obj);
		if (Obj == NULL) throw ErrGDSFmt(ERR_NO_DATA);

		int Len = Obj->DimCnt();
		if (dm_st_n != Len || dm_cnt_n != Len)
			throw ErrGDSFmt("'start'/'count' length must equal the dimensions.");

		size_t num = 0; int sv = -1;
		if (PyList_Check(val))
		{
			Py_ssize_t n = PyList_Size(val);
			vector<UTF8String> buf(n);
			for (Py_ssize_t i=0; i < n; i++)
			{
				PyObject *s = PyList_GetItem(val, i);
				if (PYSTR_IS(s)) buf[i] = UTF8Text(PYSTR_CHAR(s));
			}
			Obj->WriteData(dm_st, dm_cnt, n>0 ? &buf[0] : NULL, svStrUTF8);
		} else {
			void *p = numpy_get_data(val, num, sv);
			if (p == NULL)
				throw ErrGDSFmt("'val' should be a C-contiguous numpy array or "
					"a list of strings.");
			Obj->WriteData(dm_st, dm_cnt, p, (C_SVType)sv);
		}
	COREARRAY_CATCH_NONE
}


/// Set the dimensions of an array node
PY_EXPORT PyObject* gdsnSetDim(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; PyObject *dim; int permute;
	if (!PyArg_ParseTuple(args, "inO" BSTR, &nidx, &ptr_int, &dim, &permute))
		return NULL;
	if (!PyList_Check(dim))
	{
		PyErr_SetString(PyExc_ValueError, "'dim' should be a list.");
		return NULL;
	}
	COREARRAY_TRY
		CdGDSObj *obj = get_obj(nidx, ptr_int);
		CdAbstractArray *Obj = dynamic_cast<CdAbstractArray*>(obj);
		if (Obj == NULL) throw ErrGDSFmt(ERR_NO_DATA);

		int ndim = (int)PyList_Size(dim);
		if (ndim <= 0)
			throw ErrGDSFmt("The number of dimensions should be > 0.");
		if (ndim > CdAbstractArray::MAX_ARRAY_DIM)
			throw ErrGDSFmt("Too many dimensions.");
		CdAbstractArray::TArrayDim Dim;
		for (int i=0; i < ndim; i++)
		{
			long v = PyInt_AsLong(PyList_GetItem(dim, i));
			Dim[i] = (v < 0) ? 0 : v;
		}
		if (permute)
		{
			for (int i=ndim-1; i >= 0; i--)
				Obj->SetDLen(i, Dim[i]);
		} else
			Obj->ResetDim(Dim, ndim);
	COREARRAY_CATCH_NONE
}


/// Set a new compression mode
PY_EXPORT PyObject* gdsnCompress(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; const char *cp;
	if (!PyArg_ParseTuple(args, "ins", &nidx, &ptr_int, &cp))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		if (dynamic_cast<CdContainer*>(Obj))
			static_cast<CdContainer*>(Obj)->SetPackedMode(cp);
		else if (dynamic_cast<CdGDSStreamContainer*>(Obj))
			static_cast<CdGDSStreamContainer*>(Obj)->SetPackedMode(cp);
		else
			throw ErrGDSFmt("Not allowed compression / decompression.");
	COREARRAY_CATCH_NONE
}


/// Close the compression mode (switch to read mode)
PY_EXPORT PyObject* gdsnReadMode(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int;
	if (!PyArg_ParseTuple(args, "in", &nidx, &ptr_int))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		if (dynamic_cast<CdContainer*>(Obj))
			static_cast<CdContainer*>(Obj)->CloseWriter();
	COREARRAY_CATCH_NONE
}


/// Set / add an attribute
PY_EXPORT PyObject* gdsnPutAttr(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int;
	const char *name; PyObject *val;
	if (!PyArg_ParseTuple(args, "insO", &nidx, &ptr_int, &name, &val))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		CdAny *p;
		if (Obj->Attribute().HasName(UTF8Text(name)))
		{
			p = &(Obj->Attribute()[UTF8Text(name)]);
			Obj->Attribute().Changed();
		} else
			p = &(Obj->Attribute().Add(UTF8Text(name)));

		if (val == Py_None)
		{
			// a NULL-valued attribute (a presence marker, e.g. SNPRelate's
			// "sample.order"); Add() already created an empty CdAny.
			p->SetEmpty();
		} else if (PyBool_Check(val))
		{
			p->SetBool(val == Py_True);
		} else if (PyLong_Check(val))
		{
			p->SetInt32((C_Int32)PyLong_AsLong(val));
		} else if (PyFloat_Check(val))
		{
			p->SetFloat64(PyFloat_AsDouble(val));
		} else if (PYSTR_IS(val))
		{
			p->SetStr8(UTF8Text(PYSTR_CHAR(val)));
		} else if (PyList_Check(val))
		{
			Py_ssize_t n = PyList_Size(val);
			p->SetArray(n);
			for (Py_ssize_t i=0; i < n; i++)
			{
				PyObject *e = PyList_GetItem(val, i);
				CdAny &a = p->GetArray()[i];
				if (PyBool_Check(e)) a.SetBool(e == Py_True);
				else if (PyLong_Check(e)) a.SetInt32((C_Int32)PyLong_AsLong(e));
				else if (PyFloat_Check(e)) a.SetFloat64(PyFloat_AsDouble(e));
				else if (PYSTR_IS(e)) a.SetStr8(UTF8Text(PYSTR_CHAR(e)));
				else throw ErrGDSFmt("Unsupported attribute element type.");
			}
		} else
			throw ErrGDSFmt("Unsupported attribute type.");
	COREARRAY_CATCH_NONE
}


/// Delete an attribute
PY_EXPORT PyObject* gdsnDeleteAttr(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; const char *name;
	if (!PyArg_ParseTuple(args, "ins", &nidx, &ptr_int, &name))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		Obj->Attribute().Delete(UTF8Text(name));
	COREARRAY_CATCH_NONE
}


/// Test whether a path exists under a folder node
PY_EXPORT PyObject* gdsnExist(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int; const char *path;
	if (!PyArg_ParseTuple(args, "ins", &nidx, &ptr_int, &path))
		return NULL;
	int flag = 0;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		CdGDSAbsFolder *Dir = dynamic_cast<CdGDSAbsFolder*>(Obj);
		if (!Dir) throw ErrGDSFmt(ERR_NOT_FOLDER);
		flag = Dir->PathEx(UTF8Text(path)) ? 1 : 0;
	COREARRAY_CATCH
	return PyBool_FromLong(flag);
}


/// Test whether a node is a sparse array
PY_EXPORT PyObject* gdsnIsSparse(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr_int;
	if (!PyArg_ParseTuple(args, "in", &nidx, &ptr_int))
		return NULL;
	int flag = 0;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr_int);
		flag = dynamic_cast<CdSpExStruct*>(Obj) ? 1 : 0;
	COREARRAY_CATCH
	return PyBool_FromLong(flag);
}



// ----------------------------------------------------------------------------
// Relocation, copy, assign, caching and embedded files
// ----------------------------------------------------------------------------

/// Move a node to a new location relative to another node in the same folder
PY_EXPORT PyObject* gdsnMoveTo(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr; int lidx; Py_ssize_t lptr; const char *relpos;
	if (!PyArg_ParseTuple(args, "inins", &nidx, &ptr, &lidx, &lptr, &relpos))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr);
		CdGDSObj *LObj = get_obj(lidx, lptr);

		if (strcmp(relpos, "into") == 0)
		{
			CdGDSFolder *Dest = dynamic_cast<CdGDSFolder*>(LObj);
			if (!Dest)
				throw ErrGDSFmt("'loc_node' must be a folder when relpos=\"into\".");
			if (Obj == LObj)
				throw ErrGDSFmt("'node' and 'loc_node' are the same folder.");
			if (Obj->Folder() != Dest)
				Obj->MoveTo(*Dest);
		} else if (Obj->Folder() == LObj->Folder())
		{
			int i_Obj  = Obj->Folder()->IndexObj(Obj);
			int i_LObj = LObj->Folder()->IndexObj(LObj);
			if (i_Obj != i_LObj)
			{
				if ((strcmp(relpos,"after")==0) || (strcmp(relpos,"replace")==0) ||
					(strcmp(relpos,"replace+rename")==0))
				{
					if (i_Obj <= i_LObj)
						Obj->Folder()->MoveTo(i_Obj, i_LObj);
					else
						Obj->Folder()->MoveTo(i_Obj, i_LObj+1);
					if (strcmp(relpos, "replace") == 0)
					{
						GDS_Node_Delete(LObj, true);
					} else if (strcmp(relpos, "replace+rename") == 0)
					{
						UTF8String nm(LObj->Name());
						GDS_Node_Delete(LObj, true);
						Obj->SetName(nm);
					}
				} else if (strcmp(relpos, "before") == 0)
				{
					if (i_Obj <= i_LObj)
						Obj->Folder()->MoveTo(i_Obj, i_LObj-1);
					else
						Obj->Folder()->MoveTo(i_Obj, i_LObj);
				} else
					throw ErrGDSFmt("Invalid 'relpos'!");
			}
		} else
			throw ErrGDSFmt("'node' and 'loc_node' should be in the same folder.");
	COREARRAY_CATCH_NONE
}


/// Copy a source node into a folder under a new name; returns (idx, ptr)
PY_EXPORT PyObject* gdsnCopyTo(PyObject *self, PyObject *args)
{
	int fidx; Py_ssize_t fptr; const char *name; int sidx; Py_ssize_t sptr;
	if (!PyArg_ParseTuple(args, "insin", &fidx, &fptr, &name, &sidx, &sptr))
		return NULL;
	int idx; Py_ssize_t ptr;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(fidx, fptr);
		CdGDSObj *SObj = get_obj(sidx, sptr);
		if (!dynamic_cast<CdGDSAbsFolder*>(Obj))
			throw ErrGDSFmt("'node' should be a folder.");
		if (dynamic_cast<CdGDSAbsFolder*>(SObj) &&
			static_cast<CdGDSAbsFolder*>(SObj)->HasChild(Obj, true))
			throw ErrGDSFmt("Should not copy the node(s) into its sub folder.");
		CdGDSAbsFolder *Folder = static_cast<CdGDSAbsFolder*>(Obj);
		UTF8String s = UTF8Text(name);
		if (Folder->ObjItemEx(s) != NULL)
			throw ErrGDSFmt("Copy error: '%s' has existed.", name);
		CdGDSObj *o = Folder->AddObj(s, SObj->NewObject());
		o->Assign(*SObj, true);
		set_obj(o, idx, ptr);
	COREARRAY_CATCH
	return Py_BuildValue("in", idx, ptr);
}


/// Assign (deep copy) the content of a source node to a destination node
PY_EXPORT PyObject* gdsnAssign(PyObject *self, PyObject *args)
{
	int didx; Py_ssize_t dptr; int sidx; Py_ssize_t sptr;
	if (!PyArg_ParseTuple(args, "inin", &didx, &dptr, &sidx, &sptr))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Dst = get_obj(didx, dptr);
		CdGDSObj *Src = get_obj(sidx, sptr);
		Dst->Assign(*Src, true);
	COREARRAY_CATCH_NONE
}


/// Cache the data associated with a node in memory
PY_EXPORT PyObject* gdsnCache(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr;
	if (!PyArg_ParseTuple(args, "in", &nidx, &ptr))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr);
		if (dynamic_cast<CdContainer*>(Obj))
			static_cast<CdContainer*>(Obj)->Caching();
	COREARRAY_CATCH_NONE
}


/// Unload a node to free memory
PY_EXPORT PyObject* gdsnUnload(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr;
	if (!PyArg_ParseTuple(args, "in", &nidx, &ptr))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr);
		if (Obj->Folder())
			Obj->Folder()->UnloadObj(Obj);
	COREARRAY_CATCH_NONE
}


/// Add an external file into the GDS as a stream container; returns (idx, ptr)
PY_EXPORT PyObject* gdsnAddFile(PyObject *self, PyObject *args)
{
	int fidx; Py_ssize_t fptr;
	const char *name, *filename, *compress;
	int replace, visible;
	if (!PyArg_ParseTuple(args, "insss" BSTR BSTR, &fidx, &fptr, &name,
			&filename, &compress, &replace, &visible))
		return NULL;
	int idx; Py_ssize_t ptr;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(fidx, fptr);
		if (!dynamic_cast<CdGDSAbsFolder*>(Obj))
			throw ErrGDSFmt(ERR_NOT_FOLDER);
		CdGDSAbsFolder &Dir = *((CdGDSAbsFolder*)Obj);

		int IdxReplace = -1;
		if (replace)
		{
			CdGDSObj *tmp = Dir.ObjItemEx(UTF8Text(name));
			if (tmp) { IdxReplace = Dir.IndexObj(tmp); GDS_Node_Delete(tmp, true); }
		}

		TdAutoRef<CdBufStream> file(new CdBufStream(
			new CdFileStream(filename, CdFileStream::fmOpenRead)));
		CdGDSStreamContainer *vObj = new CdGDSStreamContainer();
		vObj->SetPackedMode(compress);
		Dir.InsertObj(IdxReplace, UTF8Text(name), vObj);
		vObj->CopyFromBuf(*file.get());
		vObj->CloseWriter();

		if (!visible)
		{
			vObj->SetHidden(true);
			vObj->Attribute().Add(UTF8Text("R.invisible"));
		}

		set_obj(vObj, idx, ptr);
	COREARRAY_CATCH
	return Py_BuildValue("in", idx, ptr);
}


/// Extract an embedded file node to an external file
PY_EXPORT PyObject* gdsnGetFile(PyObject *self, PyObject *args)
{
	int nidx; Py_ssize_t ptr; const char *out_fn;
	if (!PyArg_ParseTuple(args, "ins", &nidx, &ptr, &out_fn))
		return NULL;
	COREARRAY_TRY
		CdGDSObj *Obj = get_obj(nidx, ptr);
		if (!dynamic_cast<CdGDSStreamContainer*>(Obj))
			throw ErrGDSFmt("It is not a stream container!");
		CdGDSStreamContainer *_Obj = static_cast<CdGDSStreamContainer*>(Obj);
		TdAutoRef<CdBufStream> file(new CdBufStream(
			new CdFileStream(out_fn, CdFileStream::fmCreate)));
		_Obj->CopyTo(*file.get());
	COREARRAY_CATCH_NONE
}


/// File fragment diagnosis; returns dict {num_fragment, size}
PY_EXPORT PyObject* gdsDiagnosis(PyObject *self, PyObject *args)
{
	int file_id;
	if (!PyArg_ParseTuple(args, "i", &file_id))
		return NULL;
	int nfrag = 0; double sz = NaN;
	COREARRAY_TRY
		CdGDSFile *file = GDS_ID2File(file_id);
		nfrag = file->GetNumOfFragment();
		sz = file->GetFileSize();
	COREARRAY_CATCH
	return Py_BuildValue("{s:i,s:d}", "num_fragment", nfrag, "size", sz);
}



// ----------------------------------------------------------------------------
// Register routines
// ----------------------------------------------------------------------------

extern COREARRAY_DLL_LOCAL bool pygds_init();

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

	// node creation / deletion
	{ "add_gdsn", (PyCFunction)gdsnAddNode, METH_VARARGS, NULL },
	{ "addfolder_gdsn", (PyCFunction)gdsnAddFolder, METH_VARARGS, NULL },
	{ "delete_gdsn", (PyCFunction)gdsnDelete, METH_VARARGS, NULL },

	// data operations
	{ "read_gdsn", (PyCFunction)gdsnRead, METH_VARARGS, NULL },
	{ "read2_gdsn", (PyCFunction)gdsnRead2, METH_VARARGS, NULL },
	{ "writeall_gdsn", (PyCFunction)gdsnWriteAll, METH_VARARGS, NULL },
	{ "write_gdsn", (PyCFunction)gdsnWrite, METH_VARARGS, NULL },
	{ "append_gdsn", (PyCFunction)gdsnAppend, METH_VARARGS, NULL },
	{ "setdim_gdsn", (PyCFunction)gdsnSetDim, METH_VARARGS, NULL },
	{ "compression_gdsn", (PyCFunction)gdsnCompress, METH_VARARGS, NULL },
	{ "readmode_gdsn", (PyCFunction)gdsnReadMode, METH_VARARGS, NULL },
	{ "is_sparse_gdsn", (PyCFunction)gdsnIsSparse, METH_VARARGS, NULL },
	{ "exist_gdsn", (PyCFunction)gdsnExist, METH_VARARGS, NULL },

	// relocation / copy / files
	{ "moveto_gdsn", (PyCFunction)gdsnMoveTo, METH_VARARGS, NULL },
	{ "copyto_gdsn", (PyCFunction)gdsnCopyTo, METH_VARARGS, NULL },
	{ "assign_gdsn", (PyCFunction)gdsnAssign, METH_VARARGS, NULL },
	{ "cache_gdsn", (PyCFunction)gdsnCache, METH_VARARGS, NULL },
	{ "unload_gdsn", (PyCFunction)gdsnUnload, METH_VARARGS, NULL },
	{ "addfile_gdsn", (PyCFunction)gdsnAddFile, METH_VARARGS, NULL },
	{ "getfile_gdsn", (PyCFunction)gdsnGetFile, METH_VARARGS, NULL },
	{ "diagnosis_gds", (PyCFunction)gdsDiagnosis, METH_VARARGS, NULL },

	// attribute operations
	{ "getattr_gdsn", (PyCFunction)gdsnGetAttr, METH_VARARGS, NULL },
	{ "putattr_gdsn", (PyCFunction)gdsnPutAttr, METH_VARARGS, NULL },
	{ "delattr_gdsn", (PyCFunction)gdsnDeleteAttr, METH_VARARGS, NULL },

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
{
	if (!pygds_init()) return NULL;
#else
PyMODINIT_FUNC initccall()
{
	pygds_init();
#endif

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

#if PY_MAJOR_VERSION >= 3
	return mod;
#endif
}

}
