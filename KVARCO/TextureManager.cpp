#include "TextureManager.h"
#include "ArchiveManager.h"

namespace dxlib_load_func_object
{
	struct LoadGraph
		:public TextureManager::LoadGraphAnySource
	{
	public:
		typedef TextureManager::LoadGraphAnySource::InstType InstType;

	private:
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

	struct LoadDivGraph
		:public TextureManager::LoadGraphAnySource
	{
	public:
		typedef TextureManager::LoadGraphAnySource::InstType InstType;

	private:
		const std::string		file_path_;
		const TextureInstType*	buf_begin_;

		const WORD				all_num_;
		const WORD				x_num_;
		const WORD				y_num_;
		const WORD				x_size_;
		const WORD				y_size_;
		const WORD				not_use_3D_flag_;

	public:
		LoadGraph(std::string& file_path,TextureSetType* buf_begin,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,int not_use_3D)
			:
				file_path_(file_path),
				buf_begin_(buf_begin),
				all_num_(all_num),x_num_(x_num),y_num_(y_num),x_size_(x_size),y_size_(y_size),
				not_use_3D_flag_(not_use_3D_flag)
			{}
		
		InstType operator()()
			{
				return ::DxLib::LoadDivGraph(file_path_.c_str(),all_num_,x_num_,y_num_,x_size_,y_size_,buf_begin_,not_use_3D_flag_);
			}
	};

	struct CreateGraphFromMem
		:public LoadGraph
	{
		void* memory_image_;
		DWORD memory_image_size_;

	public:
		InstType operator()()
		{
			return ::DxLib::CreateGraphFromMem
				(
					memory_image_,memory_image_size_,
					file_path_.c_str(),not_use_3D_flag_
				);
		}
	};

	struct CreateDivGraphFromMem
		:public LoadDivGraph
	{
		void* memory_image_;
		DWORD memory_image_size_;

	public:
		InstType operator()()
		{
			return ::DxLib::CreateDivGraphFromMem
				(
					memory_image_,memory_image_size_,
					file_path_.c_str(),all_num_,x_num_,y_num_,x_size_,y_size_,buf_begin_,not_use_3D_flag_
				);
		}
	};
}

TextureManager::LoadWithWarning(const TextureManager::LoadGraphAnySource* op)
{
	int gr_handle=0,i=0;
	do{
		i++;
		if(i>retry_time_)
		{
			std::string tmp=error_message_+file.string();
			int result=
				::MessageBox(NULL,tmp.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(0);
			else					gr_handle=i=0;
		}

		//gr_handle=::DxLib::LoadGraph(file.string().c_str(),not_use_3D ? 1:0);
		gr_handle=op();
	}while(gr_handle==-1);

	return gr_handle;
}

TextureManager::DivLoadWithWarning(const TextureManager::LoadGraphAnySource* op)
{
	int result=0,i=0;
	bool go=false;

	do{
		i++;
		if(i>RetryTime)
		{
			std::string tmp=error_message_+file.string();
			int result=
				::MessageBox(GetMainWindowHandle(),tmp.c_str(),"エラーが発生しました",MB_RETRYCANCEL | MB_ICONWARNING);
			if(result==IDCANCEL)	exit(0);
			else					{ go=false; i=0; result=0; }
		}

		//result=DxLib::LoadDivGraph(file.string().c_str(),all_num,x_num,y_num,x_size,y_size,&(*buf.begin()),not_use_3D? 1:0);
		result=op();
	}while(result==-1);

	return result;
}

#define RETURN_ALLOCED_TEXTURE_PTR(inst,type,ptr_type)					\
	type* tmp=new type(inst);											\
	if(name.empty()) return ptr_type(tmp);								\
	named_texture_map_.insert(std::make_pair(name,ptr_type(tmp)));		\
	return ptr_type(tmp);

//
TextureManager::TexturePtr TextureManager::Load(fsys::path& file,std::string& name,bool not_use_3D)
{
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	typedef std::allocator<char> allocator;

	boost::intmax_t size=0;
	ResultType memory_image=ArchiveManager::GetInst()->UnzipToAllocatedMemory<char,allocator>(path,&size);

	//yes! >from archive?
	if(!memory_image) return Load(memory_image->get(),size,name,not_use_3D);

	//no >from archive?

	//load
	dxlib_load_func_object::LoadGraph op(path.string(),not_use_3D);
	TextureType::InstType inst=LoadWithWarning(&op);
	
	RETURN_ALLOCED_TEXTURE_PTR(inst,TextureType,TexturePtr);
}

TextureManager::TexturePtr TextureManager::Load(BYTE* src,DWORD size,std::string& name,bool not_use_3D)
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
	typedef std::allocator<char> allocator;

	boost::intmax_t size=0;
	ResultType memory_image=ArchiveManager::GetInst()->UnzipToAllocatedMemory<char,allocator>(path,&size);

	//yes! >from archive?
	if(!memory_image) return Load(memory_image->get(),size,name,not_use_3D);

	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::LoadDivGraph op(file.string(),&(*buf.beign()),all_num,x_num,y_num,x_sizey_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(buf,TextureSetType,TextureSetPtr);
}

TextureManager::TextureSetPtr TextureManager::DivLoad(BYTE* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D)
{
	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::CreateDivGraphFromMem op(src,size,file.string(),&(*buf.beign()),all_num,x_num,y_num,x_sizey_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR(buf,TextureSetType,TextureSetPtr);
}

TextureManager::AnimationPtr TextureManager::LoadAnimation(TypeOfAnimation anime_type,fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true)
{
	fsys::path path=fsys::absolute(file.string(),kvarco::ExePath);

	//from archive?
	typedef std::allocator<char> allocator;

	boost::intmax_t size=0;
	ResultType memory_image=ArchiveManager::GetInst()->UnzipToAllocatedMemory<char,allocator>(path,&size);

	//yes! >from archive?
	if(!memory_image) return LoadAnimation(anime_type,memory_image->get(),size,name,all_num,x_num,y_num,x_sizey_size,not_use_3D);

	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::LoadDivGraph op(file.string(),&(*buf.beign()),all_num,x_num,y_num,x_sizey_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR((buf,anime_type),AnimationType,AnimationPtr);
}

TextureManager::AnimationPtr TextureManager::LoadAnimation(TypeOfAnimation anime_type,BYTE* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true)
{
	//load
	std::vector<TextureInstType> buf(all_num);
	dxlib_load_func_object::CreateDivGraphFromMem op(src,size,file.string(),&(*buf.beign()),all_num,x_num,y_num,x_sizey_size,not_use_3D);
	TextureType::InstType inst=DivLoadWithWarning(&op);

	RETURN_ALLOCED_TEXTURE_PTR((buf,anime_type),AnimationType,AnimationPtr);
}

TextureManager::TexturePtr TextureManager::CutTexture(const std::string& old_name,const std::string& new_name,RectType cut_range)
{
	TextureTypePtrX texture(GetAsTexture(old_name.c_str()));
	TextureInstType new_inst=DxLib::DerivationGraph(x,y,w,h,texture->Get());
	
	RETURN_ALLOCED_TEXTURE_PTR(new_inst,AnimationType,AnimationPtr);
}

void TextureManager::UnloadTexture(const std::string& name)
{
	named_texture_map_.erase(name.c_str());
}

TextureManager::TexturePtrX TextureManager::CutTextureX(const TextureManager::TextureNameType& old_name,const TextureManager::TextureNameType& new_name,TextureManager::RectTypePtr cut_range)
{
	return TexturePtrX(CutTexture(old_name.c_str(),new_name.c_str(),*cut_range),xtal::undeleter);
}

void TextureManager::UnloadTextureX(const TextureManager::TextureNameType& name)
{
	named_texture_map_.erase(name);
}

TextureManager::TextureTypeX TextureManager::LoadX(PathType& file,TextureManager::TextureNameType& name,bool not_use_3D=true)
{
	return TexturePtrX(Load(file.c_str(),name.c_str(),not_use_3D),xtal::undeleter);
}

TextureManager::TextureSetTypeX TextureManager::DivLoadX(TextureManager::PathType& file,TextureManager::TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true)
{
	return TextureSetPtrX(DivLoad(file.c_str(),name.c_str(),all_num,x_num,y_num,x_size,y_size,not_use_3D),xtal::undeleter);
}

TextureManager::AnimationPtrX TextureManager::LoadAnimationX(TextureManager::TypeOfAnimation anime_type,TextureManager::PathType& file,TextureManager::TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true)
{
	return AnimationPtrX(LoadAnimation(anime_type,file.c_str(),name.c_str(),all_num,x_num,y_num,x_size,y_size,not_use_3D),xtal::undeleter);
}

NamedTextureMapType::iterator TextureManager::GetRawIterator(const std::string& name)
{
	return named_texture_map_.find(const_cast<std::string&>(name).c_str());
}

NamedTextureMapType::iterator GetRawIterator(const xtal::String& name)
{
	return named_texture_map_.find(const_cast<xtal::String&>(name));
}

bool TextureManager::IsLoaded(const std::string& name)
{
	return GetRawIterator(name)!=named_texture_map_.end();
}

bool TextureManager::IsLoadedX(const xtal::String& name)
{
	return GetRawIterator(name)!=named_texture_map_.end();
}

TextureTypePtrX TextureManager::GetAsTexture(const xtal::String& name)
{
	return TextureTypePtrX(GetRawIterator(name)->get(),xtal::undeleter);
}

TextureSetPtrX TextureManager::GetAsTextureSet(const xtal::String& name)
{
	return TextureSetPtrX(GetRawIterator(name)->get(),xtal::undeleter);
}

AnimationPtrX TextureManager::GetAsAnimation(const xtal::String& name)
{
	return AnimationPtrX(GetRawIterator(name)->get(),xtal::undeleter);
}

void TextureManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(TextureManager);

#define Xdef_param_alias(x,name) xtal::lib()->def(Xid(name),xtal::fun(x)

	Xdef_param_alias(RetrySetting		,&RetrySettingX)	->param(3,Xid(error_message),"");
	Xdef_method(NotUseRetry);

	Xdef_param_alias(Load				,&LoadX)			->param(3,Xid(not_use_3D),true);
	Xdef_param_alias(DivLoad			,&DivLoadX)			->param(8,Xid(not_use_3D),true);
	Xdef_param_alias(LoadAnimation		,&LoadAnimationX)	->param(9,Xid(not_use_3D),true);
	Xdef_method_alias(IsLoaded			,&IsLoadedX);
	Xdef_method_alias(CutTexture		,&CutTextureX);
	Xdef_method_alias(UnloadTextureX	,&UnloadTextureX);

	Xdef_method(GetAsTexture);
	Xdef_method(GetAsTextureSet);
	Xdef_method(GetAsAnimation);

#undef Xdef_param_alias
}
