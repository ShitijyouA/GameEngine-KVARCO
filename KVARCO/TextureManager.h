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
	typedef xtal::String PathType;
	typedef xtal::String TextureNameType;
#else
	typedef fsys::path	PathType;
	typedef std::string TextureNameType;
#endif
	//the above is config

	typedef concept::Texture										TextureBaseType;
	typedef boost::shared_ptr<TextureBaseType>						TextureBasePtr;
	typedef boost::unordered_map<TextureNameType,TextureBasePtr>	NamedTextureMapType;
	
	typedef texture::Texture<TextureInstType>						TextureType;
	typedef texture::TextureSet<TextureInstType>					TextureSetType;
	typedef texture::Animation<TextureInstType>						AnimationType;
	typedef AnimationType::TypeOfAnimation::Type					TypeOfAnimation;

	typedef boost::shared_ptr<TextureType>							TexturePtr;
	typedef boost::shared_ptr<TextureSetType>						TextureSetPtr;
	typedef boost::shared_ptr<AnimationType>						AnimationPtr;

	typedef xtal::SmartPtr<TextureType>								TexturePtrX;
	typedef xtal::SmartPtr<TextureSetType>							TextureSetPtrX;
	typedef xtal::SmartPtr<AnimationType>							AnimationPtrX;

	typedef tRect<TextureType::SizeType>							RectType;
	typedef xtal::SmartPtr<RectType>								RectTypePtrX;

	struct LoadGraphFromSomeSource
	{
		typedef TextureManager::TextureInstType InstType;
		virtual InstType operator()()=0;
	};

private:
	//for warning
	std::string error_message_;
	BYTE retry_time_;

	NamedTextureMapType named_texture_map_;

	TextureInstType LoadWithWarning(const LoadGraphFromSomeSource* op);
	TextureInstType DivLoadWithWarning(const LoadGraphFromSomeSource* op);

	NamedTextureMapType::iterator GetRawIterator(const std::string& name);
	NamedTextureMapType::iterator GetRawIterator(const xtal::String& name);

public:
	TextureManager()
		{
			RetrySetting();
		}
	
	void RetrySetting(BYTE retry_time=10,std::string error_message="")
		{
			retry_time_		=retry_time;
			error_message_	=error_message;
		}

	void RetrySettingX(BYTE retry_time=10,xtal::String& error_message)
		{
			retry_time_		=retry_time;
			error_message_	=error_message.c_str();
		}

	void NotUseRetry()
		{
			retry_time_=1;
		}

	//load
	TexturePtr Load(fsys::path& file,std::string& name,bool not_use_3D=true);
	TexturePtr Load(BYTE* src,DWORD size,std::string& name,bool not_use_3D=true);

	TextureSetPtr DivLoad(fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	TextureSetPtr DivLoad(BYTE* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);

	AnimationPtr LoadAnimation(TypeOfAnimation anime_type,fsys::path& file,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	AnimationPtr LoadAnimation(TypeOfAnimation anime_type,BYTE* src,DWORD size,std::string& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	
	//cut
	TexturePtr CutTexture(const std::string& old_name,const std::string& new_name,RectType cut_range);

	//unload
	void UnloadTexture(const std::string& name);

	//Load for xtal
	TextureTypeX LoadX(PathType& file,TextureNameType& name,bool not_use_3D=true);
	TextureSetTypeX DivLoadX(PathType& file,TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	AnimationPtrX LoadAnimationX(TypeOfAnimation anime_type,PathType& file,TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	TextureTypePtrX CutTextureX(const TextureNameType& old_name,const TextureNameType& new_name,RectTypePtr cut_range);
	void UnloadTextureX(const TextureNameType& name);

	//TODO
	bool IsLoaded(const std::string& name);

	//for xtal
	bool IsLoadedX(const xtal::String& name);
	TextureTypePtrX GetAsTexture(const xtal::String& name);
	TextureSetPtrX GetAsTextureSet(const xtal::String& name);
	AnimationPtrX GetAsAnimation(const xtal::String& name);

	static void bind(xtal::ClassPtr it);
};
