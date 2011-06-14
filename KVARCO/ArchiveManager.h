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
	///\return 指定されたファイルがあったらtrue。アーカイブ違いでもfalseになる
	///\param file 形式は [アーカイブ名]/[アーカイブ内でのファイルパス]
	bool Exists(fsys::path& file);

	///\return 指定されたアーカイブがあったらtrue
	bool ArchiveExists(fsys::path& arc);
	
	///すでに確保されたメモリに展開
	///\return ファイル展開に成功した場合はtrue,失敗した場合はfalse
	bool DecompToMemory(fsys::path& file,BYTE* dst,DWORD dst_size);

	///メモリを確保してから展開
	template<typename Type,typename Alloc=std::allocator<Type> >
	boost::shared_ptr<Type> DecompToMemory(fsys::path& file);

	///指定ファイルに展開
	bool DecompToFile(fsys::path& file,fsys::path& dst_file);

	//singleton
	SINGLETON_PATTERNX(ArchiveManager,ArchiveMngrPtr)
};

#endif