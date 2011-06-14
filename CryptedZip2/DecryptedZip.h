//Boost! Boost! Boost!

#ifndef KVARCO_CRYPTED_ZIP_DECRYPTED_ZIP_H_
#define KVARCO_CRYPTED_ZIP_DECRYPTED_ZIP_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif

#include "config.h"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/checked_delete.hpp>

#ifdef KVARCO_CRYPTED_ZIP_USE_ZLIB
#	include <boost/iostreams/filter/zlib.hpp>
#else
#	include <boost/iostreams/filter/gzip.hpp>
#endif

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/copy.hpp>

//#include <boost/iostreams/seek.hpp>
//#include <boost/iostreams/read.hpp>
//#include <boost/iostreams/write.hpp>

#ifndef BOOST_FILESYSTEM_VERSION
#	define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/flyweight.hpp>

#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>

#include "detail.h"
#include "Logger.h"
#include "Header.h"

namespace kvarco
{
namespace crypted_zip
{

namespace ios	=boost::iostreams;
namespace ipc	=boost::interprocess;
namespace fsys	=boost::filesystem;
namespace range	=boost::range;

typedef unsigned long DWORD;
typedef unsigned char BYTE;

extern const std::string ZIP_HEADER;

/// \brief EncryptedZipで暗号化/圧縮/アーカイブされたファイルを展開するクラス
class DecryptedZip
{
public:
	typedef boost::intmax_t SizeType;

	class ArchivedFile;
	class ArchivedFile_Exist;

	//typedef boost::shared_ptr<ArchivedFile> ArchivedFilePtr;
	typedef ArchivedFile* ArchivedFilePtr;

private:
	typedef boost::unordered_map<fsys::path,header::CentralHeader>	CentralHeaderMap;
	typedef CentralHeaderMap::value_type							CentralHeaderMapValueType;
	//typedef boost::flyweight<ArchivedFile>							FlyweightArchivedFile;

	const std::string		archive_name_;
	const fsys::path		archive_path_;
	header::TerminalHeader	terminal_;
	CentralHeaderMap		centrals_;

	template<typename StreamType>
	void ReadCentralHeader(StreamType& stream);

	template<typename StreamType>
	void ReadTerminalHeader(StreamType& stream);

	//
	CentralHeaderMap::iterator GetIteratorToCentralHeader(fsys::path& file);

public:

	/// \brief コンストラクタ
	DecryptedZip(const fsys::path& source,DWORD key);

	/// \brief デストラクタ
	~DecryptedZip();

	/// ファイルがアーカイブに含まれているか
	/// \return もしアーカイブに含まれているならtrueを返し、そうでないならfalseを返す
	bool Exists(fsys::path& file);
	
	/** \brief ファイルのArchivedFileクラスインスタンスを取得する
	*	\return ArchivedFileクラスインスタンス
	*	Exists(file)==falseの場合でもno throwだが、返されたインスタンスのOutTo***系関数は必ずfalseを返す
	*/
	ArchivedFilePtr GetFile(fsys::path& file);

	///boost::flyweightに包まれたArchivedFileを返すGetFile()
	//FlyweightArchivedFile GetFile_flyweight(fsys::path& file);

	///ファイルが存在しない場合返されるクラス
	class ArchivedFile
	{
		const std::string	archive_name_;
		const bool			this_file_exists_;
		const bool			is_crypted_;

		friend ArchivedFilePtr DecryptedZip::GetFile(fsys::path& file);
		friend class filtering_istream_ptr;
		friend class ArchivedFile_Exist;

		/// \brief コンストラクタ
		ArchivedFile(const std::string& archive_name,const bool& this_file_exists,const bool& is_crypted)
			:
				archive_name_(archive_name)			,
				this_file_exists_(this_file_exists)	,
				is_crypted_(is_crypted)
		{}

	public:

		/// \brief ムーブコンストラクタ
		ArchivedFile(ArchivedFile& src)
			:
				archive_name_(src.archive_name_)		,
				this_file_exists_(src.this_file_exists_),
				is_crypted_(src.is_crypted_)
		{}

		/// \return アーカイブ内にあるファイルならtrueを返す
		bool Exists()
			{
				return this_file_exists_;
			}

		/// \return アーカイブ内にあるファイルならtrueを返す
		operator bool()
			{
				return Exists();
			}

		/// \return 伸長後のファイルサイズ(bytes)
		virtual boost::intmax_t Size()
			{
				return RawSize();
			}

