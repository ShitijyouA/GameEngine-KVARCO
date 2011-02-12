#include "pch.h"
#include "CryptedZip.h"

using namespace KVARCO;
using namespace KVARCO::CryptedZip;
using namespace KVARCO::CryptedZip::Detail;
using namespace KVARCO::CryptedZip::Header;

PathAndParent KVARCO::CryptedZip::MakePathPair(path& file_path)
{
	return PathAndParent(file_path,path());
}

namespace boost
{
	namespace filesystem3
	{
		size_t hash_value(const path& data)
		{
			size_t h=boost::hash_value(data.native());
			return h;
		}
	}
}

const BYTE ROTATE_BITS	=5;		//set num which is smaller than 8
const char* HEADER		="GCZ";
const path_char* ErrorMes_FileOpen=TEXT_PATH("could not open this file : ");

//CBaseCryptedZip
CBaseCryptedZip::CBaseCryptedZip(const bool enable_crypt)
	:UniformDWORD(0,UINT_MAX),					UniformBYTE(0,UCHAR_MAX),
	 DiceDWORD(RandomGenerator,UniformDWORD),	DiceBYTE(RandomGenerator,UniformBYTE),
	 EnableCrypt(enable_crypt)
{
	//init by zero
	PrevCiper			=0;
	MeaninglessIndex	=0;
	ElapsedByte			=0;
}

void CBaseCryptedZip::InitByKey(DWORD key)
{
	RandomGenerator.seed(key);

	//truncat to make sure (at least 7 times)
	int truncat;
	truncat=DiceDWORD()&0xffff	+7;
	for(int i=0; i<truncat; i++) DiceDWORD();

	truncat=DiceBYTE()&0xffff	+7;
	for(int i=0; i<truncat; i++) DiceBYTE();

	//make put intvervals meaningless char(at least 1)
	memset(MeaninglessChar,0,sizeof(MeaninglessChar));
	for(DWORD i=0; i<MEANINGLESS_NUM; i++)
		MeaninglessChar[i]=DiceBYTE()+1;
}

//CEncryptedZip
void CEncryptedZip::AddFilesFromDirPath_(const path& dir,const path& parent_dir)
{
	directory_iterator end;
	for(directory_iterator di(dir); di!=end; ++di)
	{
		if(!is_directory(*di))
			InputPathList.push_back(make_pair(di->path(),parent_dir));
		else
			AddFilesFromDirPath_(di->path(),parent_dir);
	}
}

CEncryptedZip::CEncryptedZip(PathList path_list,BYTE rotate_bits,DWORD xor_key,DWORD key,bool enable_crypt)
	:RotateBits(rotate_bits%8),XorKey(xor_key),
	CBaseCryptedZip(enable_crypt),InputPathList(path_list)
{
	//BOOST_FOREACH(PathAndParent& i,InputPathList)
	for(PathList::iterator i=InputPathList.begin(); i!=InputPathList.end(); )
	{
		if(is_directory(i->first))
		{
			//this is directory path
			const path_string& ps=i->first.native();
			if(ps[ps.size()-1]!='\\') i->first/="\\";

			path parent;
			for(path::iterator pi=i->first.begin(); pi!=--(--i->first.end()); ++pi)
				parent/=*pi;
			AddFilesFromDirPath_(i->first,parent);

			i=InputPathList.erase(i);
			continue;
		}
		else
		{
			Header::CentralHeader tmp;
			tmp.CRC			=0;
			tmp.FilePos		=0;

			if(EnableCrypt)	tmp.Key=DiceDWORD();
			else			tmp.Key=0;

			tmp.NameLen		=i->first.string().size();
			tmp.Name		=const_cast<path_char*>(i->first.c_str());

			CentralHeaderList.push_back(make_pair(tmp,i->second));
		}

		 ++i;
	}

	Header.Header=HEADER;
}

void CEncryptedZip::InitPrevCiper()
{
	//set initial vector for CBC mode
	PrevCiper=(DiceBYTE()^0xb6)^(DiceBYTE()^0x49);
}

BYTE CEncryptedZip::Encrypt1Byte_(BYTE x,BYTE prev_x)
{
	x^=prev_x;												//CBC mode

	BYTE cipher=Detail::BitLeftRotate<BYTE>(x,RotateBits);	//bit rotate
	BYTE rand_byte=DiceBYTE();
	cipher^=rand_byte;										//XOR with random byte

	return cipher;
}

