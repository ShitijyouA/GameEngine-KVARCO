#pragma once

#if 1
#include <CryptedZip/CryptedZip.h>

class ArchiveManager
{
public:
	typedef ArchiveManager												ArchiveMngrPtr;
	
private:
	typedef boost::unordered_map<fsys::path,crypted_zip::DecryptedZip>	ArchivesMap;
	typedef ArchivesMap::value_type										ArchiveInfo;
	
	ArchivesMap archives_map_;

public:
	///\return �w�肳�ꂽ�t�@�C������������true�B�A�[�J�C�u�Ⴂ�ł�false�ɂȂ�
	///\param file �`���� [�A�[�J�C�u��]/[�A�[�J�C�u���ł̃t�@�C���p�X]
	bool Exists(fsys::path& file);

	///\return �w�肳�ꂽ�A�[�J�C�u����������true
	bool ArchiveExists(fsys::path& arc);
	
	///���łɊm�ۂ��ꂽ�������ɓW�J
	///\return �t�@�C���W�J�ɐ��������ꍇ��true,���s�����ꍇ��false
	bool DecompToMemory(fsys::path& file,BYTE* dst,DWORD dst_size);

	///���������m�ۂ��Ă���W�J
	template<typename Type,typename Alloc=std::allocator<Type> >
	boost::shared_ptr<Type> DecompToMemory(fsys::path& file);

	///�w��t�@�C���ɓW�J
	bool DecompToFile(fsys::path& file,fsys::path& dst_file);

	//singleton
	SINGLETON_PATTERNX(ArchiveManager,ArchiveMngrPtr)
};

#endif