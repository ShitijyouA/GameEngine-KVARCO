//Boost! Boost! Boost!

#ifndef KVARCO_CRYPTED_ZIP_ENCRYPTED_ZIP_H_
#define KVARCO_CRYPTED_ZIP_ENCRYPTED_ZIP_H_

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

#include <boost/iostreams/seek.hpp>
#include <boost/iostreams/read.hpp>
#include <boost/iostreams/write.hpp>

#ifndef BOOST_FILESYSTEM_VERSION
#	define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/utility/result_of.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

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

/// \brief ファイルを暗号化/圧縮/アーカイブするクラス
class EncryptedZip
{
public:
	#ifdef KVARCO_CRYPTED_ZIP_USE_ZLIB
		typedef ios::zlib_compressor	CompresserType;
		typedef ios::zlib_params		CompresserParamType;
	#else
		typedef ios::gzip_compressor	CompresserType;
		typedef ios::gzip_params		CompresserParamType;
	#endif
	typedef boost::intmax_t SizeType;

	typedef boost::scoped_ptr<concept::Logger>	LoggerPtrType;
	//concept::Logger*	LoggerPtrType;
	
private:
	LoggerPtrType logger_;

	CompresserParamType compress_params_;
	
	bool do_encrypt_;
	detail::DiceSet dices_;

	fsys::path					parent_dir_;			///< アーカイブするファイル(フォルダ)がある親フォルダのパス。変化する
	header::CentralHeaderList	central_header_list_;	///< 各ファイルのCentralHeader
	detail::PathList			input_file_path_list_;	///< 各ファイルの絶対パス

	template<typename StreamType>
	void CryptingAndZipping_impl(const fsys::path& path,StreamType& dst_stream);

	void InsertAllFilePath(const fsys::path& dir);
	void MakeRltPathList(detail::PathList& rls_path_list);
	void MakeCentralHeaderList();
	
	template<typename DstStreamType>
	bool CompToDst(header::PathAndCentralHeader& it,DstStreamType& dst_stream);

	template<typename DstStreamType>
	bool WriteToTemp(header::PathAndCentralHeader& it,DstStreamType& dst_stream);

	template<typename DstStreamType>
	bool WriteToDst(header::PathAndCentralHeader& it,DstStreamType& dst_stream);

public:
	/** \brief コンストラクタ
	* \param source アーカイブするファイル(フォルダ)のパス。相対パスでも絶対パスでも可
	* \param do_encrypt 暗号化するかどうか
	* \param params 圧縮するためのパラメータ。詳細はboost::iostreams::zlib_paramsの解説を参照
	*/
	template<typename LoggerType>
	EncryptedZip(const fsys::path& source,bool do_encrypt=true,CompresserParamType params=CompresserParamType(),LoggerType logger=logger::NullLogger())
			: compress_params_(params),do_encrypt_(do_encrypt),logger_(new LoggerType)
		{
			AddPackFile(source);
		}

	~EncryptedZip();

	/// \brief 圧縮するファイルの追加
	void AddPackFile(const fsys::path& source);

	/// \brief 圧縮するファイルの追加
	void operator+=(const fsys::path& source)
		{
			AddPackFile(source);
		}
	
	/** \brief 暗号化/圧縮/アーカイブしたイメージをファイルに出力する
	*	\return 出力まで成功したらtrueを返し、そうでないならflaseを返す
	*/
	bool OutToFile(const fsys::path& dst);
};

} //namespace crypted_zip
} //namespace kvarco

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma warning(pop)
#endif

#endif