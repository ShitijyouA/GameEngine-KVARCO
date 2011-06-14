#include "EncryptedZip.h"
#include "DecryptedZip.h"

namespace czip=kvarco::crypted_zip;
namespace fsys=boost::filesystem;

const fsys::path SAMPLE_SOUCE_DIR("..\\..\\NOW_HERE-in_KVARCO\\Resouce");
const fsys::path SAMPLE_DIR("..\\SampleFiles\\");

int main()
{
	{
		czip::EncryptedZip::CompresserParamType param(9); // --best
		czip::EncryptedZip enzip
		(
			SAMPLE_SOUCE_DIR/"Graph"/"Char"
			,true
			,param
			,czip::logger::StdLogger()	//TODO : ‚ß‚ñ‚Ç‚¤‚È‚Ì‚Å‚Ç‚¤‚É‚©‚·‚é
		);
		//enzip+=(SAMPLE_SOUCE_DIR/"Audio");

		enzip.OutToFile(SAMPLE_DIR/"dst.czg");
	}

	{
		czip::DecryptedZip dezip(SAMPLE_DIR/"dst.czg",123456);
		czip::DecryptedZip::ArchivedFilePtr the_file=dezip.GetFile(fsys::path("Char\\E-1.png"));

		if(the_file->Exists())
			the_file->OutToFile(SAMPLE_DIR/"sample.png");
	}

	return 0;
}