DWORD CEncryptedZip::EncryptStream_(BYTE* buffer,BYTE* stream,DWORD length)
{
	//main loop
	DWORD res_len=0;
	for(DWORD i=0; i<length; ++i)
	{
		if(ElapsedByte==MeaninglessChar[MeaninglessIndex])
		{
			buffer[res_len]=DiceBYTE();

			ElapsedByte=0;
			++MeaninglessIndex;
			if(MeaninglessIndex>=MEANINGLESS_NUM) MeaninglessIndex=0;

			--i;
		}
		else
			buffer[res_len]=Encrypt1Byte_(stream[i],PrevCiper);

		PrevCiper=buffer[res_len];

		++ElapsedByte;
		++res_len;
	}

	return res_len;
}

void CEncryptedZip::OutputToFile(path out_path)
{
	//open output file
	FILE* output_file;
	fopen_s_cz(&output_file,out_path.c_str(),TEXT_PATH("wb"));
	if(!output_file) throw bad_exception();

	BOOST_FOREACH(HeaderAndParent& i,CentralHeaderList)
	{
		//open input file
		FILE* input_file;
		fopen_s_cz(&input_file,i.first.Name,TEXT_PATH("rb"));
		if(!input_file) throw bad_exception();

		//set FilePos
		i.first.FilePos=ftell(output_file);

		//init for CRC32
		crc_32_type crc_result;

		//init for crypting
		if(EnableCrypt)
		{
			InitByKey(i.first.Key);
			InitPrevCiper();
			fwrite(&PrevCiper,sizeof(BYTE),1,output_file);
		}

		DWORD begin_write_pos=ftell(output_file);
		//copy & crypting
		while(!feof(input_file))
		{
			BYTE buf[BUFFER_SIZE];
			DWORD num=fread(buf,sizeof(char),BUFFER_SIZE,input_file);
			if(!num) break;
			crc_result.process_bytes(buf,num);

			if(EnableCrypt)
			{
				BYTE res[BUFFER_SIZE+MEANINGLESS_NUM];
				DWORD res_num=EncryptStream_(res,buf,sizeof(BYTE)*num);
				fwrite(res,sizeof(BYTE),res_num,output_file);
			}
			else
				fwrite(buf,sizeof(BYTE),num,output_file);
		}
		i.first.ZipedFileSize	=ftell(output_file)-begin_write_pos;
		i.first.CRC				=crc_result.checksum();

		fclose(input_file);
	}

	//ready for writing CentralHeader
	Header.CentralPos=ftell(output_file);

	//write CentralHeader
	BOOST_FOREACH(HeaderAndParent& i,CentralHeaderList)
	{
		path		tmp_path(i.first.Name);
		path		filename(tmp_path.filename());
		path_string full_path=i.first.Name;
		path_string	rlt_path;
		
		if(!i.second.empty())
		{
			rlt_path		=full_path.substr(i.second.native().size());
			i.first.Name	=const_cast<path_char*>(rlt_path.c_str());
			i.first.NameLen	=rlt_path.size();
		}
		else
		{
			i.first.Name	=const_cast<path_char*>(filename.c_str());
			i.first.NameLen	=filename.native().size();
		}

		i.first.Key^=XorKey;
		fwrite(&i,sizeof(CentralHeader),1,output_file);
		fwrite(i.first.Name,sizeof(path_char),i.first.NameLen+1,output_file);
	}

	//add TerminalHeader
	Header.FileNum=InputPathList.size();
	fwrite(&Header,sizeof(TerminalHeader),1,output_file);

	//close output file
	fclose(output_file);
}

CDecryptedUnZip::CDecryptedUnZip(path path,BYTE rotate_bits,DWORD xor_key,bool enable_crypt)
	:RotateBits(rotate_bits%8),XorKey(xor_key),
	CBaseCryptedZip(enable_crypt),InputFilePath(path)
{
	fopen_s_cz(&InputFile,InputFilePath.c_str(),TEXT_PATH("rb"));

	fseek(InputFile,-static_cast<long>(sizeof(TerminalHeader)),SEEK_END);
	fread(&Header,sizeof(TerminalHeader),1,InputFile);
	if(!CheckHeader_(Header))
		throw bad_exception("this is not KVARCO::CryptedZip file");

	ReadCHeaders_();
}

BYTE inline CDecryptedUnZip::Decrypt1Byte_(BYTE x,BYTE prev_x)
{
	BYTE rand_byte=DiceBYTE();
	BYTE tmp1=x^rand_byte;
	BYTE plain=BitRightRotate<BYTE>(tmp1,RotateBits);
	plain^=prev_x;

	return plain;
}

