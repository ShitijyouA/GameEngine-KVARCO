#pragma once
#include "KVARCO.h"
#include "XtalHelper.h"
#include "Utillity.h"
#include "Tag.h"

//#define COMPLATE_CODING_SERIALIZE	//シリアライズ周りが完了したらdefine

/// ロード済ファイルの情報
struct LoadedFile
{
	typedef fsys::path	PathType;
	typedef std::time_t	TimeStampType;

	PathType		abs_path_;		//絶対パス
	TimeStampType	time_stamp_;	//最後に書きこまれた時間
	xtal::CodePtr	compiled_code_;	//コンパイルしたスクリプト

	LoadedFile(PathType abs_path,TimeStampType time_stamp)
		:	abs_path_(abs_path),
			time_stamp_(time_stamp),
			compiled_code_(xtal::null)
	{}

	LoadedFile();
};

typedef boost::multi_index::multi_index_container
	<
		LoadedFile,
		indexed_by
		<
			ordered_unique<	kvarco::tag::detail::Path,member<LoadedFile,LoadedFile::PathType,&LoadedFile::abs_path_> >,
			sequenced<		kvarco::tag::detail::Sequence >
		>
	>
LoadedScriptMap;

class ScriptManager;
typedef ScriptManager* ScriptMngrPtr;

class ScriptManager
{
public:
	typedef LoadedFile::PathType		PathType;
	typedef LoadedFile::TimeStampType	TimeStampType;

private:
	typedef LoadedScriptMap::index<kvarco::tag::Path>		::type	LoadedScriptMap_tag_Path;
	typedef LoadedScriptMap::index<kvarco::tag::Sequence>	::type	LoadedScriptMap_tag_Seq;

	LoadedScriptMap loaded_scripts_;

	bool IsLoaded(const PathType& path,const TimeStampType& time_stamp);

	xtal::CodePtr CompileOneFileFromArchive(const fsys::path& abs_file_path);
	xtal::CodePtr CompileOneFile_impl(const fsys::path& abs_file_path);
	xtal::AnyPtr LoadOneFile_impl(const fsys::path& abs_file_path);
	void ReLoad_impl(LoadedScriptMap_tag_Seq::iterator i,LoadedScriptMap_tag_Seq& map);

public:
	xtal::AnyPtr LoadOneFile(const xtal::StringPtr& file_path);		//未読み込みのファイルなら1つロード
	xtal::CodePtr CompileOneFile(const xtal::StringPtr& file_path);	//1つコンパイル
	void LoadFiles(const xtal::ArrayPtr& file_paths);				//いっぱいロード
	void ReLoad();													//更新されたファイルをリロード

#ifdef COMPLATE_CODING_SERIALIZE
	//バイトコード保存・読み込み
	bool SaveByteCode(PathType rela_path);			//すべてスクリプトファイルを読み込んでから実行
	xtal::CodePtr ReadByteCode(PathType rela_path);
	bool LoadByteCode(PathType path);
#endif

	static void bind(const xtal::ClassPtr it);
	void Release();
	SINGLETON_PATTERNX(ScriptManager,ScriptMngrPtr)
};
