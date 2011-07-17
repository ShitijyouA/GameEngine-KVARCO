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
		LoadGraph(std::string& file_path,int not_use_3D_flag)
			:file_path_(file_path),not_use_3D_flag_(not_use_3D_flag)
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
		LoadDivGraph(const std::string& file_path,TextureManager::TextureSetType::InstType* buf_begin,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,int not_use_3D)
			:
				file_path_(file_path),
				buf_begin_(buf_begin),
				all_num_(all_num),x_num_(x_num),y_num_(y_num),x_size_(x_size),y_size_(y_size),
				not_use_3D_flag_(not_use_3D)
			{}
		
		InstType operator()()
			{
				return ::DxLib::LoadDivGraph(file_path_.c_str(),all_num_,x_num_,y_num_,x_size_,y_size_,buf_begin_,not_use_3D_flag_);
			}
	};

	class CreateGraphFromMem
		:public LoadGraph
	{
	protected:
		void* memory_image_;
		DWORD memory_image_size_;

	public:
		CreateGraphFromMem(void* memory_image,DWORD memory_image_size,int not_use_3D_flag)
			:memory_image_(memory_image),memory_image_size_(memory_image_size),LoadGraph(std::string(),not_use_3D_flag)
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
		CreateDivGraphFromMem(void* memory_image,DWORD memory_image_size,TextureManager::TextureSetType::InstType* buf_begin,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,int not_use_3D)
			:
			memory_image_(memory_image),memory_image_size_(memory_image_size),
			LoadDivGraph("",buf_begin,all_num,x_num,y_num,x_size,y_size,not_use_3D)
			{}

		InstType operator()()
		{
			return ::DxLib::CreateDivGraphFromMem
				(
					memory_image_,memory_image_size_,
					all_num_,x_num_,y_num_,x_size_,y_size_,buf_begin_,not_use_3D_flag_
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
			int result=
				::MessageBox(NULL,error_message_.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(0);
			else					gr_handle=i=0;
		}

		//gr_handle=::DxLib::LoadGraph(file.string().c_str(),not_use_3D ? 1:0);
		gr_handle=(*op)();
	}while(gr_handle==-1);

	return gr_handle;
}

TextureManager::TextureInstType TextureManager::DivLoadWithWarning(TextureManager::LoadGraphFromSomeSource* op)
{
	int result=0,i=0;
	bool go=false;

	do{
		i++;
		if(i>retry_time_)
		{
			int result=
				::MessageBox(GetMainWindowHandle(),error_message_.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(0);
			else					{ go=false; i=0; result=0; }
		}

		//result=DxLib::LoadDivGraph(file.string().c_str(),all_num,x_num,y_num,x_size,y_size,&(*buf.begin()),not_use_3D? 1:0);
		result=(*op)();
	}while(result==-1);

	return result;
}

#define RETURN_ALLOCED_TEXTURE_PTR(inst,type,ptr_type)				\
	type* tmp=new type(inst);										\
	if(name.empty()) return ptr_type(tmp);							\
	named_texture_map_.insert(std::make_pair(name,ptr_type(tmp)));	\
	return ptr_type(tmp);

//
TextureManager::TexturePtr TextureManager::Load(fsys::path& file,std::string& name,bool not_use_3D)
{
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	boost::intmax_t size=0;
	ArchiveManager::AllocatedMemoryType memory_image=ArchiveManager::GetInst()->UnzipToAllocatedMemory(path,&size);

	//yes! >from archive?
	if(!memory_image) return Load(memory_image.get(),size,name,not_use_3D);

	//no >from archive?

	//load
	dxlib_load_func_object::LoadGraph op(const_cast<std::string&>(path.string()),not_use_3D);
	TextureType::InstType inst=LoadWithWarning(&op);
	
	RETURN_ALLOCED_TEXTURE_PTR(inst,TextureType,TexturePtr);
}

TextureManager::TexturePtr TextureManager::Load(void* src,DWORD size,std::string& name,bool not_use_3D)
{
	//load
	dxlib_load_func_object::CreateGraphFromMem op(src,size,not_use_3D);
	TextureType::InstType inst=LoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(inst,TextureType,TexturePtr);
}

TextureManager::TextureSetPtr TextureManager::DivLoad(fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	boost::intmax_t size=0;
	ArchiveManager::AllocatedMemoryType memory_image=ArchiveManager::GetInst()->UnzipToAllocatedMemory(path,&size);

	//yes! >from archive?
	if(!memory_image) return DivLoad(memory_image.get(),size,name,all_num,x_num,y_num,x_size,y_size,not_use_3D);

	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::LoadDivGraph op(file.string(),&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(buf,TextureSetType,TextureSetPtr);
}

TextureManager::TextureSetPtr TextureManager::DivLoad(void* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::CreateDivGraphFromMem op(src,size,&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(buf,TextureSetType,TextureSetPtr);
}

TextureManager::AnimationPtr TextureManager::LoadAnimation(TypeOfAnimation anime_type,fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	boost::intmax_t size=0;
	ArchiveManager::AllocatedMemoryType memory_image=ArchiveManager::GetInst()->UnzipToAllocatedMemory(path,&size);

	//yes! >from archive?
	if(!memory_image) return LoadAnimation(anime_type,memory_image.get(),size,name,all_num,x_num,y_num,x_size,y_size,not_use_3D);

	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::LoadDivGraph op(file.string(),&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	AnimationType* tmp=new AnimationType(buf,anime_type);
	if(name.empty()) return AnimationPtr(tmp);
	named_texture_map_.insert(std::make_pair(name,AnimationPtr(tmp)));
	return AnimationPtr(tmp);
}

TextureManager::AnimationPtr TextureManager::LoadAnimation(TypeOfAnimation anime_type,void* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::CreateDivGraphFromMem op(src,size,&(*buf.begin()),all_num,x_num,y_num,x_size,y_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	AnimationType* tmp=new AnimationType(buf,anime_type);
	if(name.empty()) return AnimationPtr(tmp);
	named_texture_map_.insert(std::make_pair(name,AnimationPtr(tmp)));
	return AnimationPtr(tmp);
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

	TextureType* tmp=new TextureType(new_inst);
	if(new_name.empty()) return TexturePtr(tmp);
	named_texture_map_.insert(std::make_pair(new_name,TexturePtr(tmp)));
	return TexturePtr(tmp);
}

TextureManager::TexturePtr TextureManager::CutTexture(const std::string& old_name,const std::string& new_name,RectType cut_range)
{
	return CutTexture(GetAsTexture(old_name.c_str()),new_name,cut_range);
}

void TextureManager::UnloadTexture(const std::string& name)
{
	NamedTextureMapType::iterator i=GetRawIterator(name);
	i->second->Unload();
	named_texture_map_.erase(i);
}

TextureManager::TexturePtrX TextureManager::CutTextureX(const TextureManager::TextureNameType& old_name,const TextureManager::TextureNameType& new_name,TextureManager::RectTypePtrX cut_range)
{
	return CutTexture(old_name->c_str(),new_name->c_str(),*cut_range).get();
}

void TextureManager::UnloadTextureX(const TextureManager::TextureNameType& name)
{
	NamedTextureMapType::iterator i=GetRawIterator(name);
	i->second->Unload();
	named_texture_map_.erase(i);
}

TextureManager::TexturePtrX TextureManager::LoadX(PathType& file,TextureManager::TextureNameType& name,bool not_use_3D)
{
	fsys::path tmp_path(file->c_str());
	std::string tmp_name(name->c_str());
	return xtal::SmartPtr<TextureType>(Load(tmp_path,tmp_name,not_use_3D).get(),xtal::undeleter);
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

TextureManager::NamedTextureMapType::iterator TextureManager::GetRawIterator(const std::string& name)
{
	return named_texture_map_.find(const_cast<std::string&>(name));
}

TextureManager::NamedTextureMapType::iterator TextureManager::GetRawIterator(const TextureManager::TextureNameType& name)
{
	return named_texture_map_.find(const_cast<TextureManager::TextureNameType&>(name)->c_str());
}

bool TextureManager::IsLoaded(const std::string& name)
{
	return GetRawIterator(name)!=named_texture_map_.end();
}

bool TextureManager::IsLoadedX(const TextureManager::TextureNameType& name)
{
	return GetRawIterator(name)!=named_texture_map_.end();
}

TextureManager::TexturePtrX TextureManager::GetAsTexture(const TextureManager::TextureNameType& name)
{
	TextureType* tmp=NULL;
	try
	{
		tmp=dynamic_cast<TextureType*>(GetRawIterator(name)->second.get());
	}
	catch(...)
	{}

	return xtal::SmartPtr<TextureType>(tmp,xtal::undeleter);
}

TextureManager::TextureSetPtrX TextureManager::GetAsTextureSet(const TextureManager::TextureNameType& name)
{
	TextureSetType* tmp=NULL;
	try
	{
		tmp=dynamic_cast<TextureSetType*>(GetRawIterator(name)->second.get());
	}
	catch(...)
	{}

	return xtal::SmartPtr<TextureSetType>(tmp,xtal::undeleter);
}

TextureManager::AnimationPtrX TextureManager::GetAsAnimation(const TextureManager::TextureNameType& name)
{
	AnimationType* tmp=NULL;
	try
	{
		tmp=dynamic_cast<AnimationType*>(GetRawIterator(name)->second.get());
	}
	catch(...)
	{}

	return xtal::SmartPtr<AnimationType>(tmp,xtal::undeleter);
}

void TextureManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(TextureManager);

#define Xdef_param_alias(name,x) xtal::lib()->def(Xid(name),xtal::fun(x)

	Xdef_param_alias(RetrySetting		,&RetrySettingX)	->param(3,Xid(error_message),""));
	Xdef_method(NotUseRetry);

	Xdef_param_alias(Load				,&LoadX)			->param(3,Xid(not_use_3D),true));
	Xdef_param_alias(DivLoad			,&DivLoadX)			->param(8,Xid(not_use_3D),true));
	Xdef_param_alias(LoadAnimation		,&LoadAnimationX)	->param(9,Xid(not_use_3D),true));
	Xdef_method_alias(IsLoaded			,&IsLoadedX);
	Xdef_method_alias(CutTexture		,&CutTextureX);
	Xdef_method_alias(UnloadTextureX	,&UnloadTextureX);

	Xdef_method(GetAsTexture);
	Xdef_method(GetAsTextureSet);
	Xdef_method(GetAsAnimation);

#undef Xdef_param_alias
}
