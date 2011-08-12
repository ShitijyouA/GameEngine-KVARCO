#include "pch.h"
#include "TextureManager.h"
#include "ArchiveManager.h"
#include "KVARCO.h"

namespace dxlib_load_func_object
{
	class LoadGraph
		:public TextureManager::LoadGraphFromSomeSource
	{
	public:
		typedef TextureManager::LoadGraphFromSomeSource::InstType InstType;

	protected:
		const std::string file_path_;
		const int not_use_3D_flag_;

	public:
		LoadGraph(std::string& file_path,std::string& name,int not_use_3D_flag)
			:
			file_path_(file_path),
			TextureManager::LoadGraphFromSomeSource(name),
			not_use_3D_flag_(not_use_3D_flag)
			{}
		
		InstType operator()()
			{
				return ::DxLib::LoadGraph(file_path_.c_str(),not_use_3D_flag_);
			}
	};

	class LoadDivGraph
		:public TextureManager::LoadGraphFromSomeSource
	{
	public:
		typedef TextureManager::LoadGraphFromSomeSource::InstType InstType;

	protected:
		const std::string		file_path_;
		InstType*				buf_begin_;

		const WORD				all_num_;
		const WORD				x_num_;
		const WORD				y_num_;
		const WORD				x_size_;
		const WORD				y_size_;
		const int				not_use_3D_flag_;

	public:
		LoadDivGraph
			(
				const std::string& file_path,
				std::string& name,
				TextureManager::TextureSetType::InstType* buf_begin,
				WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,
				int not_use_3D
			)
			:
				file_path_(file_path),
				TextureManager::LoadGraphFromSomeSource(name),
				buf_begin_(buf_begin),
				all_num_(all_num),x_num_(x_num),y_num_(y_num),x_size_(x_size),y_size_(y_size),
				not_use_3D_flag_(not_use_3D)
			{}
		
		InstType operator()()
			{
				return ::DxLib::LoadDivGraph
					(
						file_path_.c_str(),
						all_num_,x_num_,y_num_,x_size_,y_size_,
						buf_begin_,
						not_use_3D_flag_
					);
			}
	};

	class CreateGraphFromMem
		:public LoadGraph
	{
	protected:
		void* memory_image_;
		DWORD memory_image_size_;

	public:
		CreateGraphFromMem(void* memory_image,DWORD memory_image_size,std::string& name,int not_use_3D_flag)
			:memory_image_(memory_image),memory_image_size_(memory_image_size),LoadGraph(std::string(),name,not_use_3D_flag)
			{}

		InstType operator()()
		{
			return ::DxLib::CreateGraphFromMem(memory_image_,memory_image_size_);
		}
	};

	class CreateDivGraphFromMem
		:public LoadDivGraph
	{
	protected:
		void* memory_image_;
		DWORD memory_image_size_;

	public:
		CreateDivGraphFromMem
			(
				void* memory_image,DWORD memory_image_size,
				std::string& name,
				TextureManager::TextureSetType::InstType* buf_begin,
				WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,
				int not_use_3D
			)
			:
			memory_image_(memory_image),memory_image_size_(memory_image_size),
			LoadDivGraph("",name,buf_begin,all_num,x_num,y_num,x_size,y_size,not_use_3D)
			{}

		InstType operator()()
		{
			return ::DxLib::CreateDivGraphFromMem
				(
					memory_image_,memory_image_size_,
					all_num_,x_num_,y_num_,x_size_,y_size_,buf_begin_,
					not_use_3D_flag_
				);
		}
	};
}