void CDecryptedUnZip::InitPrevCiper_(FILE* file)
{
	fread(&PrevCiper,sizeof(BYTE),1,file);
}

DWORD CDecryptedUnZip::DecryptStream_(BYTE* buffer,BYTE* stream,DWORD length)
{
	//main loop
	DWORD res_len;
	DWORD i;
	for(i=0,res_len=0; i<length; ++i)
	{
		if(ElapsedByte==MeaninglessChar[MeaninglessIndex])
		{
			DiceBYTE();

			ElapsedByte=0;
			++MeaninglessIndex;
			if(MeaninglessIndex>=MEANINGLESS_NUM) MeaninglessIndex=0;

			--i;
			--length;
		}
		else
			buffer[i]=Decrypt1Byte_(stream[res_len],PrevCiper);

		PrevCiper=stream[res_len];

		++ElapsedByte;
		++res_len;
	}

	return i;
}


bool CDecryptedUnZip::CheckHeader_(TerminalHeader& header)
{
	return (strcmp(header.Header,HEADER)==0);
}

void CDecryptedUnZip::ReadCHeaders_()
{
	fseek(InputFile,Header.CentralPos,SEEK_SET);
	for(DWORD i=0; i<Header.FileNum; ++i)
	{
		CentralHeader tmp;
		fread(&tmp,sizeof(CentralHeader),1,InputFile);
		tmp.Name=new path_char[tmp.NameLen+1];
		fread(tmp.Name,sizeof(path_char),tmp.NameLen+1,InputFile);
		CentralHeaderMap.insert(make_pair(tmp.Name,tmp));
	}
}

CHeader_op CDecryptedUnZip::FindFile(path& file_path)
{
	CHeaderMap::iterator i=CentralHeaderMap.find(file_path);
	if(i==CentralHeaderMap.end())	return CHeader_op();
	else							return CHeader_op(i->second);
}

void CDecryptedUnZip::InitDecrypt_(FILE* file,DWORD seed)
{
	//init by zero
	PrevCiper			=0;
	MeaninglessIndex	=0;
	ElapsedByte			=0;

	//init random generator
	InitByKey(seed);

	//turn cut
	DiceBYTE(); DiceBYTE();

	//read PrevCiper from file
	InitPrevCiper_(file);
}

DWORD CDecryptedUnZip::OutputToMemory(BYTE* buffer,DWORD buffer_size,Header::CentralHeader& file_header)
{
	if(buffer_size<file_header.ZipedFileSize) return 0;

	fseek(InputFile,file_header.FilePos,SEEK_SET);

	DWORD num=0;
	if(EnableCrypt)
	{
		InitDecrypt_(InputFile,file_header.Key^XorKey);
		BYTE* buf=new BYTE[file_header.ZipedFileSize];
		DWORD num_tmp=fread(buf,sizeof(BYTE),file_header.ZipedFileSize,InputFile);
		num=DecryptStream_(buffer,buf,num_tmp);
		delete[] buf;
	}
	else
		num=fread(buffer,sizeof(char),file_header.ZipedFileSize,InputFile);

	return num;
}

void CDecryptedUnZip::OutputToSmallFile(path& out_path,CentralHeader& file_header)
{
	FILE* output_file;
	fopen_s_cz(&output_file,out_path.c_str(),TEXT_PATH("wb"));
	BOOST_ASSERT(output_file);

	BYTE* buffer=new BYTE[file_header.ZipedFileSize];
	DWORD num=OutputToMemory(buffer,file_header.ZipedFileSize,file_header);
	fwrite(buffer,sizeof(BYTE),num,output_file);

	fclose(output_file);
	delete[] buffer;
}

void CDecryptedUnZip::OutputToFile(path out_path,Header::CentralHeader& file_header)
{
	/*
	FILE* output_file;
	fopen_s_cz(&output_file,out_path.c_str(),TEXT_PATH("wb"));
	BOOST_ASSERT(output_file);

	BYTE buffer[BUFFER_SIZE];
	DWORD rest_bytes=file_header.ZipedFileSize;
	while(rest_bytes)
	{
		rest_bytes
	}
	
	fwrite(buffer,sizeof(BYTE),num,output_file);

	fclose(output_file);
	delete[] buffer;
	*/
}

CDecryptedUnZip::~CDecryptedUnZip()
{
	fclose(InputFile);
}