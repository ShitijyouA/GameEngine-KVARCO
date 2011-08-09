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
	typedef std::vector<TextureBasePtr>								UnnamedTextureVectorType;

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

		const std::string& texture_name_;
		LoadGraphFromSomeSource(std::string& name)
			:texture_name_(name)
		{}

		virtual InstType operator()()=0;
	};

	struct LoadedTextureFromArchive
	{
		typedef TextureManager::TextureInstType InstType;
		enum LoadType
		{
			LOAD,
			DIV_LOAD,
		};

		fsys::path		path_;
		std::string		name_;
		enum LoadType	load_type_;

		//for DivLoad
		WORD			all_num_;
		WORD			x_num_;
		WORD			y_num_;
		WORD			x_size_;
		WORD			y_size_;

		LoadedTextureFromArchive
			(
			fsys::path& path,std::string& name,enum LoadType load_type,
				WORD all_num=0,WORD x_num=0,WORD y_num=0,WORD x_size=0,WORD y_size=0
			)
			:
				path_(path),name_(name),load_type_(load_type),
				all_num_(all_num),x_num_(x_num),y_num_(y_num),x_size_(x_size),y_size_(y_size)
			{}
	};
	typedef std::vector<LoadedTextureFromArchive>					LoadedFileVectorFromArchiveType;

private:
	//for warning
	xtal::String error_message_;
	BYTE retry_time_;

	NamedTextureMapType				named_texture_map_;
	UnnamedTextureVectorType		unnamed_texture_vector_;
	LoadedFileVectorFromArchiveType	file_path_from_archive_;

	TextureInstType LoadWithWarning(LoadGraphFromSomeSource* op);
	TextureInstType DivLoadWithWarning(LoadGraphFromSomeSource* op);

	TextureBasePtr GetRawPtr(const std::string& name);
	TextureBasePtr GetRawPtr(const TextureNameType& name);

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
	
	//Load for xtal
	TexturePtrX LoadX(PathType& file,TextureNameType& name,bool not_use_3D=true);
	TextureSetPtrX DivLoadX(PathType& file,TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	AnimationPtrX LoadAnimationX(TypeOfAnimation anime_type,PathType& file,TextureNameType& name,WORD all_num,WORD x_num,WORD y_num,WORD x_size,WORD y_size,bool not_use_3D=true);
	TexturePtrX CutTextureX(const TextureNameType& old_name,const TextureNameType& new_name,RectTypePtrX cut_range);
	void UnloadTextureX(const TextureNameType& name);

	//for native
	bool IsLoaded(const std::string& name);
	TexturePtr GetAsTexture(const std::string& name);
	TextureSetPtr GetAsTextureSet(const std::string& name);
	AnimationPtr GetAsAnimation(const std::string& name);

	//for xtal
	bool IsLoadedX(const TextureNameType& name);
	TexturePtrX GetAsTextureX(const TextureNameType& name);
	TextureSetPtrX GetAsTextureSetX(const TextureNameType& name);
	AnimationPtrX GetAsAnimationX(const TextureNameType& name);

private:
	FRIENDS_FOR_XTAL(TextureManager)
	TextureManager()
		{
			RetrySetting(10,"‰æ‘œ‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s‚µ‚Ü‚µ‚½ : ");
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
			named_texture_map_.clear();
		}

	void Release()
		{
			ReleaseAllTexture();
		}

	void Reload();
};