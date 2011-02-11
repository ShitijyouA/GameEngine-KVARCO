#include "pch.h"
#include "ScManager.h"

//==でtrue,!=でfalse

optional<CLoadedFile> CScriptManager::FindLoadedFile(string path) const
{
	ScMap_tag_Path::iterator i;
	const ScMap_tag_Path& path_list=ScriptFiles.get<tag_Path>();
	i=path_list.find(path);
	if(i==path_list.end()) return optional<CLoadedFile>();
	return optional<CLoadedFile>((*i));
}

bool CScriptManager::IsLoaded(const string path,const FILETIME& time_stamp)const
{
	optional<CLoadedFile> ploaded_file=FindLoadedFile(path);
	if(!ploaded_file)	return false;

	FILETIME& ptime_stamp=ploaded_file->TimeStamp;
	if(ptime_stamp.dwLowDateTime	!=time_stamp.dwLowDateTime	||
		ptime_stamp.dwHighDateTime	!=time_stamp.dwHighDateTime) return false;
	return true;
}

xtal::AnyPtr CScriptManager::LoadOneFile(const xtal::StringPtr& file_name)
{
	string path=KVARCO::ExePath;
	path.append(file_name->c_str());

	optional<FILETIME> time_stamp=GetTimeStamp(path);
	if(!time_stamp)	return xtal::null;

	xtal::AnyPtr res;
	if(!IsLoaded(path,*time_stamp))
	{
		res=xtal::load(path.c_str());
		bool IsFailed=false,IsRetry=false;
		XtalHelper::processExcept(_T("Xtal runtime error on call\n"),&IsFailed,&IsRetry);
	}

	ScriptFiles.insert(CLoadedFile(file_name->c_str(),*time_stamp));

	return res;
}

xtal::CodePtr CScriptManager::CompileOneFile(const xtal::StringPtr& FileName)
{
	string path=KVARCO::ExePath;
	path.append(FileName->c_str());

	optional<FILETIME> time_stamp=GetTimeStamp(path);
	if(!time_stamp)	return xtal::null;

	xtal::CodePtr comped;
	if(IsLoaded(path,*time_stamp)!=0)
	{
		comped=xtal::compile(path.c_str());
	}

	return comped;
}

optional<FILETIME> CScriptManager::GetTimeStamp(const string path) const
{
	//タイムスタンプ獲得
	HANDLE file;
	FILETIME time_stamp;

	//ファイルを開く
	file=CreateFile(path.c_str(),
		GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(file==INVALID_HANDLE_VALUE){
		assert("ファイルが開けません。\n");
		return optional<FILETIME>();
	}

	GetFileTime(file,NULL,NULL,&time_stamp);
	CloseHandle(file);

	return optional<FILETIME>(time_stamp);
}

void CScriptManager::LoadFiles(const xtal::ArrayPtr& FileNames)
{
	bool isFailed = false, isRetry = false;
	for(DWORD i=0; i<FileNames->size(); i++)
	{
		xtal::AnyPtr res=LoadOneFile(FileNames->op_at(i).to_s());
		XtalHelper::processExcept(_T("Xtal load error: \n"),&isFailed,&isRetry);
	}
}

void CScriptManager::ReLoad()
{
	ScMap_tag_Seq& seq_list=ScriptFiles.get<tag_Seq>();
	ScMap_tag_Seq::iterator i=	seq_list.begin();

	for(;i!=seq_list.end(); i++)
	{
		string tmp_path=KVARCO::ExePath;
		tmp_path.append((*i).RelaPath.c_str());

		optional<FILETIME> tmp_ts=GetTimeStamp(tmp_path);
		if(!tmp_ts) continue;

		if(IsLoaded(tmp_path.c_str(),*tmp_ts))
			LoadOneFile((*i).RelaPath.c_str());
	}
}

#ifdef COMPLATE_CODING_SERIALIZE
bool CScriptManager::SaveByteCode(string rela_path)
{
	ScMap_tag_Seq& seq_list=	ScriptFiles.get<tag_Seq>();
	ScMap_tag_Seq::iterator i=	seq_list.begin();
	ScMap_tag_Seq::iterator end=seq_list.end();

	string path=KVARCO::ExePath;
	path.append(rela_path.c_str());

	xtal::SmartPtr<FileStream> fsp = xtal::xnew<xtal::FileStream>(path.c_str(), "w");
	for(;i!=end; i++)
	{
		path=KVARCO::ExePath;
		path.append((*i).RelaPath.c_str());

		xtal::CodePtr code=CompileOneFile(path.c_str());
		if(!xtal::is_null(code))	fsp->serialize(code);
	}
}

xtal::CodePtr CScriptManager::ReadByteCode(string rela_path)
{
	string path=KVARCO::ExePath;
	path.append(rela_path.c_str());

	xtal::SmartPtr<FileStream> fsp = xtal::xnew<xtal::FileStream>(path.c_str(), "r");
	return xtal::ptr_cast<xtal::Code>(fsp->deserialize());
}

bool CScriptManager::LoadByteCode(string rela_path)
{
	xtal::CodePtr code=ReadByteCode(rela_path);
	if(!xtal::is_null(code))	code->call();
}
#endif

//ScriptMngrPtr CScriptManager::Inst(xtal::null);
//ScriptMngrPtr CScriptManager::Inst=NULL;
ScriptMngrPtr CScriptManager::GetInst()
{
	//if(xtal::is_null(Inst))
	//	CScriptManager::Inst=xtal::xnew<CScriptManager>();
	//if(Inst==NULL)	CScriptManager::Inst=new CScriptManager;

	static CScriptManager Inst;
	return &Inst;
}

void CScriptManager::bind(const xtal::ClassPtr it)
{
	//xtal::lib()->def(Xid(LoadOneFile),xtal::fun(LoadOneFile));

	BIND_XTAL_CLASSFUN_DEFNAME_IT(CScriptManager,LoadOneFile);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CScriptManager,CompileOneFile);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CScriptManager,LoadFiles);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CScriptManager,ReLoad);
}