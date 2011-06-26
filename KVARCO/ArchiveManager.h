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
	
private:
	/// ".XXX"形式のアーカイブファイルの拡張子
	std::string extention_;
	std::string password_;

	ArchivesMapType archives_map_;
	inline ArchivesMapType::iterator GetLoadedArchiveIterator(const fsys::path& arc);

	/// アーカイブファイル内のファイルパスまでを含んだパスから、アーカイブファイルまでのパスだけを返す
	fsys::path GetArchivePath(const fsys::path& archive);

	/// アーカイブファイル内のファイルパスまでを含んだパスから、アーカイブファイルまでのパスを除いたものを返す
	fsys::path GetArchivedFilePath(const fsys::path& archive);

	/// アーカイブファイル内のファイルパスまでを含んだパスを、アーカイブファイルまでのパスと、それ以降に分割する
	/// \param archive_path out アーカイブファイルまでのパス
	/// \param archived_file_path out アーカイブファイルから先のパス
	void DivideArchivedFilePath(const fsys::path& file,fsys::path* archive_path,fsys::path* archived_file_path);

	///メモリを確保してから展開(実装)
	template<typename Type,typename Alloc>
	Type* UnzipToMemory_impl(fsys::path& file,boost::intmax_t* buf_size);

	ArchiveFileSharedPtrType GetArchivedFile(const fsys::path& file);

public:
	/// \param ext アーカイブファイルの拡張子(標準では"kcz")(ver.native)
	void SetExtentionOfArchive(const fsys::path& ext)
		{
			static const std::string dot(".");
			extention_=dot+ext.string();
		}

	/// \param password アーカイブファイルのパスワード(ver.native)
	void SetPassword(const std::string& password)
		{
			password_=password;
		}

	/// \param ext アーカイブファイルの拡張子(標準では"kcz")(ver.xtal)
	void SetExtentionOfArchiveX(xtal::String& ext)
		{
			static const xtal::String dot(".");
			xtal::StringPtr full=dot.cat(ext.to_s());
			extention_=full->c_str();
		}

	/// \param password アーカイブファイルのパスワード(ver.xtal)
	void SetPasswordX(xtal::String& password)
		{
			password_=password.c_str();
		}

	/// \return 指定されたファイルがあったらtrue。アーカイブ違いでもfalseになる
	/// \param file 形式は [アーカイブ名]/[アーカイブ内でのファイルパス]
	bool Exists(const fsys::path& file);

	/// \return 指定されたアーカイブがあったらtrue
	inline bool ArchiveExists(const fsys::path& archive);
	
	/// 指定されたアーカイブを読み込む(ver.native)
	ArchivePtrType LoadArchive(const fsys::path& archive);

	/// 指定されたアーカイブを読み込む(ver.xtal)
	void LoadArchiveX(const xtal::String& archive);

	/// すでに確保されたメモリに展開
	/// \return ファイル展開に成功した場合はtrue,失敗した場合はfalse
	bool UnzipToMemory(const fsys::path& file,BYTE* dst,DWORD dst_size);

	/// メモリを確保してから展開(ver.native)
	template<typename Type,typename Alloc>
	boost::shared_array<Type> UnzipToAllocatedMemory(const fsys::path& file);

	/// メモリを確保してから展開(ver.xtal)
	xtal::MemoryStreamPtr UnzipToAllocatedMemoryX(xtal::String& file);

	/// 指定ファイルに展開
	bool UnzipToFile(fsys::path& file,fsys::path& dst_file);

	//singleton
private:
	FRIENDS_FOR_XTAL(ArchiveManager)

	ArchiveManager()
		:extention_("kcz"),password_("")
		{}

public:
	static void bind(xtal::ClassPtr it);
	static ArchiveMngrPtr GetInst();
};