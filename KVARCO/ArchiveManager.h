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

	/// ".XXX"形式のアーカイブファイルの拡張子
	fsys::path extention_;
	std::string password_;

	ArchivesMap archives_map_;
	inline ArchivesMapType::iterator GetLoadedArchiveIterator(const fsys::path& arc);

	ArchivePtrType LoadArchive(const fsys::path& archive);

	/// アーカイブファイル内のファイルパスまでを含んだパスから、アーカイブファイルまでのパスだけを返す
	fsys::path GetArchivePath(const fsys::path& archive);

	/// アーカイブファイル内のファイルパスまでを含んだパスから、アーカイブファイルまでのパスを除いたものを返す
	fsys::path GetArchivedFilePath(const fsys::path& archive);

	/// アーカイブファイル内のファイルパスまでを含んだパスを、アーカイブファイルまでのパスと、それ以降に分割する
	/// \param archive_path out アーカイブファイルまでのパス
	/// \param archived_file_path out アーカイブファイルから先のパス
	void DivideArchivedFilePath(const fsys::path& file,fsys::path* archive_path,fsys::path* archived_file_path);

	///メモリを確保してから展開(実装)
	template<typename Type=char,typename Alloc=std::allocator<Type> >
	Type* UnzipToMemory_impl(fsys::path& file);

	ArchiveFileSharedPtrType GetArchivedFile(fsys::path& file);

public:
	/// \param ext アーカイブファイルの拡張子(標準では"kcz")
	void SetExtentionOfArchive(const fsys::path& ext)
		{
			extention_=ext;
		}

	/// \param password アーカイブファイルのパスワード
	void SetPassword(const std::string& password)
		{
			password_=password;
		}

	/// \return 指定されたファイルがあったらtrue。アーカイブ違いでもfalseになる
	/// \param file 形式は [アーカイブ名]/[アーカイブ内でのファイルパス]
	bool Exists(const fsys::path& file);

	/// \return 指定されたアーカイブがあったらtrue
	inline bool ArchiveExists(const fsys::path& archive);
	
	/// すでに確保されたメモリに展開
	/// \return ファイル展開に成功した場合はtrue,失敗した場合はfalse
	bool UnzipToMemory(fsys::path& file,BYTE* dst,DWORD dst_size);

	/// メモリを確保してから展開(ver.native)
	template<typename Type=char,typename Alloc=std::allocator<Type> >
	boost::shared_array<Type> UnzipToAllocatedMemory(fsys::path& file);

	/// メモリを確保してから展開(ver.xtal)
	xtal::MemoryStreamPtr UnzipToAllocatedMemoryX(xta::String& file);

	/// 指定ファイルに展開
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