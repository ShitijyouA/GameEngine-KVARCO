#if 0	//at compile as library=> set 0

#include "EncryptedZip.h"
#include "DecryptedZip.h"
#include <cstdio>

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
			,"SomePassword"
			,param
		);
		//enzip+=(SAMPLE_SOUCE_DIR/"Audio");

		enzip.ZipToFile(SAMPLE_DIR/"dst.czg");
	}

	{
		czip::DecryptedZip dezip(SAMPLE_DIR/"dst.czg","SomePassword");
		czip::DecryptedZip::ArchivedFilePtr the_file=dezip.GetFile(fsys::path("Char\\E-1.png"));

		if(the_file->Exists())
		{
			//test OutToFile
			the_file->UnzipToFile(SAMPLE_DIR/"sample.png");

			//test OutToMemory
			FILE* fp;
			fopen_s(&fp,(SAMPLE_DIR/"sample_mem.png").string().c_str(),"wb");

			int size=the_file->RawSize();
			czip::BYTE* dst_buf=new czip::BYTE[size];
			std::fill(dst_buf,dst_buf+size,0);

			the_file->UnzipToMemory(dst_buf,size);

			std::fwrite(dst_buf,sizeof(czip::BYTE),size,fp);

			delete[] dst_buf;
			std::fclose(fp);
		}
	}

	return 0;
}

#endif