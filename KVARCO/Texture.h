#pragma once
#include "GeoStruct.h"

#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/assert.hpp>

class TextureManager;

namespace concept
{
	template<typename TextureInstType>
	class Texture
	{
	public:
		typedef TextureInstType	InstType;
		typedef DWORD			SizeType;
	#ifdef USE_SIZE_STRCT
		typedef tSize<SizeType>	SizeStructType;
	#else
		typedef tRect<SizeType>	SizeStructType;
	#endif
		typedef float			PointType;
		typedef tPoint<float>	PointStructType;

		virtual const InstType& Get()=0;
		const InstType& operator()() { return Get(); }
			
		virtual const SizeStructType& GetSize()=0;
		virtual const PointStructType& GetCenter()=0;
	};
}

namespace texture
{

template<typename TextureInstType>
class Texture
	: public concept::Texture<TextureInstType>
{
public:
	typedef TextureInstType				InstType;
	typedef Texture<InstType>			TextureType

protected:
	const InstType	inst_;
	SizeStructType	size_;
	PointStructType	center_;

	friend class TextureManager;
	Texture(const InstType& inst)
		:inst_(inst)
		{
//DxLibを使用しているんだよね?
#ifdef __DXLIB
			BOOST_MPL_ASSERT((boost::is_convertible<InstType,int>))
			// DxLib「ASSERT……嘘だったんだね……ッ! 私のこと使ってるって、使ってるって信じてたのに!!」
			
			int buf_width	=0;
			int buf_height	=0;
			int ggs_res=::DxLib::GetGraphSize(inst_,&buf_width,&buf_height);
			
			if(ggs_res!=-1)
			{
#ifdef USE_SIZE_STRCT
				size_.width_	=buf_width;
				size_.height_	=buf_height;
#else
				size_.left	=0;			size_.top	=0;
				size_.right	=buf_width;	size_.bottom=buf_height;
#endif
			}
			center_.x=static_cast<PoingType>(size_.width_)	*0.5;
			center_.y=static_cast<PoingType>(size_.height_)	*0.5;
#endif
		}

public:
	virtual const InstType& Get()
		{
			return inst_;	
		}
	
	virtual const SizeStructType& GetSize()
		{
			return size_;
		}

	virtual const PointStructType& GetCenter()
		{
			return center_;
		}

	static void bind(xtal::ClassPtr it)
		{
			USE_XDEFX(TextureType)

			Xdef_method(Get);
			Xdef_method(GetSize);
			Xdef_method(GetCenter);
		}
};

template<typename TextureInstType>
class TextureSet
	: public concept::Texture<TextureInstType>,public std::vector<TextureInstType>
{
public:
	typedef std::vector<TextureInstType>	TextureListType;
	typedef Texture<TextureInstType>		TextureType;
 //   typedef TextureType::InstType			InstType;
 //   typedef TextureType::TextureType		TextureType;
 //   typedef TextureType::SizeType			SizeType;
 //   typedef TextureType::SizeStructType		SizeStructType;
 //   typedef TextureType::PointType			PointType;
 //   typedef TextureType::PointStructType	PointStructType;

private:
	SizeStructType	size_;
	PointStructType	center_;

	friend class TextureManager;
	TextureSet(const InstType* instz,const SizeType num)
		:std::vector<TextureInstType>(const_cast<InstType*>(instz),const_cast<InstType*>(instz)+num)
	{
		TextureType	temp(this->at(0));
		size_	=const_cast<SizeStructType&>(temp.GetSize());
		center_	=const_cast<PointStructType&>(temp.GetCenter());
	}

	TextureSet(std::vector<InstType>& instz)
		:std::vector<TextureInstType>()
	{
		this->swap(instz);
		TextureType	temp(this->at(0));
		size_	=const_cast<SizeStructType&>(temp.GetSize());
		center_	=const_cast<PointStructType&>(temp.GetCenter());
	}

public:
	virtual const InstType& Get(SizeType pos)
		{
			return this->at(pos);
		}
	
	const TextureListType& GetRawInstance()
		{
			return *this;
		}

	virtual const SizeStructType& GetSize()
		{
			return size_;
		}

	virtual const PointStructType& GetCenter()
		{
			return center_;
		}

};

template<typename TextureInstType>
class Animation
	: public concept::Texture<TextureInstType>,public TextureSet<TextureInstType>
{
public:
	typedef std::vector<TextureInstType>	FrameListType;
	typedef FrameListType::size_type		FrameCountType;
	typedef Animation<TextureInstType>		AnimationType;
 //   typedef Texture<TextureInstType>		TextureType;
 //   typedef TextureType::InstType			InstType;
 //   typedef TextureType::SizeType			SizeType;
 //   typedef TextureType::SizeStructType		SizeStructType;
 //   typedef TextureType::PointType			PointType;
 //   typedef TextureType::PointStructType	PointStructType;

	struct TypeOfAnimation
	{
		enum Type
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
	const TypeOfAnimation::Type	type_of_animation_;
	AnimationState::state		animation_state_;
	FrameCountType				draw_time_per_frame_;

	FrameListType	frame_list_;
	FrameCountType	frame_count_;
	FrameCountType	draw_time_;

	SizeStructType	size_;
	PointStrucType	center_;
	
	void NextFrame()
	{
		if(++draw_time_<draw_time_per_frame_) return;
		draw_time_=0;

		bool anime_end=(frame_count_>=frame_list_.size());
		switch(type_of_animation_)
		{
			case STOP:				//停止
				if(anime_end) animation_state_=AnimationState::PLAYED_ONCE;
				else
				if(animation_state_!=AnimationState::PLAYED_ONCE) ++frame_count_;
			break;

			case RETURN_AND_STOP:	//最初にもどって停止
				if(anime_end)
				{
					frame_count_=0;
					animation_state_=AnimationState::PLAY_END;
				}
				if(animation_state_==AnimationState::PLAYING_NORMAL) ++frame_count_;
			break;

			case REPEAT:			//最初にもどって再度再生
				animation_state_=AnimationState::PLAYING_NORMAL;
				if(anime_end) frame_count_=0;
				++frame_count_;
			break;

			case REPEAT_REVERSE:	//最後から逆に再度再生
				anime_end|=(animation_state_==AnimationState::PLAYING_REVERSE && frame_count==0);
				if(anime_end)
				{
					if(animation_state_==AnimationState::PLAYING_NORMAL)
						animation_state_=AnimationState::PLAYING_REVERSE;
					else
					if(animation_state_==AnimationState::PLAYING_REVERSE)
						animation_state_=AnimationState::PLAYING_NORMAL;
				}
				if(animation_state_==AnimationState::PLAYING_NORMAL) ++frame_count;
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
	Animation(const InstType* instz,const SizeType num,TypeOfAnimation::Type type_of_animation)
			:frame_list_(const_cast<InstType*>(instz),const_cast<InstType*>(instz)+num)
		{
			TextureType	temp(frame_list_.at(0));
			size_	=const_cast<SizeStructType&>(temp.GetSize());
			center_	=const_cast<PointStructType&>(temp.GetCenter());
			
			Init();
		}

	Animation(std::vector<InstType>& instz,TypeOfAnimation::Type type_of_animation)
			:frame_list_(),type_of_animation_(type_of_animation)
		{
			frame_list_.swap(instz);

			TextureType	tmp(frame_list_.at(0));
			size_	=const_cast<SizeStructType&>(tmp.GetSize());
			center_	=const_cast<PointStructType&>(tmp.GetCenter());
			
			Init();
		}

public:
	const InstType& GetNextFrame()
		{
			InstType& ret=frame_list_.at(current_frame_);
			NaxtFrame();
			return ret;
		}
	
	const InstType& Get()
		{
			return GetNextFrame();
		}

	const SizeStructType& GetSize()
		{
			return size_;
		}

	const PointStructType& GetCenter()
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
};

#ifdef __DXLIB

typedef Texture<int>		Texture;
typedef TextureSet<int>		TextureSet;
typedef Animation<int>		Animation;

#endif
}
