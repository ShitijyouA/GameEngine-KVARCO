#pragma once
#include "pch.h"

//cryption's parametar
const DWORD KEY_LENGTH		=SHA256_BIN_BUFFER_SIZE;
const DWORD MEANINGLESS_NUM	=128;
extern const char* HEADER;

//for path
#ifdef BOOST_WINDOWS_API
	#ifndef fopen_s_cz
		#define fopen_s_cz				_wfopen_s
	#endif

	#ifndef TEXT_PATH
		#define TEXT_PATH(text)			L##text
	#else
		#define TEXT_PATH(text)
	#endif
#endif
typedef filesystem3::path::value_type	path_char;
typedef filesystem3::path::string_type	path_string;

namespace KVARCO
{
namespace CryptedZip
{
	namespace Detail
	{
		//utillity inline function
		template<typename T>
		T inline BitLeftRotate(T x,BYTE rotate)
		{
			return (x<<rotate | x>>((sizeof(T)*CHAR_BIT)-rotate));
		}

		template<typename T>
		T inline BitRightRotate(T x,BYTE rotate)
		{
			return (x>>rotate | x<<(sizeof(T)*CHAR_BIT-rotate));
		}
	}

	//header
	namespace Header
	{
		struct TerminalHeader
		{
			const char*	Header;

			DWORD		FileNum;
			DWORD		CentralPos;
		};

		struct CentralHeader
		{
			DWORD	FilePos;
			DWORD	ZipedFileSize;
			DWORD	Key;
			BYTE	Hash[SHA256_BIN_BUFFER_SIZE];

			DWORD		NameLen;
			path_char*	Name;
		};

		typedef pair<CentralHeader,path>	HeaderAndParent;

		const DWORD BUFFER_SIZE			=(1<<10)*10;	//10KiB
		extern const path_char* ErrorMes_FileOpen;
	}

	typedef pair<path,path>				PathAndParent;
	PathAndParent MakePathPair(path& file_path);

	typedef list<PathAndParent>							PathList;
	typedef list<Header::HeaderAndParent>				CHeaderList;
	typedef unordered_map<path,Header::CentralHeader>	CHeaderMap;

	class CBaseCryptedZip
	{
	protected:
		const bool EnableCrypt;

		//for zipping
		Header::TerminalHeader	Header;

		//for crypting
		mt19937				RandomGenerator;

		uniform_int<DWORD>	UniformDWORD;
		typedef variate_generator<mt19937&,uniform_int<DWORD> >	mt_DWORD_gen;
		mt_DWORD_gen		DiceDWORD;

		uniform_int<BYTE>	UniformBYTE;
		typedef variate_generator<mt19937&,uniform_int<BYTE> >	mt_BYTE_gen;
		mt_BYTE_gen			DiceBYTE;

		BYTE		PrevCiper;

		BYTE		MeaninglessChar[MEANINGLESS_NUM];
		DWORD		MeaninglessIndex;
		BYTE		ElapsedByte;

		void InitByKey(DWORD key);

	public:
		CBaseCryptedZip(const bool enable_crypt=true);

		virtual void OutputToFile(path out_path)	{}
		virtual DWORD OutputToMemory(void* buffer)	{ return 0; }

		virtual ~CBaseCryptedZip()					{}
	};

	class CEncryptedZip : public CBaseCryptedZip
	{
		CHeaderList	CentralHeaderList;
		PathList	InputPathList;
		void AddFilesFromDirPath_(const path& dir,const path& parent_dir);

	protected:
		BYTE inline Encrypt1Byte_(BYTE x,BYTE prev_x);

		void InitPrevCiper();
		DWORD EncryptStream_(BYTE* buffer,BYTE* stream,DWORD length);

	public:
		CEncryptedZip(PathList path_list,DWORD key,bool enable_crypt=true);
		void OutputToFile(path out_path);
	};

	typedef optional<Header::CentralHeader> CHeader_op;
	class CDecryptedUnZip : public CBaseCryptedZip
	{
		path InputFilePath;
		FILE* InputFile;

		CHeaderMap CentralHeaderMap;

	protected:
		BYTE inline Decrypt1Byte_(BYTE x,BYTE prev_x);

		void InitPrevCiper_(FILE* file);
		void InitDecrypt_(FILE* file,DWORD seed);
		DWORD DecryptStream_(BYTE* buffer,BYTE* stream,DWORD length);

		bool CheckHeader_(Header::TerminalHeader& header);
		void ReadCHeaders_();

	public:
		CDecryptedUnZip(path path,bool enable_crypt=true);

		CHeader_op FindFile(path& file_path);
		void OutputToSmallFile(path& out_path,Header::CentralHeader& file_header);	//directory path
		void OutputToFile(path out_path,Header::CentralHeader& file_header);
		DWORD OutputToMemory(BYTE* buffer,DWORD buffer_size,Header::CentralHeader& file_header);

		~CDecryptedUnZip();
	};

}
}