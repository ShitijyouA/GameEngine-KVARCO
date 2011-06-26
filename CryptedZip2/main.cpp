#ifdef AS_EXEC	//at compile as library=> set 0

#include "EncryptedZip.h"
#include "DecryptedZip.h"
#include <cstdio>

namespace czip=kvarco::crypted_zip;
namespace fsys=boost::filesystem;

const fsys::path SAMPLE_SOUCE_DIR("..\\..\\NOW_HERE-in_KVARCO\\Resouce");
const fsys::path SAMPLE_DIR("..\\");

int main(int argc,char** argv)
{
#if 1
	{
		if(argc==2)
		{
			czip::EncryptedZip::CompresserParamType param(9); // --best
			czip::EncryptedZip enzip
			(
				argv[1]
				,"\x18\x88\x4E\x18\x8B\xC6\xC3\x8B\x07\x89\x06\x8B\x4F\x04\x53\x89\x4E\x04\x8B\x5E\x08\x85\xDB\x74\x14\x53\xFF\x15\x68\x43\x4D\x85"
				,param
			);
			//for(int i=1; i<argc; ++i) enzip+=(argv[i]);

			enzip.ZipToFile("dst.kcz");
		}
	}
#endif

#if 0
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
#endif

	return 0;
}

#endif