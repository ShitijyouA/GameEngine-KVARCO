#include "pch.h"
#include "ScriptManager.h"

//コンパイルするが、loaded_scripts_には登録されない
xtal::CodePtr ScriptManager::CompileOneFile_impl(const fsys::path& abs_file_path)
{
	if(!fsys::exists(abs_file_path))
		return xtal::null;
	else
	{
		std::string temp=abs_file_path.string();
		return xtal::compile_file(temp.c_str());
	}
}

xtal::CodePtr ScriptManager::CompileOneFile(const xtal::StringPtr& file_path)
{
	fsys::path abs_path(fsys::absolute(file_path->c_str(),kvarco::ExePath));
	return CompileOneFile_impl(abs_path);
}

//
bool ScriptManager::IsLoaded(const PathType& path,const TimeStampType& time_stamp)
{
	LoadedScriptMap_tag_Path::iterator i;
	const LoadedScriptMap_tag_Path& path_list=loaded_scripts_.get<tag_Path>();
	i=path_list.find(path);

	if(i==path_list.end())
		return false;
	else
		return i->time_stamp_==time_stamp;
}

//
xtal::AnyPtr ScriptManager::LoadOneFile_impl(const fsys::path& abs_file_path)
{
	if(!fsys::exists(abs_file_path)) return xtal::null;

	//check what this was loaded
	const TimeStampType last_time_stamp=fsys::last_write_time(abs_file_path);
	if(IsLoaded(abs_file_path,last_time_stamp)) return xtal::null;

	LoadedFile load_info(abs_file_path,last_time_stamp);

	//compile
	load_info.compiled_code_=CompileOneFile_impl(abs_file_path);

	bool IsFailed=false,IsRetry=false;
	XtalHelper::processExcept(_T("Xtal runtime error on call\n"),&IsFailed,&IsRetry);

	//load
	xtal::AnyPtr res=xtal::null;
	if(!xtal::is_null(load_info.compiled_code_))
		res=XtalHelper::call(load_info.compiled_code_);

	//insert to map
	loaded_scripts_.insert(load_info);

	return res;
}

xtal::AnyPtr ScriptManager::LoadOneFile(const xtal::StringPtr& file_path)
{
	fsys::path abs_path(fsys::absolute(file_path->c_str(),kvarco::ExePath));

	return LoadOneFile_impl(abs_path);
}

//

namespace
{

//キーであるabs_path_が書き換えられることはないので書き換えでの衝突は起き得ない
//速度優先でmodify()を使う
class LoadedFileRewriter
{
	xtal::CodePtr				new_code_;
	LoadedFile::TimeStampType	new_time_stamp_;

public:
	LoadedFileRewriter(const xtal::CodePtr& code,const LoadedFile::TimeStampType& time_stamp)
        :new_code_(code),new_time_stamp_(time_stamp)
	{}

    void operator()(LoadedFile& target) const
    {
		target.compiled_code_	=xtal::null;
		xtal::full_gc();
		target.compiled_code_	=new_code_;

		target.time_stamp_		=new_time_stamp_;
    }
};

}

void ScriptManager::ReLoad_impl(LoadedScriptMap_tag_Seq::iterator i,LoadedScriptMap_tag_Seq& map)
{
	const fsys::path& abs_file_path=(*i).abs_path_;

	if(!fsys::exists(abs_file_path)) return;

	//check what this was loaded
	const TimeStampType last_time_stamp=fsys::last_write_time(abs_file_path);
	if(IsLoaded(abs_file_path,last_time_stamp)) return;

	//compile
	xtal::CodePtr comped_code=CompileOneFile_impl(abs_file_path);

	//load
	comped_code.call();

	//
	bool IsFailed=false,IsRetry=false;
	XtalHelper::processExcept(_T("Xtal runtime error on call\n"),&IsFailed,&IsRetry);

	//apply changes(compiled_code,time_stamp)
	map.modify(i,LoadedFileRewriter(comped_code,last_time_stamp));
}

void ScriptManager::LoadFiles(const xtal::ArrayPtr& file_paths)
{
	for(DWORD i=0; i<file_paths->size(); ++i)
		xtal::AnyPtr res=LoadOneFile(file_paths->op_at(i).to_s());
}

void ScriptManager::ReLoad()
{
	LoadedScriptMap_tag_Seq& seq_list	=loaded_scripts_.get<tag_Seq>();
	LoadedScriptMap_tag_Seq::iterator i	=seq_list.begin();

	for(;i!=seq_list.end(); ++i) ReLoad_impl(i,seq_list);
}

ScriptMngrPtr ScriptManager::GetInst()
{
	static ScriptManager Inst;
	return &Inst;
}

namespace
{
	struct EraserX
	{
		void operator()(LoadedFile& target) const
		{
			target.compiled_code_=xtal::null;
		}
	};
}

void ScriptManager::Release()
{
	LoadedScriptMap_tag_Seq& seq_list	=loaded_scripts_.get<tag_Seq>();
	LoadedScriptMap_tag_Seq::iterator i	=seq_list.begin();

	EraserX eraser;
	for(;i!=seq_list.end(); ++i)
	{
		seq_list.modify(i,eraser);
		i=seq_list.erase(i);
	}
	xtal::full_gc();
}

void ScriptManager::bind(const xtal::ClassPtr it)
{
	USE_XDEFZ(ScriptManager);

	Xdef_method(LoadOneFile);
	Xdef_method(CompileOneFile);
	Xdef_method(LoadFiles);
	Xdef_method(ReLoad);
}