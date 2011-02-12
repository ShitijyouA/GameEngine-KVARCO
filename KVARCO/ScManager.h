#pragma once
#include "KVARCO.h"
#include "XtalHelper.h"
#include "Utillity.h"

class CLoadedFile;
class CScriptManager;

//#define COMPLATE_CODING_SERIALIZE	//シリアライズ周りが完了したら

class CLoadedFile
{
public:
	string		RelaPath;
	FILETIME	TimeStamp;

	CLoadedFile(string rela_path,FILETIME time_stamp)
		:RelaPath(rela_path),TimeStamp(time_stamp)
	{}
	CLoadedFile();
};

struct tag_Path {};
struct tag_Seq {};

typedef multi_index_container
	<
		CLoadedFile,
		indexed_by
		<
			ordered_unique<tag<tag_Path>,member<CLoadedFile,string,&CLoadedFile::RelaPath> >,
			sequenced<tag<tag_Seq> >
		>
	> ScMap;

typedef ScMap::index<tag_Path>::type	ScMap_tag_Path;
typedef ScMap::index<tag_Seq>::type		ScMap_tag_Seq;
//typedef xtal::SmartPtr<CScriptManager> ScriptMngrPtr;
typedef CScriptManager* ScriptMngrPtr;

class CScriptManager
{
	ScMap ScriptFiles;
	optional<CLoadedFile> FindLoadedFile(string path) const;
	bool IsLoaded(const string path,const FILETIME& time_stamp) const;
	optional<FILETIME> GetTimeStamp(string path) const;

public:
	
	xtal::AnyPtr LoadOneFile(const xtal::StringPtr& file_name);
	xtal::CodePtr CompileOneFile(const xtal::StringPtr& file_name);
	void LoadFiles(const xtal::ArrayPtr& file_names);
	void ReLoad();

#ifdef COMPLATE_CODING_SERIALIZE
	//バイトコード保存・読み込み
	bool SaveByteCode(string rela_path);	//すべてスクリプトファイルを読み込んでから実行
	xtal::CodePtr ReadByteCode(string rela_path);
	bool LoadByteCode(string path);
#endif

	static void bind(const xtal::ClassPtr it);
	SINGLETON_PATTERNX(CScriptManager,ScriptMngrPtr)
};