#pragma once
#include <boost/shared_array.hpp>

class ArchiveManager
{
public:
	typedef ArchiveManager*									ArchiveMngrPtr;
	typedef kvarco::crypted_zip::DecryptedZip				ArchiveType;
	typedef boost::shared_ptr<ArchiveType>					ArchivePtrType;
	typedef ArchiveType::ArchivedFilePtr					ArchiveFilePtrType;
	typedef boost::shared_ptr<ArchiveType::ArchivedFile>	ArchiveFileSharedPtrType;
	typedef boost::unordered_map<fsys::path,ArchivePtrType>	ArchivesMapType;
	typedef ArchivesMapType::value_type						ArchiveInfoType;
	typedef boost::shared_ptr<char>							AllocatedMemoryType;
	
	typedef xtal::StringPtr									PathTypeX;
	typedef xtal::StringPtr									StringTypeX;
	
private:
	/// ".XXX"�`���̃A�[�J�C�u�t�@�C���̊g���q
	std::string extension_;
	std::string password_;

	ArchivesMapType archives_map_;
	inline ArchivesMapType::iterator GetLoadedArchiveIterator(const fsys::path& arc);

	/// �A�[�J�C�u�t�@�C�����̃t�@�C���p�X�܂ł��܂񂾃p�X����A�A�[�J�C�u�t�@�C���܂ł̃p�X������Ԃ�
	fsys::path GetArchivePath(const fsys::path& archive);

	/// �A�[�J�C�u�t�@�C�����̃t�@�C���p�X�܂ł��܂񂾃p�X����A�A�[�J�C�u�t�@�C���܂ł̃p�X�����������̂�Ԃ�
	fsys::path GetArchivedFilePath(const fsys::path& archive);

	/// �A�[�J�C�u�t�@�C�����̃t�@�C���p�X�܂ł��܂񂾃p�X���A�A�[�J�C�u�t�@�C���܂ł̃p�X�ƁA����ȍ~�ɕ�������
	/// \param archive_path out �A�[�J�C�u�t�@�C���܂ł̃p�X
	/// \param archived_file_path out �A�[�J�C�u�t�@�C�������̃p�X
	void DivideArchivedFilePath(const fsys::path& file,fsys::path* archive_path,fsys::path* archived_file_path);

	///���������m�ۂ��Ă���W�J(����)
	template<typename Type,typename Alloc>
	Type* UnzipToMemory_impl(const fsys::path& file,boost::intmax_t* buf_size);

	ArchiveFileSharedPtrType GetArchivedFile(const fsys::path& file);

public:
	/// \param ext �A�[�J�C�u�t�@�C���̊g���q(�W���ł�"kcz")(ver.native)
	void SetExtensionOfArchive(const fsys::path& ext)
		{
			static const std::string dot(".");
			extension_=dot+ext.string();
		}

	/// \param password �A�[�J�C�u�t�@�C���̃p�X���[�h(ver.native)
	void SetPassword(const std::string& password)
		{
			password_=password;
		}

	/// \param ext �A�[�J�C�u�t�@�C���̊g���q(�W���ł�"kcz")(ver.xtal)
	void SetExtensionOfArchiveX(StringTypeX& ext)
		{
			static const xtal::String dot(".");
			xtal::StringPtr full=dot.cat(ext.to_s());
			extension_=full->c_str();
		}

	/// \param password �A�[�J�C�u�t�@�C���̃p�X���[�h(ver.xtal)
	void SetPasswordX(StringTypeX& password)
		{
			password_=password->c_str();
		}

	/// \return �w�肳�ꂽ�t�@�C������������true�B�A�[�J�C�u�Ⴂ�ł�false�ɂȂ�
	/// \param file �`���� [�A�[�J�C�u��]/[�A�[�J�C�u���ł̃t�@�C���p�X]
	bool Exists(const fsys::path& file);

	/// Exists() ver.xtal
	bool ExistsX(const xtal::String file);

	/// \return �w�肳�ꂽ�A�[�J�C�u����������true
	inline bool ArchiveExists(const fsys::path& archive);

	/// ArchiveExists() ver.xtal
	inline bool ArchiveExistsX(const PathTypeX archive);
	
	/// �w�肳�ꂽ�A�[�J�C�u��ǂݍ���(ver.native)
	ArchivePtrType LoadArchive(const fsys::path& archive);

	/// �w�肳�ꂽ�A�[�J�C�u��ǂݍ���(ver.xtal)
	void LoadArchiveX(const PathTypeX& archive);

	/// ���łɊm�ۂ��ꂽ�������ɓW�J
	/// \return �t�@�C���W�J�ɐ��������ꍇ��true,���s�����ꍇ��false
	bool UnzipToMemory(const fsys::path& file,BYTE* dst,boost::intmax_t dst_size,boost::intmax_t* res_size);

	/// ���������m�ۂ��Ă���W�J(ver.native)
	AllocatedMemoryType UnzipToAllocatedMemory(const fsys::path& file,boost::intmax_t* buf_size);

	/// ���������m�ۂ��Ă���W�J(ver.xtal)
	xtal::MemoryStreamPtr UnzipToAllocatedMemoryX(PathTypeX& file);

	/// �w��t�@�C���ɓW�J
	bool UnzipToFile(fsys::path& file,fsys::path& dst_file);

	//singleton
private:
	FRIENDS_FOR_XTAL(ArchiveManager)

	ArchiveManager()
		:extension_("kcz"),password_("")
		{}

public:
	static void bind(xtal::ClassPtr it);
	static ArchiveMngrPtr GetInst();
};
