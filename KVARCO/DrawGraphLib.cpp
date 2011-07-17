#include "pch.h"
#include "KVARCO.h"
#include "CharacterParam.h"

namespace kvarco
{
namespace detail
{

void DrawGraphRaw(int GrHandle,long x,long y,bool Trans,bool call_alpha)
{
	if(!call_alpha) kvarco::SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,0xFF);
	DxLib::DrawGraph(x,y,GrHandle,(int )Trans);
}

void DrawRotaGraph(int GrHandle,float Angle,long cx,long cy,bool Trans,bool call_alpha)
{
	if(!call_alpha) kvarco::SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,0xFF);
	DxLib::DrawRotaGraph(cx,cy,1.0,Angle,GrHandle,(int )Trans,1);
}

void DrawGraphAlpha(int GrHandle,long x,long y,int Alpha,bool Trans)
{
	kvarco::SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha&0xFF));
	DrawGraphRaw(GrHandle,x,y,Trans,true);
}

void DrawRotaGraphAlpha(int GrHandle,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	kvarco::SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha&0xFF));
	DrawRotaGraph(GrHandle,Angle,cx,cy,(int )Trans,true);
}

void DrawZoom(const CharParamPtrX& char_param,bool trans,bool call_alpha)
{
	CharParam::Type half_x=char_param->texture_->GetSize().width_ *char_param->scale_x_/2;
	CharParam::Type half_y=char_param->texture_->GetSize().height_*char_param->scale_y_/2;
	fRect zoomed(char_param->center_.x-half_x,char_param->center_.y-half_y,char_param->center_.x+half_x,char_param->center_.y+half_y);
	
	if(!call_alpha) kvarco::SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,0xFF);
	DxLib::DrawExtendGraph(zoomed.left_,zoomed.top_,zoomed.right_,zoomed.bottom_,char_param->texture_->Get(),true);
}

void DrawZoomAlpha(const CharParamPtrX& char_param,int Alpha)
{
	kvarco::SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(char_param->alpha_&0xFF));
	DrawZoom(char_param,true,true);
}

void DrawRotaZoom(const CharParamPtrX& char_param,bool Trans,bool call_alpha)
{
	CharParam::Type half_x=char_param->texture_->GetSize().width_ *char_param->scale_x_/2;
	CharParam::Type half_y=char_param->texture_->GetSize().height_*char_param->scale_y_/2;
	fRect zoomed(char_param->center_.x-half_x,char_param->center_.y-half_y,char_param->center_.x+half_x,char_param->center_.y+half_y);

	fPoint tmp[4];
	tmp[0].x=zoomed.left_;	tmp[0].y=zoomed.top_;
	tmp[1].x=zoomed.right_;	tmp[1].y=zoomed.top_;
	tmp[2].x=zoomed.right_;	tmp[2].y=zoomed.bottom_;
	tmp[3].x=zoomed.left_;	tmp[3].y=zoomed.bottom_;

	float sin_=sin(char_param->angle_.GetAsRadian());
	float _sin_=-sin_;
	float cos_=cos(char_param->angle_.GetAsRadian());

	fPoint res[4];
	//線形変換による回転
	for(int i=0; i<4; i++)
	{
		res[i].x=	_sin_*	tmp[i].y+	cos_*	tmp[i].x+	char_param->center_.x;
		res[i].y=	cos_*	tmp[i].y+	sin_*	tmp[i].x+	char_param->center_.y;
	}

	if(!call_alpha) kvarco::SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,0xFF);
	DxLib::DrawModiGraph(
		static_cast<int>(res[0].x),static_cast<int>(res[0].y),
		static_cast<int>(res[1].x),static_cast<int>(res[1].y),
		static_cast<int>(res[2].x),static_cast<int>(res[2].y),
		static_cast<int>(res[3].x),static_cast<int>(res[3].y),
		char_param->texture_->Get(),(int )Trans);
}

void DrawRotaZoomAlpha(const CharParamPtrX& char_param,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(char_param->alpha_&0xFF));
	DrawRotaZoom(char_param,Trans,true);
}

} //namespace detail
} //namespace kvarco

namespace kvarco
{

void DrawGraphAllDisabled(const CharParamPtrX& char_param)
{
	CharParam::PointPtrX upper_left=char_param->GetUpperLeftCorner();
	kvarco::detail::DrawGraphRaw(char_param->texture_->Get(),upper_left->x,upper_left->y,true,false);
}

void DrawGraph(const CharParamPtrX& char_param)
{
	bool enable_alpha	=char_param->alpha_!=0.0;
	bool enable_rota	=char_param->angle_.GetAngle()!=0.0;
	bool enable_zoom	=(char_param->scale_x_!=1.0) && (char_param->scale_y_!=1.0);

	DrawGraphEnabledFeatures(char_param,enable_alpha,enable_rota,enable_zoom);
}

void DrawGraphEnabledFeatures(const CharParamPtrX& char_param,bool enable_alpha,bool enable_rota,bool enable_zoom)
{
	// bit 1 : enable_alpha
	// bit 2 : enable_rota
	// bit 3 : enable_zoom
	char switch_flag=((enable_zoom? 1:0)<<2)|((enable_rota ? 1:0)<<1)|((enable_alpha ? 1:0)<<0);

	//transをoffにしたい? え? なに言ってるの? それ知らない
	switch(switch_flag)
	{
		case 0: // enable_alpha=false | enable_rota=false | enable_zoom=false
		{
			DrawGraphAllDisabled(char_param);
		}
		break;

		case 1: // enable_alpha=true  | enable_rota=false | enable_zoom=false
		{
			CharParam::PointPtrX upper_left=char_param->GetUpperLeftCorner();
			kvarco::detail::DrawGraphAlpha(char_param->texture_->Get(),upper_left->x,upper_left->y,char_param->alpha_,true);
		}
		break;

		case 2: // enable_alpha=false | enable_rota=true  | enable_zoom=false
		{
			kvarco::detail::DrawRotaGraph
				(char_param->texture_->Get(),char_param->angle_.GetAsRadian(),char_param->center_.x,char_param->center_.y,true,false);
		}
		break;

		case 3: // enable_alpha=true  | enable_rota=true  | enable_zoom=false
		{
			kvarco::detail::DrawRotaGraphAlpha
				(char_param->texture_->Get(),char_param->angle_.GetAsRadian(),char_param->center_.x,char_param->center_.y,char_param->alpha_,false);
		}
		break;

		case 4: // enable_alpha=false | enable_rota=false | enable_zoom=true
		{
			kvarco::detail::DrawZoom(char_param,true,false);
		}
		break;
		
		case 5: // enable_alpha=true  | enable_rota=false | enable_zoom=true
		{
			kvarco::detail::DrawZoomAlpha(char_param,true);
		}
		break;
		
		case 6: // enable_alpha=false | enable_rota=true  | enable_zoom=true
		{
			kvarco::detail::DrawRotaZoom(char_param,true,false);
		}
		break;
		
		case 7: // enable_alpha=true  | enable_rota=true  | enable_zoom=true
		{
			kvarco::detail::DrawRotaZoomAlpha(char_param,true);
		}
		break;
	}
}

}
