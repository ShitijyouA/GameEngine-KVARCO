#pragma once
#include <boost/shared_array.hpp>

class ArchiveManager
{
public:
	typedef ArchiveManager*									ArchiveMngrPtr;
	
private:
	typedef crypted_zip::DecryptedZip						ArchiveType;
	typedef boost::shared_ptr<ArchiveType>					ArchivePtrType;
	typedef ArchivedFileType::ArchivedFilePtr				ArchiveFilePtrType;
	typedef boost::shared_ptr<ArchiveType::ArchivedFile>	ArchiveFileSharedPtrType;
	typedef boost::unordered_map<fsys::path,ArchivePtrType>	ArchivesMapType;
	typedef ArchivesMap::value_type							ArchiveInfoType;

	/// ".XXX"�`���̃A�[�J�C�u�t�@�C���̊g���q
	fsys::path extention_;
	std::string password_;

	ArchivesMap archives_map_;
	inline ArchivesMapType::iterator GetLoadedArchiveIterator(const fsys::path& arc);

	ArchivePtrType LoadArchive(const fsys::path& archive);

	/// �A�[�J�C�u�t�@�C�����̃t�@�C���p�X�܂ł��܂񂾃p�X����A�A�[�J�C�u�t�@�C���܂ł̃p�X������Ԃ�
	fsys::path GetArchivePath(const fsys::path& archive);

	/// �A�[�J�C�u�t�@�C�����̃t�@�C���p�X�܂ł��܂񂾃p�X����A�A�[�J�C�u�t�@�C���܂ł̃p�X�����������̂�Ԃ�
	fsys::path GetArchivedFilePath(const fsys::path& archive);

	/// �A�[�J�C�u�t�@�C�����̃t�@�C���p�X�܂ł��܂񂾃p�X���A�A�[�J�C�u�t�@�C���܂ł̃p�X�ƁA����ȍ~�ɕ�������
	/// \param archive_path out �A�[�J�C�u�t�@�C���܂ł̃p�X
	/// \param archived_file_path out �A�[�J�C�u�t�@�C�������̃p�X
	void DivideArchivedFilePath(const fsys::path& file,fsys::path* archive_path,fsys::path* archived_file_path);

	///���������m�ۂ��Ă���W�J(����)
	template<typename Type=char,typename Alloc=std::allocator<Type> >
	Type* UnzipToMemory_impl(fsys::path& file);

	ArchiveFileSharedPtrType GetArchivedFile(fsys::path& file);

public:
	/// \param ext �A�[�J�C�u�t�@�C���̊g���q(�W���ł�"kcz")
	void SetExtentionOfArchive(const fsys::path& ext)
		{
			extention_=ext;
		}

	/// \param password �A�[�J�C�u�t�@�C���̃p�X���[�h
	void SetPassword(const std::string& password)
		{
			password_=password;
		}

	/// \return �w�肳�ꂽ�t�@�C������������true�B�A�[�J�C�u�Ⴂ�ł�false�ɂȂ�
	/// \param file �`���� [�A�[�J�C�u��]/[�A�[�J�C�u���ł̃t�@�C���p�X]
	bool Exists(const fsys::path& file);

	/// \return �w�肳�ꂽ�A�[�J�C�u����������true
	inline bool ArchiveExists(const fsys::path& archive);
	
	/// ���łɊm�ۂ��ꂽ�������ɓW�J
	/// \return �t�@�C���W�J�ɐ��������ꍇ��true,���s�����ꍇ��false
	bool UnzipToMemory(fsys::path& file,BYTE* dst,DWORD dst_size);

	/// ���������m�ۂ��Ă���W�J(ver.native)
	template<typename Type=char,typename Alloc=std::allocator<Type> >
	boost::shared_array<Type> UnzipToAllocatedMemory(fsys::path& file);

	/// ���������m�ۂ��Ă���W�J(ver.xtal)
	xtal::MemoryStreamPtr UnzipToAllocatedMemoryX(xta::String& file);

	/// �w��t�@�C���ɓW�J
	bool UnzipToFile(fsys::path& file,fsys::path& dst_file);

	//singleton
private:
	FRIENDS_FOR_XTAL(ArchiveManager)

	ArchiveManager()
		:extention_("kcz"),password_("")
		{}

public:
	void bind(xtal::ClassPtr it);
	static ArchiveMngrPtr GetInst();
};