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

/// \brief EncryptedZip�ňÍ���/���k/�A�[�J�C�u���ꂽ�t�@�C����W�J����N���X
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

	/// \brief �R���X�g���N�^
	DecryptedZip(const fsys::path& source,DWORD key);

	/// \brief �f�X�g���N�^
	~DecryptedZip();

	/// �t�@�C�����A�[�J�C�u�Ɋ܂܂�Ă��邩
	/// \return �����A�[�J�C�u�Ɋ܂܂�Ă���Ȃ�true��Ԃ��A�����łȂ��Ȃ�false��Ԃ�
	bool Exists(fsys::path& file);
	
	/** \brief �t�@�C����ArchivedFile�N���X�C���X�^���X���擾����
	*	\return ArchivedFile�N���X�C���X�^���X
	*	Exists(file)==false�̏ꍇ�ł�no throw�����A�Ԃ��ꂽ�C���X�^���X��OutTo***�n�֐��͕K��false��Ԃ�
	*/
	ArchivedFilePtr GetFile(fsys::path& file);

	///boost::flyweight�ɕ�܂ꂽArchivedFile��Ԃ�GetFile()
	//FlyweightArchivedFile GetFile_flyweight(fsys::path& file);

	///�t�@�C�������݂��Ȃ��ꍇ�Ԃ����N���X
	class ArchivedFile
	{
		const std::string	archive_name_;
		const bool			this_file_exists_;
		const bool			is_crypted_;

		friend ArchivedFilePtr DecryptedZip::GetFile(fsys::path& file);
		friend class filtering_istream_ptr;
		friend class ArchivedFile_Exist;

		/// \brief �R���X�g���N�^
		ArchivedFile(const std::string& archive_name,const bool& this_file_exists,const bool& is_crypted)
			:
				archive_name_(archive_name)			,
				this_file_exists_(this_file_exists)	,
				is_crypted_(is_crypted)
		{}

	public:

		/// \brief ���[�u�R���X�g���N�^
		ArchivedFile(ArchivedFile& src)
			:
				archive_name_(src.archive_name_)		,
				this_file_exists_(src.this_file_exists_),
				is_crypted_(src.is_crypted_)
		{}

		/// \return �A�[�J�C�u���ɂ���t�@�C���Ȃ�true��Ԃ�
		bool Exists()
			{
				return this_file_exists_;
			}

		/// \return �A�[�J�C�u���ɂ���t�@�C���Ȃ�true��Ԃ�
		operator bool()
			{
				return Exists();
			}

		/// \return �L����̃t�@�C���T�C�Y(bytes)
		virtual boost::intmax_t Size()
			{
				return RawSize();
			}

		/// \return �L����̃t�@�C���T�C�Y(bytes)
		virtual boost::intmax_t RawSize()
			{
				return 0;
			}

		/// \return �L���O�̃t�@�C���T�C�Y(bytes)
		virtual boost::intmax_t CompedSize()
			{
				return 0;
			}

		/**	\brief ����/�L��/�A���A�[�J�C�u�����C���[�W���������ɏo�͂���
		*	\param dst�@�o�͐惁�����̐擪�A�h���X
		*	\param dst_size �o�͐惁�����̊m�ۍς݃T�C�Y(bytes)
		*	\return �o�͂܂Ő���������true��Ԃ��A�����łȂ��Ȃ�flase��Ԃ�
		*/
		virtual bool OutToMemory(BYTE* dst,DWORD dst_size)
			{
				return false;
			}
		
		/**	\brief ����/�L��/�A���A�[�J�C�u�����C���[�W���t�@�C���ɏo�͂���
		*	\param dst �o�͐�t�@�C���p�X
		*	\return �o�͂܂Ő���������true��Ԃ��A�����łȂ��Ȃ�flase��Ԃ�
		*/
		virtual bool OutToFile(const fsys::path& dst)
			{
				return false;
			}
	};

	///�t�@�C�������݂���ꍇ�Ԃ����N���X
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

		// DecryptedZip������̂ݍ쐬�ł���
		friend ArchivedFilePtr DecryptedZip::GetFile(fsys::path& file);
		friend class filtering_istream_ptr;
		friend class ArchivedFile_Exist;

		/// \brief �R���X�g���N�^
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

		/// \brief ���[�u�R���X�g���N�^
		ArchivedFile_Exist(ArchivedFile_Exist& src)
			:
				ArchivedFile(src.archive_name_,true,src.is_crypted_),
				rlt_path_(src.rlt_path_)							,
				file_info_(src.file_info_)
		{
			file_view_.swap(src.file_view_);
		}

		/// \return �A�[�J�C�u���ɂ���t�@�C���Ȃ�true��Ԃ�
		operator bool()
			{
				return Exists();
			}

		/// \return �L����̃t�@�C���T�C�Y(bytes)
		boost::intmax_t Size()
			{
				return RawSize();
			}

		/// \return �L����̃t�@�C���T�C�Y(bytes)
		boost::intmax_t RawSize();

		/// \return �L���O�̃t�@�C���T�C�Y(bytes)
		boost::intmax_t CompedSize();

		/**	\brief ����/�L��/�A���A�[�J�C�u�����C���[�W���������ɏo�͂���
		*	\param dst�@�o�͐惁�����̐擪�A�h���X
		*	\param dst_size �o�͐惁�����̊m�ۍς݃T�C�Y(bytes)
		*	\return �o�͂܂Ő���������true��Ԃ��A�����łȂ��Ȃ�flase��Ԃ�
		*/
		bool OutToMemory(BYTE* dst,DWORD dst_size);
		
		/**	\brief ����/�L��/�A���A�[�J�C�u�����C���[�W���t�@�C���ɏo�͂���
		*	\param dst �o�͐�t�@�C���p�X
		*	\return �o�͂܂Ő���������true��Ԃ��A�����łȂ��Ȃ�flase��Ԃ�
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