#include "Archiver.h"

//global var
EncryptedZipPtr encrypter;

//global constants
const fsys::path XTAL_EXT(".xtal");
const fsys::path COMPILED_XTAL_EXT(".xtalc");
const fsys::path TMP_DIR("kcz_archiver_tmp\\");						//すでに存在する/しないにかかわらず、最後にまるごと消える。

int main(int argc,const char** argv)
{
	using boost::int32_t;
	using boost::uint32_t;
	using boost::uint16_t;
	using boost::uint8_t;

	kcz::logger::StdLogger logger;

	if(argc<=1)
	{
		logger("Error : NoFilePath : Please input file path.");
		return 0;
	}
	//<2011-08-12> 現在、Xtalスクリプトとそれ以外のファイルが混ざっていると停止する。この時点では仕様。

	PathListType src_pathz;
	std::cout<<"Is this xtal script files? (Y/N) : "<<std::flush;
	char is_xtal_raw=0;
	std::cin>>is_xtal_raw;
	std::cout<<std::endl;
	bool is_xtal=(is_xtal_raw=='Y' || is_xtal_raw=='y');

	for(int32_t i=1; i<argc; ++i)
	{
		if(is_xtal)
			InsertAllFile(argv[i],src_pathz);
		else
			src_pathz.push_back(std::make_pair(argv[i],""));
	}

	InitXtal();			logger("init xtal.");
	InitEncryptedZip();	logger("init EncryptedZip.");

	//TODO
	//*<
	try
	{

	if(is_xtal)
	{
		if(!CompileAllXtalScript(src_pathz,TMP_DIR)) goto end;
		encrypter->AddPackFile(TMP_DIR);
	}
	else
	{
		BOOST_FOREACH(AbsAndRltPathType& i,src_pathz)
		{
			encrypter->AddPackFile(i.first);
		}
	}
	//*>

	//get dst path
	//scope for measures for error C2363
	{
		std::string dst_path;
		std::cout<<"Please input destination path =>"<<std::flush;
		std::getline(std::cin,dst_path);
		std::cout<<std::endl;

		if(!encrypter->ZipToFile(dst_path))
			logger("faile to compression/output files.");
	}

	}
	catch(std::runtime_error e)
	{
		std::cout<<"Error : Boost::Filesystem : happened error at "<<e.what()<<std::endl;
	}

end:
	fsys::remove_all(TMP_DIR);
	xtal::uninitialize();

	std::cout<<"finish!"							<<std::endl
			 <<"waiting now... please press enter"	<<std::endl;
	std::getchar();
	return 0;
}


void InitXtal()
{
	static xtal::CStdioStdStreamLib cstdStdStreamLib;
	static xtal::WinThreadLib winThreadLib;
	static xtal::WinFilesystemLib winFilesystemLib;
	static xtal::SJISChCodeLib chCodeLib;

	xtal::Setting setting;
	setting.thread_lib		=&winThreadLib;
	setting.std_stream_lib	=&cstdStdStreamLib;
	setting.filesystem_lib	=&winFilesystemLib;
	setting.ch_code_lib		=&chCodeLib;

	xtal::initialize(setting);
	xtal::bind_error_message();
}

void InitEncryptedZip()
{
	//get password
	if(!std::cin.eof()) std::cin.ignore();
	std::string password;
	std::cout<<"Please input password =>"<<std::flush;
	std::getline(std::cin,password);
	std::cout<<std::endl;

	//init EncryptedZip
	kcz::EncryptedZip::CompresserParamType param(9); // --best
	encrypter.reset(new kcz::EncryptedZip(password,param));
}

//author : Martin Lutken (Thunks!)
//from : http://lists.boost.org/Archives/boost/2007/10/127976.php
boost::filesystem::path 
remove_from_start( 
    const boost::filesystem::path& inPath, ///< [in] Path to remove start of path from 
    const boost::filesystem::path& remPath ///< [in] Path to remove from inPath 
                  ) 
{ 
    namespace fs = boost::filesystem; 
         
    fs::path::iterator itIn = inPath.begin(); 
    fs::path::iterator itRem = remPath.begin(); 
    if ( *itIn != *itRem ) 
    { 
        // assert(*itIn == *itRem ); 
        return inPath; // If path do not have same root, just return inPath !!! 
    } 
    while ((itIn != inPath.end()) && (itRem != remPath.end()) && (*itIn == *itRem) ) 
    { 
        ++itIn; 
        ++itRem; 
    } 
    fs::path outPath; 
    for ( ;itIn != inPath.end(); ++itIn ) 
    { 
        outPath /= *itIn; // fs::native so we can cope with spaces in file names 
    } 
    return outPath; 
}

void InsertAllFile(const fsys::path& parent_dir,PathListType& path_listz)
{
	if(!fsys::is_directory(parent_dir))
	{
		path_listz.push_back(std::make_pair(parent_dir,parent_dir));
		return;
	}
	
	fsys::recursive_directory_iterator ite(parent_dir);
	fsys::recursive_directory_iterator end;
	for(; ite!=end; ++ite)
	{
		if(fsys::is_directory(*ite)) continue;

		fsys::path rlt_path=remove_from_start(*ite,parent_dir);
		path_listz.push_back(std::make_pair(*ite,rlt_path));
	}
}

bool CompileOneXtalScript(const fsys::path& src_path,const fsys::path& dst_path)
{
	xtal::CodePtr code=xtal::compile_file(src_path.string().c_str());
	if(!code)
	{
		std::cout<<"Error : CompileError : faile to compile "<<src_path.string()<<std::endl;
		return false;
	}

	//serialize byte code
	xtal::StreamPtr fs=
		xtal::xnew<xtal::FileStream>(dst_path.string().c_str(),"w");

	fs->serialize(code);
	fs->close();

	return true;
}

bool CompileAllXtalScript(PathListType& path_listz,const fsys::path& dst_dir)
{
	BOOST_FOREACH(AbsAndRltPathType& i,path_listz)
	{
		if(!fsys::exists(dst_dir))	fsys::create_directory(TMP_DIR);
		fsys::path parent=dst_dir/i.second.parent_path();
		if(!fsys::exists(parent))	fsys::create_directories(parent);

		fsys::path dst_file_name=parent/i.second.filename();
		dst_file_name.replace_extension(COMPILED_XTAL_EXT);

		if(!CompileOneXtalScript(i.first,dst_file_name)) return false;
	}
	return true;
}