TextureManager::TextureInstType TextureManager::LoadWithWarning(TextureManager::LoadGraphFromSomeSource* op)
{
	int gr_handle=0,i=0;
	do{
		i++;
		if(i>retry_time_)
		{
			std::string error_message_and_name=std::string(error_message_.c_str())+op->texture_name_;
			kvarco::OutputLog(error_message_and_name.c_str());
			int result=
				::MessageBox(NULL,error_message_and_name.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(-1);
			else					{ i=0; gr_handle=0; }
		}

		gr_handle=(*op)();
	}while(gr_handle==-1);

	return gr_handle;
}

TextureManager::TextureInstType TextureManager::DivLoadWithWarning(TextureManager::LoadGraphFromSomeSource* op)
{
	int load_result=0,i=0;

	do{
		i++;
		if(i>retry_time_)
		{
			std::string error_message_and_name=std::string(error_message_.c_str())+op->texture_name_;
			kvarco::OutputLog(error_message_and_name.c_str());
			int result=
				::MessageBox(GetMainWindowHandle(),error_message_and_name.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(-1);
			else					{ i=0; load_result=0; }
		}

		load_result=(*op)();
	}while(load_result==-1);

	return load_result;
}

#define RETURN_ALLOCED_TEXTURE_PTR(inst,type,ptr_type)	\
	ptr_type tmp(new type(inst));						\
	if(!name.empty())									\
		named_texture_map_.insert(std::make_pair(name,boost::static_pointer_cast<TextureBaseType>(tmp)));\
	else												\
		unnamed_texture_vector_.push_back(boost::static_pointer_cast<TextureBaseType>(tmp));\
	return ptr_type(tmp);

//
TextureManager::TexturePtr TextureManager::Load(fsys::path& file,std::string& name,bool not_use_3D)
{
	kvarco::OutputLog("LoadImageFile : %s (Name : %s)",file.string().c_str(),name.c_str());
	
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);
	
	//from archive?
	if(ArchiveManager::GetInst()->Exists(path))
	{
		//yes! >from archive?
		boost::intmax_t size=0;
		ArchiveManager::AllocatedMemoryType memory_image=
			ArchiveManager::GetInst()->UnzipToAllocatedMemory(path,&size);

		LoadedTextureFromArchive tmp(file,name,LoadedTextureFromArchive::LOAD);
		file_path_from_archive_.push_back(tmp);
		return Load(memory_image.get(),size,name,not_use_3D);
	}

	//no >from archive?

	//load
	dxlib_load_func_object::LoadGraph op(const_cast<std::string&>(path.string()),name,not_use_3D);
	TextureType::InstType inst=LoadWithWarning(&op);

	TexturePtr tmp(new TextureType(inst));
	TextureBasePtr tmp2=boost::static_pointer_cast<TextureBaseType>(tmp);
	if(!name.empty())
		named_texture_map_.insert(std::make_pair(name,tmp2));
	else
		unnamed_texture_vector_.push_back(tmp2);
	return tmp;
}

TextureManager::TexturePtr TextureManager::Load(void* src,DWORD size,std::string& name,bool not_use_3D)
{
	//load
	dxlib_load_func_object::CreateGraphFromMem op(src,size,name,not_use_3D);
	TextureType::InstType inst=LoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(inst,TextureType,TexturePtr)
}

TextureManager::TextureSetPtr TextureManager::DivLoad(fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	kvarco::OutputLog("LoadImageFile : %s (Name : %s)",file.string().c_str(),name.c_str());
	
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	if(ArchiveManager::GetInst()->Exists(path))
	{
		//yes! >from archive?
		boost::intmax_t size=0;
		ArchiveManager::AllocatedMemoryType memory_image=
			ArchiveManager::GetInst()->UnzipToAllocatedMemory(path,&size);

		LoadedTextureFromArchive tmp
			(
				file,name,
				LoadedTextureFromArchive::DIV_LOAD,
				all_num,x_num,y_num,x_size,y_size
			);
		file_path_from_archive_.push_back(tmp);
		return DivLoad(memory_image.get(),size,name,all_num,x_num,y_num,x_size,y_size,not_use_3D);
	}

	//load
	std::vector<TextureInstType> buf(all_num,-1);
	dxlib_load_func_object::LoadDivGraph op(file.string(),name,&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(buf,TextureSetType,TextureSetPtr)
}

TextureManager::TextureSetPtr TextureManager::DivLoad(void* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::CreateDivGraphFromMem op(src,size,name,&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(buf,TextureSetType,TextureSetPtr);
}

TextureManager::AnimationPtr TextureManager::LoadAnimation(TypeOfAnimation anime_type,fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	kvarco::OutputLog("LoadImageFile : %s (Name : %s)",file.string().c_str(),name.c_str());
	
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	if(ArchiveManager::GetInst()->Exists(path))
	{
		//yes! >from archive?
		boost::intmax_t size=0;
		ArchiveManager::AllocatedMemoryType memory_image=
			ArchiveManager::GetInst()->UnzipToAllocatedMemory(path,&size);

		LoadedTextureFromArchive tmp
			(
				file,name,
				LoadedTextureFromArchive::DIV_LOAD,
				all_num,x_num,y_num,x_size,y_size
			);
		file_path_from_archive_.push_back(tmp);
		return LoadAnimation(anime_type,memory_image.get(),size,name,all_num,x_num,y_num,x_size,y_size,not_use_3D);
	}

	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::LoadDivGraph op(file.string(),name,&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	DivLoadWithWarning(&op);

	AnimationPtr tmp(new AnimationType(buf,anime_type));
	TextureBasePtr tmp2=boost::static_pointer_cast<TextureBaseType>(tmp);
	if(!name.empty())
		named_texture_map_.insert(std::make_pair(name,tmp2));
	else
		unnamed_texture_vector_.push_back(tmp2);

	return tmp;
}

TextureManager::AnimationPtr TextureManager::LoadAnimation(TypeOfAnimation anime_type,void* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::CreateDivGraphFromMem op(src,size,name,&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	DivLoadWithWarning(&op);

	AnimationPtr tmp(new AnimationType(buf,anime_type));
	TextureBasePtr tmp2=boost::static_pointer_cast<TextureBaseType>(tmp);
	if(!name.empty())
		named_texture_map_.insert(std::make_pair(name,boost::static_pointer_cast<TextureBaseType>(tmp)));
	else
		unnamed_texture_vector_.push_back(tmp2);

	return tmp;
}

TextureManager::TexturePtr TextureManager::CutTexture(const TextureManager::TexturePtrX& old_texture,const std::string& new_name,TextureManager::RectType cut_range)
{
	TextureInstType new_inst=DxLib::DerivationGraph
		(
			cut_range.left_,
			cut_range.top_,
			cut_range.right_-cut_range.left_,
			cut_range.bottom_-cut_range.top_,
			old_texture->Get()
		);

	TexturePtr tmp(new TextureType(new_inst));
	TextureBasePtr tmp2=boost::static_pointer_cast<TextureBaseType>(tmp);
	if(!new_name.empty())
		named_texture_map_.insert(std::make_pair(new_name,tmp2));
	else
		unnamed_texture_vector_.push_back(tmp2);

	return tmp;
}

TextureManager::TexturePtr TextureManager::CutTexture(const std::string& old_name,const std::string& new_name,RectType cut_range)
{
	return CutTexture(GetAsTextureX(old_name.c_str()),new_name,cut_range);
}

void TextureManager::UnloadTexture(const std::string& name)
{
	NamedTextureMapType::iterator i=named_texture_map_.find(name);
	i->second->Unload();
	named_texture_map_.erase(i);
}

TextureManager::TexturePtrX TextureManager::CutTextureX(const TextureManager::TextureNameType& old_name,const TextureManager::TextureNameType& new_name,TextureManager::RectTypePtrX cut_range)
{
	return CutTexture(old_name->c_str(),new_name->c_str(),*cut_range).get();
}

void TextureManager::UnloadTextureX(const TextureManager::TextureNameType& name)
{
	NamedTextureMapType::iterator i=named_texture_map_.find(name->c_str());
	i->second->Unload();
	named_texture_map_.erase(i);
}

TextureManager::TexturePtrX TextureManager::LoadX(PathType& file,TextureManager::TextureNameType& name,bool not_use_3D)
{
	fsys::path tmp_path(file->c_str());
	std::string tmp_name(name->c_str());
	TexturePtr tmp=Load(tmp_path,tmp_name,not_use_3D);
	TexturePtrX tmpx(tmp.get(),xtal::undeleter);
	return tmpx;
}

TextureManager::TextureSetPtrX TextureManager::DivLoadX(TextureManager::PathType& file,TextureManager::TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	fsys::path tmp_path(file->c_str());
	std::string tmp_name(name->c_str());
	return xtal::SmartPtr<TextureSetType>(DivLoad(tmp_path,tmp_name,all_num,x_num,y_num,x_size,y_size,not_use_3D).get(),xtal::undeleter);
}

TextureManager::AnimationPtrX TextureManager::LoadAnimationX(TextureManager::TypeOfAnimation anime_type,TextureManager::PathType& file,TextureManager::TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	fsys::path tmp_path(file->c_str());
	std::string tmp_name(name->c_str());
	AnimationPtr tmp_ptr=LoadAnimation(anime_type,tmp_path,tmp_name,all_num,x_num,y_num,x_size,y_size,not_use_3D);
	return xtal::SmartPtr<AnimationType>(tmp_ptr.get(),xtal::undeleter);
}

TextureManager::TextureBasePtr TextureManager::GetRawPtr(const std::string& name)
{
	NamedTextureMapType::iterator ite=named_texture_map_.find(const_cast<std::string&>(name));
	return (ite!=named_texture_map_.end()) ? ite->second : TextureBasePtr();
}

TextureManager::TextureBasePtr TextureManager::GetRawPtr(const TextureManager::TextureNameType& name)
{
	NamedTextureMapType::iterator ite=named_texture_map_.find(const_cast<TextureManager::TextureNameType&>(name)->c_str());
	return (ite!=named_texture_map_.end()) ? ite->second : TextureBasePtr();
}

bool TextureManager::IsLoaded(const std::string& name)
{
	return GetRawPtr(name)!=NULL;
}

bool TextureManager::IsLoadedX(const TextureManager::TextureNameType& name)
{
	return GetRawPtr(name)!=NULL;
}

TextureManager::TexturePtr TextureManager::GetAsTexture(const std::string& name)
{
	return boost::dynamic_pointer_cast<TextureType>(GetRawPtr(name));
}

TextureManager::TextureSetPtr TextureManager::GetAsTextureSet(const std::string& name)
{
	return boost::dynamic_pointer_cast<TextureSetType>(GetRawPtr(name));
}

TextureManager::AnimationPtr TextureManager::GetAsAnimation(const std::string& name)
{
	return boost::dynamic_pointer_cast<AnimationType>(GetRawPtr(name));
}

TextureManager::TexturePtrX TextureManager::GetAsTextureX(const TextureManager::TextureNameType& name)
{
	TexturePtr ptr=GetAsTexture(name->c_str());

	if(ptr==NULL) return xtal::null;
	return xtal::SmartPtr<TextureType>(ptr.get(),xtal::undeleter);
}

TextureManager::TextureSetPtrX TextureManager::GetAsTextureSetX(const TextureManager::TextureNameType& name)
{
	TextureSetPtr ptr=GetAsTextureSet(name->c_str());

	if(ptr==NULL) return xtal::null;
	return xtal::SmartPtr<TextureSetType>(ptr.get(),xtal::undeleter);
}


TextureManager::AnimationPtrX TextureManager::GetAsAnimationX(const TextureManager::TextureNameType& name)
{
	AnimationPtr ptr=GetAsAnimation(name->c_str());

	if(ptr==NULL) return xtal::null;
	return xtal::SmartPtr<AnimationType>(ptr.get(),xtal::undeleter);
}

void TextureManager::Reload()
{
	BOOST_FOREACH(LoadedTextureFromArchive& i,file_path_from_archive_)
	{
		TextureBasePtr basic=GetRawPtr(i.name_);
		if(basic==NULL)
		{
			kvarco::OutputLog("%sのリロードを試みましたがこれはまだロードされてませんでした",i.name_.c_str());
			continue;
		}

		fsys::path path=fsys::absolute(i.path_.string(),kvarco::ExePath);

		boost::intmax_t size=0;
		ArchiveManager::AllocatedMemoryType memory_image
			=ArchiveManager::GetInst()->UnzipToAllocatedMemory(i.path_,&size);
		if(!memory_image)
		{
			kvarco::OutputLog("%sのリロードを試みましたが%sが見つかりませんでした",i.name_.c_str(),i.path_.string().c_str());
			continue;
		}

		//reload
		if(i.load_type_==LoadedTextureFromArchive::LOAD)
		{
			TexturePtr tmp=boost::dynamic_pointer_cast<TextureType>(basic);
			if(tmp) DxLib::ReCreateGraphFromMem(memory_image.get(),size,tmp->Get());
		}
		else
		{
			TextureSetPtr tmp_set	=boost::dynamic_pointer_cast<TextureSetType>(basic);
			AnimationPtr tmp_anime	=boost::dynamic_pointer_cast<AnimationType>(basic);

			if(tmp_set)
			{
				DxLib::ReCreateDivGraphFromMem
					(
						memory_image.get(),size,
						i.all_num_,i.x_num_,i.y_num_,i.x_size_,i.y_size_,
						&(*tmp_set->texture_list_.begin())
					);
			}
			else if(tmp_anime)
			{
				DxLib::ReCreateDivGraphFromMem
					(
						memory_image.get(),size,
						i.all_num_,i.x_num_,i.y_num_,i.x_size_,i.y_size_,
						&(*tmp_anime->frame_list_.begin())
					);
			}
		}
	}
}

void TextureManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(TextureManager);

#define Xdef_param_alias(name,x) it->def(Xid(name),xtal::method(x)

	Xdef_param_alias(RetrySetting		,&RetrySettingX)		->param(2,Xid(error_message),""));
	Xdef_method(NotUseRetry);

	Xdef_param_alias(Load				,&Self::LoadX)			->param(3,Xid(not_use_3D),true));
	Xdef_param_alias(DivLoad			,&Self::DivLoadX)		->param(8,Xid(not_use_3D),true));
	Xdef_param_alias(LoadAnimation		,&Self::LoadAnimationX)	->param(9,Xid(not_use_3D),true));
	Xdef_method_alias(IsLoaded			,&IsLoadedX);
	Xdef_method_alias(CutTexture		,&CutTextureX);
	Xdef_method_alias(UnloadTexture		,&UnloadTextureX);

	Xdef_method_alias(GetAsTexture		,&GetAsTextureX);
	Xdef_method_alias(GetAsTextureSet	,&GetAsTextureSetX);
	Xdef_method_alias(GetAsAnimation	,&GetAsAnimationX);

#undef Xdef_param_alias
}
