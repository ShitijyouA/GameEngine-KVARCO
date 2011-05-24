//Boost! Boost! Boost!

#ifndef KVARCO_CRYPTED_ZIP_CRYPTED_ZIP_H_
#define KVARCO_CRYPTED_ZIP_CRYPTED_ZIP_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
# pragma warning(push)
# pragma warning(disable:4996)
#endif

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/copy.hpp>

#ifndef BOOST_FILESYSTEM_VERSION
	#define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/crc.hpp>

#include <boost/foreach.hpp>

#include <fstream>
#include <iterator>
#include <string>

#include "detail.h"
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

/** \class EncryptedZip
* \brief ファイルを暗号化/圧縮/アーカイブするクラス
*/
class EncryptedZip
{
	ios::zlib_params zipping_params_; 
	
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
	bool CompToTemp(header::PathAndCentralHeader& it,DstStreamType& dst_stream);

public:
	/** \brief コンストラクタ
	* \param source アーカイブするファイル(フォルダ)のパス。相対パスでも絶対パスでも可
	* \param do_encrypt 暗号化するかどうか
	* \param params 圧縮するためのパラメータ。詳細はboost::iostreams::zlib_paramsの解説を参照
	*/
	EncryptedZip(const fsys::path& source,bool do_encrypt=true,ios::zlib_params params=ios::zlib_params());
	~EncryptedZip();

	/// \brief 圧縮するファイルの追加
	void AddPackFile(const fsys::path& source);
	void operator+=(const fsys::path& source)
		{
			AddPackFile(source);
		}
	
	/// \brief 
	//bool OutToMemory(BYTE* dst);
	
	/// \brief 暗号化/圧縮/アーカイブしたイメージをファイルに出力する
	bool OutToFile(const fsys::path& dst);
};

/** \class DecryptedZip
* \brief EncryptedZipで暗号化/圧縮/アーカイブされたファイルを展開するクラス
*/
class DecryptedZip
{
	typedef boost::unordered_map<fsys::path,header::CentralHeader>	CentralHeaderMap;
public:
	DecryptedZip() {}
};

} //namespace crypted_zip
} //namespace kvarco

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma warning(pop)
#endif

#endif