#pragma once
#include "GeoStruct.h"

#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/assert.hpp>

class TextureManager;

namespace texture
{
template<typename TextureInstType>
class BasicTexture;

template<typename TextureInstType>
class BasicTextureSet;

template<typename TextureInstType>
class BasicAnimation;
}

namespace concept
{
	template<typename TextureInstType>
	class Texture
	{
	public:
		typedef TextureInstType			InstType;
		typedef long					SizeInstType;
		typedef tSize<SizeInstType>		SizeType;
		typedef float					PointInstType;
		typedef tPoint<PointInstType>	PointType;

		virtual const InstType& Get()
			{
				static InstType null_texture;
				return null_texture;
			}

		//const InstType& operator()() { return Get(); }
			
		virtual const SizeType& GetSize()=0;
		virtual const PointType& GetCenter()=0;

		virtual void Unload()=0;

		typedef texture::BasicTexture<TextureInstType>		TextureType;
		typedef texture::BasicTextureSet<TextureInstType>	TextureSetType;
		typedef texture::BasicAnimation<TextureInstType>	AnimationType;

		typedef xtal::SmartPtr<TextureType>					TexturePtrX;
		typedef xtal::SmartPtr<TextureSetType>				TextureSetPtrX;
		typedef xtal::SmartPtr<AnimationType>				AnimationPtrX;

		TexturePtrX AsTexture()
			{
				TextureType* tmp=dynamic_cast<TextureType*>(this);
				if(tmp==NULL) return xtal::null;
				return TexturePtrX(tmp,xtal::undeleter);
			}

		TextureSetPtrX AsTextureSet()
			{
				TextureSetType* tmp=dynamic_cast<TextureSetType*>(this);
				if(tmp==NULL) return xtal::null;
				return TextureSetPtrX(tmp,xtal::undeleter);
			}

		AnimationPtrX AsAnimation()
			{
				AnimationType* tmp=dynamic_cast<AnimationType*>(this);
				if(tmp==NULL) return xtal::null;
				return AnimationPtrX(tmp,xtal::undeleter);
			}
	};
}

