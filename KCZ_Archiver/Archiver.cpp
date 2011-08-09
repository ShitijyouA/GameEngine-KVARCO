#include "Archiver.h"

//global var
EncryptedZipPtr encrypter;

//global constants
const std::string XTAL_EXT(".xtal");
const std::string COMPILED_XTAL_EXT(".xtalc");
const std::string DST_FILE_NAME("tmp_compiled_xtal");
const fsys::path TMP_DIR(".\\tmp\\");

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

	PathListType src_pathz;
	for(int32_t i=1; i<argc; ++i) src_pathz.push_back(argv[i]);

	InitXtal();						logger("init xtal.");
	InitEncryptedZip(src_pathz);	logger("init EncryptedZip.");

	uint32_t xtal_file_num=0;
	PathListType compiled_script_pathz;
	BOOST_FOREACH(fsys::path& i,src_pathz)
	{
		if(i.extension()==XTAL_EXT)
		{
			if(!fsys::exists(TMP_DIR)) fsys::create_directory(TMP_DIR);

			std::string dts_file_name=
				DST_FILE_NAME+boost::lexical_cast<std::string>(xtal_file_num)+COMPILED_XTAL_EXT;
			fsys::path tmp_file_path=TMP_DIR/dts_file_name;

			if(!CompileXtalScript(i,tmp_file_path)) goto end;
			encrypter->AddPackFile(tmp_file_path);

			compiled_script_pathz.push_back(tmp_file_path);
			++xtal_file_num;
		}
		else
			encrypter->AddPackFile(i);
	}

	//get dst path
	//scope for measures for error C2363
	{
		std::string dst_path;
		std::cout<<"Please input destination path =>"<<std::flush;
		std::cin>>dst_path;
		std::cout<<std::endl;

		if(!encrypter->ZipToFile(dst_path))
			logger("faile to compression/output files.");
	}

end:
	fsys::remove_all(TMP_DIR);
	xtal::uninitialize();

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

void InitEncryptedZip(PathListType& src_pathz)
{
	//get password
	std::string password;
	std::cout<<"Please input password =>"<<std::flush;
	std::cin>>password;
	std::cout<<std::endl;

	//init EncryptedZip
	//æ‚É“Ë‚Áž‚ñ‚Å‚¨‚­
	kcz::EncryptedZip::CompresserParamType param(9); // --best
	for(PathListType::iterator i=src_pathz.begin(); i!=src_pathz.end(); ++i)
	{
		if(i->extension()!=XTAL_EXT)
		{
			encrypter.reset(new kcz::EncryptedZip(*i,password,param));
			src_pathz.erase(i);
			break;
		}
	}
}

bool CompileXtalScript(const fsys::path& src_path,const fsys::path& dst_path)
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