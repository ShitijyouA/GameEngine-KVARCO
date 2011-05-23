#include "CryptedZip.h"

namespace czip=kvarco::crypted_zip;
namespace fsys=boost::filesystem;

const fsys::path SAMPLE_DIR("..\\SampleFiles\\");

int main()
{
	{
		czip::EncryptedZip enzip
		(
			"C:\\Documents and Settings\\KiiMasanobu\\My Documents\\Projects\\NOW_HERE-in_KVARCO\\Resouce\\Graph\\Char"
			,false
		);

		enzip.OutToFile(SAMPLE_DIR/"dst.czg");
	}

	return 0;
}