#include "CryptedZip.h"

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
			SAMPLE_SOUCE_DIR/"Graph"
			,true
			,param
		);
		//enzip+=(SAMPLE_SOUCE_DIR/"Audio");

		enzip.OutToFile(SAMPLE_DIR/"dst.czg");
	}

	return 0;
}