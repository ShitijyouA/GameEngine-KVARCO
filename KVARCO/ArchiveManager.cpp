#include "pch.h"
#include "ArchiveManager.h"

namespace boost
{
namespace filesystem3
{

inline size_t hash_value(const path& data)
{
	return boost::hash_value(data.native());
}

}
}

fsys::path ArchiveManager::GetArchivedFilePath(const fsys::path& archive)
{
	fsys::path archive_path;
	fsys::path archived_file_path;
	DivideArchivedFilePath(archive,&archive_path,&archived_file_path);

	return archived_file_path;
}

fsys::path ArchiveManager::GetArchivePath(const fsys::path& archive)
{
	fsys::path tmp_path;
	for(fsys::path::iterator i=archive.begin(); i!=archive.end(); ++i)
	{
		tmp_path/=(*i);
		if(fsys::exists(tmp_path/extention_)) break;
	}

	return tmp_path;
}
void ArchiveManager::DivideArchivedFilePath(const fsys::path& file,fsys::path* archive_path,fsys::path* archived_file_path)
{
	archive_path->clear();
	archived_file_path->clear();

	fsys::path::iterator i;
	for(i=file.begin(); i!=file.end(); ++i)
	{
		(*archive_path)/=(*i);
		if(fsys::exists(archive_path->string()+extention_)) break;
	}

	//copy different path range
	for(; i!=file.end(); ++i) (*archived_file_path)/=(*i);
}

inline ArchiveManager::ArchivesMapType::iterator ArchiveManager::GetLoadedArchiveIterator(const fsys::path& arc)
{
	return archives_map_.find(arc);
}

bool ArchiveManager::Exists(const fsys::path& file)
{
	fsys::path archive_path=GetArchivePath(file);
	if(!archive_path.empty()) return false;

	ArchivesMapType::iterator it=GetLoadedArchiveIterator(archive_path);
	if(it!=archives_map_.end()) return true;
	else
	{
		ArchivePtrType arc_ptr(LoadArchive(archive_path));
		if(arc_ptr->Exists(GetArchivedFilePath(file))) return true;
	}

	return false;
}

inline bool ArchiveManager::ArchiveExists(const fsys::path& archive)
{
	return !GetArchivePath(archive).empty();
}

ArchiveManager::ArchivePtrType ArchiveManager::LoadArchive(const fsys::path& archive)
{
	ArchivePtrType arc_ptr(new ArchiveType(archive,password_));
	archives_map_.insert(std::make_pair(archive,arc_ptr));
	return arc_ptr;
}

void ArchiveManager::LoadArchiveX(const xtal::String& archive)
{
	LoadArchive(archive.c_str());
}

ArchiveManager::ArchiveFileSharedPtrType ArchiveManager::GetArchivedFile(const fsys::path& file)
{
	fsys::path archive_path;
	fsys::path archived_file_path;
	DivideArchivedFilePath(file,&archive_path,&archived_file_path);
	if(archive_path.empty() || archived_file_path.empty()) return ArchiveFileSharedPtrType();

	ArchivesMapType::iterator it=GetLoadedArchiveIterator(archive_path);
	ArchiveFileSharedPtrType the_file((*it).second->GetFile(archived_file_path));

	return the_file;
}

bool ArchiveManager::UnzipToMemory(const fsys::path& file,BYTE* dst,DWORD dst_size)
{
	ArchiveFileSharedPtrType the_file=GetArchivedFile(file);
	if(!the_file->Exists()) return false;

	//ëΩï™Ç¢ÇÁÇ»Ç¢ÇØÇ«ÅAàÍâûèëÇ¢ÇƒÇ®Ç≠
	//if(the_file->Exists())
	//	return reinterpret_cast<ArchiveType::ArchivedFile_Exist*>(the_file.get())->UnzipToMemory(dst,dst_size);
	//else
	//	return the_file->UnzipToMemory(dst,dst_size);

	return the_file->UnzipToMemory(dst,dst_size);
}

///ÉÅÉÇÉäÇämï€ÇµÇƒÇ©ÇÁìWäJ(é¿ëï)
template<typename Type,typename Alloc>
Type* ArchiveManager::UnzipToMemory_impl(fsys::path& file,boost::intmax_t* buf_size)
{
	ArchiveFileSharedPtrType the_file(GetArchivedFile(file));
	if(!the_file->Exists()) return NULL;
	DWORD size=the_file->RawSize();

	Alloc alloc;
	Alloc::pointer buf_ptr=alloc.allocate(size);
	Type* dst_buf=alloc.allocate(size);

	the_file->UnzipToMemory(reinterpret_cast<kvarco::crypted_zip::BYTE*>(dst_buf),size);

	if(buf_size!=NULL) *buf_size=size;
	return dst_buf;
}

template<typename Type,typename Alloc>
boost::shared_array<Type> ArchiveManager::UnzipToAllocatedMemory(const fsys::path& file)
{
	return boost::shared_array<Type>(UnzipToMemory_impl<Type,Alloc>(file,NULL));
}

xtal::MemoryStreamPtr ArchiveManager::UnzipToAllocatedMemoryX(xtal::String& file)
{
	boost::intmax_t size=0;
	char* mem_stream=UnzipToMemory_impl<char,std::allocator<char> >(fsys::path(file.c_str()),&size);
	return xtal::xnew<xtal::MemoryStream>(mem_stream,size);
}

bool ArchiveManager::UnzipToFile(fsys::path& file,fsys::path& dst_file)
{
	ArchiveFileSharedPtrType the_file(GetArchivedFile(file));
	if(!the_file->Exists()) return false;

	return the_file->UnzipToFile(dst_file);
}

ArchiveManager::ArchiveMngrPtr ArchiveManager::GetInst()
{
	static ArchiveManager Inst;
	return &Inst;
}

void ArchiveManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(ArchiveManager);

	Xdef_method_alias(SetExtentionOfArchive,&SetExtentionOfArchiveX);
	Xdef_method_alias(SetPassword,&SetPasswordX);
	Xdef_method(Exists);
	Xdef_method(ArchiveExists);
	Xdef_method_alias(LoadArchive,&LoadArchiveX);
	Xdef_method_alias(UnzipToMemory,&UnzipToAllocatedMemoryX);
	Xdef_method(UnzipToFile);
}