namespace texture
{

template<typename TextureInstType>
class BasicTexture
	: public concept::Texture<TextureInstType>
{
public:
	typedef TextureInstType				InstType;
	typedef BasicTexture<InstType>		TextureType;

protected:
	const InstType	inst_;
	SizeType		size_;
	PointType		center_;

	friend class TextureManager;
	friend class BasicTextureSet<InstType>;
	friend class BasicAnimation<InstType>;
	BasicTexture(const InstType& inst)
		:inst_(inst)
		{
//DxLibを使用しているんだよね?
#ifdef __DXLIB
			BOOST_MPL_ASSERT((boost::is_convertible<InstType,int>));
			// DxLib「ASSERT……嘘だったんだね……ッ! 私のこと使ってるって、使ってるって信じてたのに!!」
			
			int buf_width	=0;
			int buf_height	=0;
			int ggs_res=::DxLib::GetGraphSize(inst_,&buf_width,&buf_height);

			size_.width_	=buf_width;
			size_.height_	=buf_height;

			center_.x=static_cast<PointInstType>(size_.width_)	*0.5;
			center_.y=static_cast<PointInstType>(size_.height_)	*0.5;
#endif
		}

public:
	BasicTexture()	//無効値
		:inst_(-1)
	{}

	virtual const InstType& Get()
		{
			return inst_;	
		}
	
	virtual const SizeType& GetSize()
		{
			return size_;
		}

	virtual const PointType& GetCenter()
		{
			return center_;
		}

	virtual void Unload()
		{
			::DxLib::DeleteGraph(inst_);

			size_.width_=0;	size_.height_=0;
			center_.x	=0;	center_.y	 =0;
		}

	static void bind(xtal::ClassPtr it)
		{
			USE_XDEFZ(TextureType);

			Xdef_method(Get);
			Xdef_method(GetSize);
			Xdef_method(GetCenter);
		}
};

template<typename TextureInstType>
class BasicTextureSet
	: public concept::Texture<TextureInstType>
{
public:
	typedef std::vector<InstType>				TextureListType;
	typedef typename TextureListType::size_type	NumType;
	typedef BasicTexture<InstType>				TextureType;
	typedef BasicTextureSet<InstType>			TextureSetType;
	typedef concept::Texture<TextureInstType>	TextureBaseType;
	typedef xtal::SmartPtr<TextureBaseType>		TextureBasePtrX;

private:
	TextureListType	texture_list_;
	SizeType		size_;
	PointType		center_;

	friend class TextureManager;
	BasicTextureSet(const InstType* instz,const SizeType num)
		:texture_list_(const_cast<InstType*>(instz),const_cast<InstType*>(instz)+num)
		{
			TextureSetType	temp(texture_list_.at(0));
			size_	=const_cast<SizeType&>(temp.GetSize());
			center_	=const_cast<PointType&>(temp.GetCenter());
		}

	BasicTextureSet(std::vector<InstType>& instz)
		:texture_list_()
		{
			texture_list_.swap(instz);
			TextureType	temp(texture_list_.at(0));
			size_	=const_cast<SizeType&>(temp.GetSize());
			center_	=const_cast<PointType&>(temp.GetCenter());
		}

public:
	//const InstType& Get()
	//	{
	//		return texture_list_.at(0);
	//	}

	const TextureBasePtrX& GetAt(NumType pos)
		{
			TextureType* tmp=new TextureType(texture_list_.at(pos));
			return TextureBasePtrX(static_cast<TextureBaseType*>(tmp));
		}
	
	const TextureBasePtrX& operator[](NumType pos)
		{
			return GetAt(pos);
		}

	const TextureListType& GetRawInstance()
		{
			return texture_list_;
		}

	const SizeType& GetSize()
		{
			return size_;
		}

	const NumType GetNum()
		{
			return texture_list_.size();
		}

	const PointType& GetCenter()
		{
			return center_;
		}

	void Unload()
		{
			BOOST_FOREACH(InstType& i,texture_list_) ::DxLib::DeleteGraph(i);

			texture_list_.clear();
			size_.width_=0;	size_.height_=0;
			center_.x	=0;	center_.y	 =0;
		}


	static void bind(xtal::ClassPtr it)
		{
			USE_XDEFZ(TextureSetType);

			//Xdef_method(Get);
			Xdef_method(GetAt);
			Xdef_method(GetNum);
			Xdef_method(GetSize);
			Xdef_method(GetCenter);
		}
};

template<typename TextureInstType>
class BasicAnimation
	: public concept::Texture<TextureInstType>
{
public:
	typedef typename std::vector<TextureInstType>	FrameListType;
	typedef typename FrameListType::size_type		FrameCountType;
	typedef BasicAnimation<TextureInstType>			AnimationType;

	struct TypeOfAnimation
	{
		enum type
		{
			STOP,				//停止
			RETURN_AND_STOP,	//最初にもどって停止
			REPEAT,				//最初にもどって再度再生
			REPEAT_REVERSE,		//最後から逆に再度再生
		};
	};

protected:
	struct AnimationState
	{
		enum state
		{
			PLAY_END,
			PLAYED_ONCE,
			PLAYING_NORMAL,
			PLAYING_REVERSE,
		};
	};

	//setting
	const enum TypeOfAnimation::type	type_of_animation_;
	typename AnimationState::state		animation_state_;
	FrameCountType						draw_time_per_frame_;

	FrameListType	frame_list_;
	FrameCountType	frame_count_;
	FrameCountType	draw_time_;

	SizeType	size_;
	PointType	center_;
	
	void NextFrame()
	{
		if(++draw_time_<draw_time_per_frame_) return;
		draw_time_=0;

		bool anime_end=(frame_count_>=frame_list_.size());
		switch(type_of_animation_)
		{
			case TypeOfAnimation::STOP:				//停止
				if(anime_end) animation_state_=AnimationState::PLAYED_ONCE;
				else
				if(animation_state_!=AnimationState::PLAYED_ONCE) ++frame_count_;
			break;

			case TypeOfAnimation::RETURN_AND_STOP:	//最初にもどって停止
				if(anime_end)
				{
					frame_count_=0;
					animation_state_=AnimationState::PLAY_END;
				}
				if(animation_state_==AnimationState::PLAYING_NORMAL) ++frame_count_;
			break;

			case TypeOfAnimation::REPEAT:			//最初にもどって再度再生
				animation_state_=AnimationState::PLAYING_NORMAL;
				if(anime_end) frame_count_=0;
				++frame_count_;
			break;

			case TypeOfAnimation::REPEAT_REVERSE:	//最後から逆に再度再生
				anime_end|=(animation_state_==AnimationState::PLAYING_REVERSE && frame_count_==0);
				if(anime_end)
				{
					if(animation_state_==AnimationState::PLAYING_NORMAL)
						animation_state_=AnimationState::PLAYING_REVERSE;
					else
					if(animation_state_==AnimationState::PLAYING_REVERSE)
						animation_state_=AnimationState::PLAYING_NORMAL;
				}
				if(animation_state_==AnimationState::PLAYING_NORMAL) ++frame_count_;
				if(animation_state_==AnimationState::PLAYING_REVERSE) frame_count_=static_cast<long>(frame_count_)-1;
			break;
		}
	}

	void Init()
		{
			animation_state_	=AnimationState::PLAYING_NORMAL;
			draw_time_per_frame_=1;
			draw_time_			=0;
		}

	friend class TextureManager;
	BasicAnimation(const InstType* instz,const SizeType num,enum TypeOfAnimation::type type_of_animation)
			:frame_list_(const_cast<InstType*>(instz),const_cast<InstType*>(instz)+num)
		{
			TextureType	temp(frame_list_.at(0));
			size_	=const_cast<SizeType&>(temp.GetSize());
			center_	=const_cast<PointType&>(temp.GetCenter());
			
			Init();
		}

	BasicAnimation(std::vector<InstType>& instz,enum TypeOfAnimation::type type_of_animation)
			:frame_list_(),type_of_animation_(type_of_animation)
		{
			frame_list_.swap(instz);

			texture::BasicTexture<InstType> tmp(frame_list_.at(0));
			size_	=const_cast<SizeType&>(tmp.GetSize());
			center_	=const_cast<PointType&>(tmp.GetCenter());
			
			Init();
		}

public:
	const InstType& GetNextFrame()
		{
			InstType& ret=frame_list_.at(frame_count_);
			NextFrame();
			return ret;
		}

	//const InstType& Get()
	//	{
	//		return GetNextFrame();
	//	}

	const SizeType& GetSize()
		{
			return size_;
		}

	FrameCountType GetFrameNum()
		{
			return frame_list_.size();
		}

	const PointType& GetCenter()
		{
			return center_;
		}

	const FrameListType& GetRawInstance()
		{
			return frame_list_;
		}

	void SetFrameSpeed(FrameCountType draw_time_per_frame)
		{
			draw_time_per_frame_=draw_time_per_frame;
		}

	void Unload()
		{
			BOOST_FOREACH(InstType& i,frame_list_) ::DxLib::DeleteGraph(i);

			size_.width_=0;	size_.height_=0;
			center_.x	=0;	center_.y	 =0;
		}


	static void bind(xtal::ClassPtr it)
		{
			USE_XDEFZ(AnimationType);

			Xdef_method(GetNextFrame);
			//Xdef_method(Get);
			Xdef_method(GetFrameNum);
			Xdef_method(GetSize);
			Xdef_method(GetCenter);
			Xdef_method(SetFrameSpeed);
		}
};

#ifdef __DXLIB

typedef concept::Texture<int>	TextureConcept;
typedef BasicTexture<int>		Texture;
typedef BasicTextureSet<int>	TextureSet;
typedef BasicAnimation<int>		Animation;

#endif
}
