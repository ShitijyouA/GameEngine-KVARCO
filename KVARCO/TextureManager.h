#pragma once

#include "Texture.h"
#include "TextureParam.h"

class TextureManager
{
public:
	//config
#ifdef __DXLIB
	typedef int TextureInstType;
#else
	typedef void TextureInstType;	//error
#endif

	//for xtal
#if 1
	typedef xtal::StringPtr PathType;
	typedef xtal::StringPtr TextureNameType;
#else
	typedef fsys::path	PathType;
	typedef std::string TextureNameType;
#endif
	//the above is config

	typedef concept::Texture<TextureInstType>						TextureBaseType;
	typedef boost::shared_ptr<TextureBaseType>						TextureBasePtr;
	typedef boost::unordered_map<std::string,TextureBasePtr>		NamedTextureMapType;
	
	typedef texture::BasicTexture<TextureInstType>					TextureType;
	typedef texture::BasicTextureSet<TextureInstType>				TextureSetType;
	typedef texture::BasicAnimation<TextureInstType>				AnimationType;
	typedef AnimationType::TypeOfAnimation::type					TypeOfAnimation;

	typedef boost::shared_ptr<TextureType>							TexturePtr;
	typedef boost::shared_ptr<TextureSetType>						TextureSetPtr;
	typedef boost::shared_ptr<AnimationType>						AnimationPtr;

	typedef xtal::SmartPtr<TextureType>								TexturePtrX;
	typedef xtal::SmartPtr<TextureSetType>							TextureSetPtrX;
	typedef xtal::SmartPtr<AnimationType>							AnimationPtrX;

	typedef tRect<TextureType::SizeInstType>						RectType;
	typedef xtal::SmartPtr<RectType>								RectTypePtrX;

	typedef TextureManager*											TextureMngrPtr;

	struct LoadGraphFromSomeSource
	{
		typedef TextureManager::TextureInstType InstType;
		virtual InstType operator()()=0;
	};

private:
	//for warning
	xtal::String error_message_;
	BYTE retry_time_;

	NamedTextureMapType named_texture_map_;

	TextureInstType LoadWithWarning(LoadGraphFromSomeSource* op);
	TextureInstType DivLoadWithWarning(LoadGraphFromSomeSource* op);

	NamedTextureMapType::iterator GetRawIterator(const std::string& name);
	NamedTextureMapType::iterator GetRawIterator(const TextureNameType& name);

public:
	void RetrySetting(BYTE retry_time=10,const std::string error_message="")
		{
			retry_time_		=retry_time;
			error_message_	=error_message.c_str();
		}

	void RetrySettingX(BYTE retry_time=10,xtal::StringPtr error_message="")
		{
			retry_time_		=retry_time;
			error_message_	=*error_message;
		}

	void NotUseRetry()
		{
			retry_time_=1;
		}

	//load
	TexturePtr Load(fsys::path& file,std::string& name,bool not_use_3D=true);
	TexturePtr Load(void* src,DWORD size,std::string& name,bool not_use_3D=true);

	TextureSetPtr DivLoad(fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	TextureSetPtr DivLoad(void* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);

	AnimationPtr LoadAnimation(TypeOfAnimation anime_type,fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	AnimationPtr LoadAnimation(TypeOfAnimation anime_type,void* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	
	//cut
	TexturePtr CutTexture(const TexturePtrX& old_texture,const std::string& new_name,RectType cut_range);
	TexturePtr CutTexture(const std::string& old_name,const std::string& new_name,RectType cut_range);

	//unload
	void UnloadTexture(const std::string& name);

	bool IsLoaded(const std::string& name);
	
	//Load for xtal
	TexturePtrX LoadX(PathType& file,TextureNameType& name,bool not_use_3D=true);
	TextureSetPtrX DivLoadX(PathType& file,TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	AnimationPtrX LoadAnimationX(TypeOfAnimation anime_type,PathType& file,TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	TexturePtrX CutTextureX(const TextureNameType& old_name,const TextureNameType& new_name,RectTypePtrX cut_range);
	void UnloadTextureX(const TextureNameType& name);

	//for xtal
	bool IsLoadedX(const TextureNameType& name);
	TexturePtrX GetAsTexture(const TextureNameType& name);
	TextureSetPtrX GetAsTextureSet(const TextureNameType& name);
	AnimationPtrX GetAsAnimation(const TextureNameType& name);

private:
	FRIENDS_FOR_XTAL(TextureManager)
	TextureManager()
		{
			RetrySetting();
		}

public:
	static TextureMngrPtr GetInst()
		{
			static TextureManager Inst;
			return &Inst;
		}

	static void bind(xtal::ClassPtr it);

	void ReleaseAllTexture()
		{
			for(NamedTextureMapType::iterator i=named_texture_map_.begin(); i!=named_texture_map_.end();++i)
				i->second->Unload();
		}

	void Release()
	{
		ReleaseAllTexture();
	}
};
