#include "pch.h"
#include "KVARCO.h"
using namespace KVARCO;

///////////////////////////////////////////
/////////グラフィックハンドル指定型//////////
///////////////////////////////////////////
void KVARCO::DrawGraph_H(int GrHandle,long x,long y,bool Trans,bool call_alpha)
{
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawGraph(x,y,GrHandle,(int )Trans);
}

void KVARCO::DrawRotaGraph_H(int GrHandle,float Angle,long cx,long cy,bool Trans,bool call_alpha)
{
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawRotaGraph(cx,cy,1.0,Angle,GrHandle,(int )Trans,1);
}

void KVARCO::DrawRotaGraph2_H(int GrHandle,float Angle,long x,long y,long cx,long cy,bool Trans,bool call_alpha)
{
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawRotaGraph2(x,y,cx,cy,1.0,Angle,GrHandle,(int )Trans);
}

void KVARCO::DrawGraphAlpha_H(int GrHandle,long x,long y,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	KVARCO::DrawGraph_H(GrHandle,x,y,Trans,true);
}

void KVARCO::DrawRotaGraphAlpha_H(int GrHandle,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	KVARCO::DrawRotaGraph_H(GrHandle,Angle,cx,cy,(int )Trans,true);
}

void KVARCO::DrawRotaGraphAlpha2_H(int GrHandle,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	KVARCO::DrawRotaGraph2_H(GrHandle,Angle,x,y,cx,cy,(int )Trans,true);
}

void KVARCO::DrawZoom_H(int GrHandle,float ZoomRateX,float ZoomRateY,long cx,long cy,bool Trans,bool call_alpha)
{
	int w,h;
	DxLib::GetGraphSize(GrHandle,&w,&h);
	long Halfx=(w/2)	*ZoomRateX;
	long Halfy=(h/2)	*ZoomRateY;
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawExtendGraph(cx-Halfx,cy-Halfy,cx+Halfx,cy+Halfy,GrHandle,(int )Trans);
}

void KVARCO::DrawZoomAlpha_H(int GrHandle,float ZoomRateX,float ZoomRateY,long x,long y,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	KVARCO::DrawZoom_H(GrHandle,ZoomRateX,ZoomRateY,x,y,(int )Trans,true);
}

void KVARCO::DrawRotaZoom_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,bool Trans,bool call_alpha)
{
	int w,h;
	DxLib::GetGraphSize(GrHandle,&w,&h);
	float Halfx=(w/2)	*ZoomRateX;
	float Halfy=(h/2)	*ZoomRateY;

	dPOINT tmp[4];

	tmp[0].x=cx-Halfx;	tmp[0].y=cy-Halfy;
	tmp[1].x=cx+Halfx;	tmp[1].y=cy-Halfy;
	tmp[2].x=cx+Halfx;	tmp[2].y=cy+Halfy;
	tmp[3].x=cx-Halfx;	tmp[3].y=cy+Halfy;

	float sin_=sin(Angle);
	float _sin_=-sin_;
	float cos_=cos(Angle);

	dPOINT res[4];
	//加法定理
	for(int i=0; i<4; i++)
	{
		res[i].x=	_sin_*	tmp[i].y+	cos_*	tmp[i].x+	cx;
		res[i].y=	cos_*	tmp[i].y+	sin_*	tmp[i].x+	cy;
	}

	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawModiGraph(
		static_cast<int>(res[0].x),static_cast<int>(res[0].y),
		static_cast<int>(res[1].x),static_cast<int>(res[1].y),
		static_cast<int>(res[2].x),static_cast<int>(res[2].y),
		static_cast<int>(res[3].x),static_cast<int>(res[3].y),
		GrHandle,(int )Trans);
}

//void KVARCO::DrawRotaZoom2_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,bool Trans);
void KVARCO::DrawRotaZoomAlpha_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	KVARCO::DrawRotaZoom_H(GrHandle,ZoomRateX,ZoomRateY,Angle,cx,cy,Trans,true);
}
//void KVARCO::DrawRotaZoomAlpha2_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans);


///////////////////////////////////////////
//////////グラフィックネーム指定型///////////
///////////////////////////////////////////
#define USE_GR_HANDLE(grhandle,name) int grhandle=GetGrHandle(name);if(grhandle!=-1)

void KVARCO::DrawGraph_N(xtal::String GrName,long x,long y,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) KVARCO::DrawGraph_H(GrHandle,x,y,Trans);
}
void KVARCO::DrawRotaGraph_N(xtal::String GrName,float Angle,long cx,long cy,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) KVARCO::DrawRotaGraph_H(GrHandle,Angle,cx,cy,Trans);
}
void KVARCO::DrawRotaGraph2_N(xtal::String GrName,float Angle,long x,long y,long cx,long cy,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaGraph2_H(GrHandle,Angle,x,y,cx,cy,Trans);
}
void KVARCO::DrawGraphAlpha_N(xtal::String GrName,long x,long y,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawGraphAlpha_H(GrHandle,x,y,Alpha,Trans);
}
void KVARCO::DrawRotaGraphAlpha_N(xtal::String GrName,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaGraphAlpha_H(GrHandle,Angle,cx,cy,Alpha,Trans);
}
void KVARCO::DrawRotaGraphAlpha2_N(xtal::String GrName,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaGraphAlpha2_H(GrHandle,Angle,x,y,cx,cy,Alpha,Trans);
}
void KVARCO::DrawZoom_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,long x,long y,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawZoom_H(GrHandle,ZoomRateX,ZoomRateY,x,y,Trans);
}
void KVARCO::DrawZoomAlpha_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,long x,long y,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawZoomAlpha_H(GrHandle,ZoomRateX,ZoomRateY,x,y,Alpha,Trans);
}
void KVARCO::DrawRotaZoom_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaZoom_H(GrHandle,ZoomRateX,ZoomRateY,ZoomRateY,cx,cy,Trans);
}
//void KVARCO::DrawRotaZoom2_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,bool Trans);
void KVARCO::DrawRotaZoomAlpha_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaZoomAlpha_H(GrHandle,ZoomRateX,ZoomRateY,Angle,cx,cy,Alpha,Trans);
}
//void KVARCO::DrawRotaZoomAlpha2_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans);
