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
#include <boost/range/istream_range.hpp>

#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>

#include "detail.h"
#include "Logger.h"
#include "Header.h"
#include "Arcfour.h"

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

/// \brief �t�@�C�����Í���/���k/�A�[�J�C�u����N���X
template<typename LoggerType>
class EncryptedZip_impl
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
	
private:
	LoggerType logger_;

	CompresserParamType compress_params_;
	
	bool do_encrypt_;
	detail::DiceSet dices_;
	const std::string password_;

	fsys::path					parent_dir_;			///< �A�[�J�C�u����t�@�C��(�t�H���_)������e�t�H���_�̃p�X�B�ω�����
	header::CentralHeaderList	central_header_list_;	///< �e�t�@�C����CentralHeader
	detail::PathList			input_file_path_list_;	///< �e�t�@�C���̐�΃p�X

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
	/** \brief �R���X�g���N�^
	* \param source �A�[�J�C�u����t�@�C��(�t�H���_)�̃p�X�B���΃p�X�ł���΃p�X�ł���
	* \param password �Í����̍ۂ̃p�X���[�h�B""(�󕶎�)�Ȃ�Í������Ȃ�
	* \param params ���k���邽�߂̃p�����[�^�B�ڍׂ�boost::iostreams::zlib_params�̉�����Q��
	*/
	EncryptedZip_impl
		(
			const fsys::path& source						,
			std::string password							,
			CompresserParamType params=CompresserParamType()
		)
			: password_(password),compress_params_(params)
		{
			AddPackFile(source);
		}

	~EncryptedZip_impl();

	/// \brief ���k����t�@�C���̒ǉ�
	void AddPackFile(const fsys::path& source);

	/// \brief ���k����t�@�C���̒ǉ�
	void operator+=(const fsys::path& source)
		{
			AddPackFile(source);
		}
	
	/** \brief �Í���/���k/�A�[�J�C�u�����C���[�W���t�@�C���ɏo�͂���
	*	\return �o�͂܂Ő���������true��Ԃ��A�����łȂ��Ȃ�flase��Ԃ�
	*/
	bool ZipToFile(const fsys::path& dst);
};

typedef EncryptedZip_impl<logger::NullLogger>	NullLogEncryptedZip;
typedef EncryptedZip_impl<logger::StdLogger>	StdLogEncryptedZip;
typedef StdLogEncryptedZip						EncryptedZip;

} //namespace crypted_zip
} //namespace kvarco

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma warning(pop)
#endif

#endif