		/// \return 伸長後のファイルサイズ(bytes)
		virtual boost::intmax_t RawSize()
			{
				return 0;
			}

		/// \return 伸長前のファイルサイズ(bytes)
		virtual boost::intmax_t CompedSize()
			{
				return 0;
			}

		/**	\brief 復号/伸長/アンアーカイブしたイメージをメモリに出力する
		*	\param dst　出力先メモリの先頭アドレス
		*	\param dst_size 出力先メモリの確保済みサイズ(bytes)
		*	\return 出力まで成功したらtrueを返し、そうでないならflaseを返す
		*/
		virtual bool OutToMemory(BYTE* dst,DWORD dst_size)
			{
				return false;
			}
		
		/**	\brief 復号/伸長/アンアーカイブしたイメージをファイルに出力する
		*	\param dst 出力先ファイルパス
		*	\return 出力まで成功したらtrueを返し、そうでないならflaseを返す
		*/
		virtual bool OutToFile(const fsys::path& dst)
			{
				return false;
			}
	};

	///ファイルが存在する場合返されるクラス
	class ArchivedFile_Exist
		: public ArchivedFile
	{
	public:
		typedef boost::intmax_t SizeType;

		#ifdef KVARCO_CRYPTED_ZIP_USE_ZLIB
			typedef ios::zlib_decompressor	DecompresserType;
		#else
			typedef ios::gzip_decompressor	DecompresserType;
		#endif

		typedef ipc::unique_ptr
		<
			ios::filtering_istream,
			boost::checked_deleter<ios::filtering_istream>
		> filtering_istream_ptr;

		typedef boost::scoped_ptr<ipc::ibufferstream> ipc_ibufferstream_ptr;

	private:

		// DecryptedZip内からのみ作成できる
		friend ArchivedFilePtr DecryptedZip::GetFile(fsys::path& file);
		friend class filtering_istream_ptr;
		friend class ArchivedFile_Exist;

		/// \brief コンストラクタ
		ArchivedFile_Exist
			(
				const std::string& archive_name		,
				const fsys::path& rlt_path			,
				const header::CentralHeader& info	,
				const bool& is_crypted				,
				ipc::file_mapping& file_map
			)
		:
			ArchivedFile(archive_name,true,is_crypted)	,
			rlt_path_(rlt_path)							,
			file_info_(info)							,
			//it can read range of the file
			file_view_(file_map,ipc::read_only,
						file_info_.data_pos_,file_info_.data_pos_+file_info_.comped_size_)
		{}

		//
		const fsys::path			rlt_path_;
		const header::CentralHeader	file_info_;
		ipc::mapped_region			file_view_;

		detail::DiceSet				dices_;
		ipc_ibufferstream_ptr		source_stream;

		filtering_istream_ptr MakeInputStream();
	public:

		/// \brief ムーブコンストラクタ
		ArchivedFile_Exist(ArchivedFile_Exist& src)
			:
				ArchivedFile(src.archive_name_,true,src.is_crypted_),
				rlt_path_(src.rlt_path_)							,
				file_info_(src.file_info_)
		{
			file_view_.swap(src.file_view_);
		}

		/// \return アーカイブ内にあるファイルならtrueを返す
		operator bool()
			{
				return Exists();
			}

		/// \return 伸長後のファイルサイズ(bytes)
		boost::intmax_t Size()
			{
				return RawSize();
			}

		/// \return 伸長後のファイルサイズ(bytes)
		boost::intmax_t RawSize();

		/// \return 伸長前のファイルサイズ(bytes)
		boost::intmax_t CompedSize();

		/**	\brief 復号/伸長/アンアーカイブしたイメージをメモリに出力する
		*	\param dst　出力先メモリの先頭アドレス
		*	\param dst_size 出力先メモリの確保済みサイズ(bytes)
		*	\return 出力まで成功したらtrueを返し、そうでないならflaseを返す
		*/
		bool OutToMemory(BYTE* dst,DWORD dst_size);
		
		/**	\brief 復号/伸長/アンアーカイブしたイメージをファイルに出力する
		*	\param dst 出力先ファイルパス
		*	\return 出力まで成功したらtrueを返し、そうでないならflaseを返す
		*/
		bool OutToFile(const fsys::path& dst);
	};

};

} //namespace crypted_zip
} //namespace kvarco

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma warning(pop)
#endif

#endif