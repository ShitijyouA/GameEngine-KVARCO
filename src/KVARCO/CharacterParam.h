#pragma once
#include "TextureParam.h"
#include "Texture.h"
#include "LayerManager.h"
#include "GeoStruct.h"

template<typename ParamType,typename PointInstType,typename TextureInstType>
struct BasicCharacterParam
	:public BasicTextureParam<ParamType>
{
	typedef BasicCharacterParam<ParamType,PointInstType,TextureInstType>	ThisType;
	typedef BasicTextureParam<ParamType>									TextureParamType;
	
	typedef concept::Texture<TextureInstType>	BasicTextureType;
	typedef xtal::SmartPtr<BasicTextureType>	TexturePtrX;

	typedef tPoint<PointInstType>				PointType;
	typedef xtal::SmartPtr<PointType>			PointPtrX;

	typedef tRect<PointInstType>				RectType;
	typedef xtal::SmartPtr<RectType>			RectPtrX;
	
	typedef concept::Texture<int>::SizeType		SizeType;
	typedef	xtal::SmartPtr<SizeType>			SizePtrX;

	typedef xtal::StringPtr						LayerNamePtrX;
	typedef int									LayerIdType;

	PointType		center_;
	PointType		velocity_;
	TexturePtrX		texture_;
	LayerNamePtrX	layer_name_;
	LayerIdType		layer_id_;

	/// ���C���[��ID���擾/�K�p���Ă���
	void ApplyLayerId()
		{
			layer_id_=LayerManager::GetInst()->GetHandle(layer_name_);
		}

	/// ��]���Ă��Ȃ����A180�x������]���Ă���ꍇ�̂ݗL���B����ȊO�ł�PointType(0,0)��Ԃ�
	/// \return �L�����̍���̍��W
	PointPtrX GetUpperLeftCorner()
		{
			Angle angle_tmp(angle_.GetAsRadian());
			angle_tmp.Normalize();
			
			if(angle_tmp.GetAsRadian()!=0 && angle_tmp.GetAsRadian()!=math::constants::pi<float>())
				return xtal::xnew<PointType>(0.0f,0.0f);

			const texture::Texture::PointType& texture_center=texture_->GetCenter();
			return xtal::xnew<PointType>(center_.x-texture_center.x,center_.y-texture_center.y);
		}

	/// �L�����N�^�̒��S����`�̒��S�ɃZ�b�g����
	void SetInCenterOfRect(const SizePtrX rect_size)
		{
			center_.x=rect_size->width_/2;
			center_.y=rect_size->height_/2;
		}

	/// �L�����N�^�̒��S�����C���[�̒��S�ɃZ�b�g����
	void SetInCenterOfLayer()
		{
			if(layer_id_==0) ApplyLayerId();
			lSizePtrX size(LayerManager::GetInst()->GetSize(layer_id_));

			SizeType tmp_size(size->width_,size->height_);
			SetInCenterOfRect(&tmp_size);
		}

	/// velocity_�ňړ�
	void Move()
		{
			center_.x+=velocity_.x;
			center_.y+=velocity_.y;
		}

	/// LayerManager::IsInLayer()�������Ɗy�Ɏg�����߂̃��b�p
	bool IsInLayer(PointInstType margin)
	{
		const BasicTextureType::SizeType& size=texture_->GetSize();
		PointPtrX g_center=
			LayerManager::GetInst()->TransPointGlobal(layer_id_,center_.x,center_.y);
		fRect area
		(
			g_center->x-size.height_,
			g_center->y- size.width_,
			g_center->x+size.height_,
			g_center->y+ size.width_
		);

		return LayerManager::GetInst()->IsInLayer(layer_name_->c_str(),area,margin);
	}

	/// �L�����N�^����`����͂ݏo���Ȃ��悤�ɂ���
	void BeInRect( const RectPtrX rect,const SizePtrX custom_char_size)
		{
			RectType::Type w=custom_char_size->width_/2;
			RectType::Type h=custom_char_size->height_/2;
			RectType char_rect(-w,-h,+w,+h);
			
			if(center_.x+char_rect.left_	<rect->left_)	center_.x=rect->left_	-char_rect.left_;
			if(center_.y+char_rect.top_		<rect->top_)	center_.y=rect->top_	-char_rect.top_;
			if(center_.x+char_rect.right_	>rect->right_)	center_.x=rect->right_	-char_rect.right_;
			if(center_.y+char_rect.bottom_	>rect->bottom_)	center_.y=rect->bottom_	-char_rect.bottom_;
		}

	/// �L�����N�^�����C���[����͂ݏo���Ȃ��悤�ɂ���(�摜�̃T�C�Y)
	void BeInLayer()
		{
			if(layer_id_==0) ApplyLayerId();
			fRectPtrX area(LayerManager::GetInst()->GetArea(layer_id_));

			RectType tmp_rect(0,0,area->right_-area->left_,area->bottom_-area->top_);
			BeInRect(&tmp_rect,&texture_->GetSize());
		}

	/// �A���t�@�l�𑫂��Aalpha_��[0...255]�Ɏ��߂�
	void AddAlpha(TextureParamType::AlphaType add_alpha)
		{
			alpha_+=add_alpha;
			if(alpha_<0.0)		alpha_=0.0;
			if(alpha_>255.0) 	alpha_=255.0;
		}

	BasicCharacterParam()
		:BasicTextureParam<ParamType>(),center_(0,0),velocity_(0,0),layer_id_(0)
		{}

	~BasicCharacterParam()
		{
			texture_	=xtal::null;
			layer_name_	=xtal::null;
			layer_id_	=0;
		}

	static void bind(xtal::ClassPtr it)
		{
			USE_XDEFZ(ThisType);
			
			Xdef_var(center_);
			Xdef_var(velocity_);
			Xdef_var(texture_);
			Xdef_var(layer_name_);
			Xdef_var(layer_id_);

			Xdef_method(ApplyLayerId);
			Xdef_method(GetUpperLeftCorner);
			Xdef_method(SetInCenterOfRect);
			Xdef_method(SetInCenterOfLayer);
			Xdef_method(Move);
			Xdef_method(IsInLayer);
			Xdef_method(BeInRect);
			Xdef_method(BeInLayer);
			Xdef_method(AddAlpha);
		}
};

typedef BasicCharacterParam<float,float,int>	CharacterParam;
typedef xtal::SmartPtr<CharacterParam>			CharacterParamPtrX;

//�ȗ���
typedef CharacterParam		CharParam;
typedef CharacterParamPtrX	CharParamPtrX;
