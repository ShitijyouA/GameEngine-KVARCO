#include "pch.h"
#include "KVARCO.h"
using namespace kvarco;

///////////////////////////////////////////
/////////グラフィックハンドル指定型//////////
///////////////////////////////////////////
void kvarco::DrawGraph_H(int GrHandle,long x,long y,bool Trans,bool call_alpha)
{
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawGraph(x,y,GrHandle,(int )Trans);
}

void kvarco::DrawRotaGraph_H(int GrHandle,float Angle,long cx,long cy,bool Trans,bool call_alpha)
{
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawRotaGraph(cx,cy,1.0,Angle,GrHandle,(int )Trans,1);
}

void kvarco::DrawRotaGraph2_H(int GrHandle,float Angle,long x,long y,long cx,long cy,bool Trans,bool call_alpha)
{
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawRotaGraph2(x,y,cx,cy,1.0,Angle,GrHandle,(int )Trans);
}

void kvarco::DrawGraphAlpha_H(int GrHandle,long x,long y,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	kvarco::DrawGraph_H(GrHandle,x,y,Trans,true);
}

void kvarco::DrawRotaGraphAlpha_H(int GrHandle,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	kvarco::DrawRotaGraph_H(GrHandle,Angle,cx,cy,(int )Trans,true);
}

void kvarco::DrawRotaGraphAlpha2_H(int GrHandle,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	kvarco::DrawRotaGraph2_H(GrHandle,Angle,x,y,cx,cy,(int )Trans,true);
}

void kvarco::DrawZoom_H(int GrHandle,float ZoomRateX,float ZoomRateY,long cx,long cy,bool Trans,bool call_alpha)
{
	int w,h;
	DxLib::GetGraphSize(GrHandle,&w,&h);
	long Halfx=static_cast<long>(w*ZoomRateX*0.5f);
	long Halfy=static_cast<long>(h*ZoomRateY*0.5f);
	if(!call_alpha) SetDrawBlendModeLight(DX_BLENDMODE_NOBLEND,255);
	DxLib::DrawExtendGraph(cx-Halfx,cy-Halfy,cx+Halfx,cy+Halfy,GrHandle,(int )Trans);
}

void kvarco::DrawZoomAlpha_H(int GrHandle,float ZoomRateX,float ZoomRateY,long x,long y,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	kvarco::DrawZoom_H(GrHandle,ZoomRateX,ZoomRateY,x,y,(int )Trans,true);
}

void kvarco::DrawRotaZoom_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,bool Trans,bool call_alpha)
{
	int w,h;
	DxLib::GetGraphSize(GrHandle,&w,&h);
	float Halfx=(w/2)	*ZoomRateX;
	float Halfy=(h/2)	*ZoomRateY;

	fPoint tmp[4];

	tmp[0].x=cx-Halfx;	tmp[0].y=cy-Halfy;
	tmp[1].x=cx+Halfx;	tmp[1].y=cy-Halfy;
	tmp[2].x=cx+Halfx;	tmp[2].y=cy+Halfy;
	tmp[3].x=cx-Halfx;	tmp[3].y=cy+Halfy;

	float sin_=sin(Angle);
	float _sin_=-sin_;
	float cos_=cos(Angle);

	fPoint res[4];
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

//void kvarco::DrawRotaZoom2_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,bool Trans);
void kvarco::DrawRotaZoomAlpha_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	SetDrawBlendModeLight(DX_BLENDMODE_ALPHA,(Alpha%256));
	kvarco::DrawRotaZoom_H(GrHandle,ZoomRateX,ZoomRateY,Angle,cx,cy,Trans,true);
}
//void kvarco::DrawRotaZoomAlpha2_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans);


///////////////////////////////////////////
//////////グラフィックネーム指定型///////////
///////////////////////////////////////////
#define USE_GR_HANDLE(grhandle,name) int grhandle=GetGrHandle(name);if(grhandle!=-1)

void kvarco::DrawGraph_N(xtal::String GrName,long x,long y,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) kvarco::DrawGraph_H(GrHandle,x,y,Trans);
}
void kvarco::DrawRotaGraph_N(xtal::String GrName,float Angle,long cx,long cy,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) kvarco::DrawRotaGraph_H(GrHandle,Angle,cx,cy,Trans);
}
void kvarco::DrawRotaGraph2_N(xtal::String GrName,float Angle,long x,long y,long cx,long cy,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaGraph2_H(GrHandle,Angle,x,y,cx,cy,Trans);
}
void kvarco::DrawGraphAlpha_N(xtal::String GrName,long x,long y,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawGraphAlpha_H(GrHandle,x,y,Alpha,Trans);
}
void kvarco::DrawRotaGraphAlpha_N(xtal::String GrName,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaGraphAlpha_H(GrHandle,Angle,cx,cy,Alpha,Trans);
}
void kvarco::DrawRotaGraphAlpha2_N(xtal::String GrName,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaGraphAlpha2_H(GrHandle,Angle,x,y,cx,cy,Alpha,Trans);
}
void kvarco::DrawZoom_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,long x,long y,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawZoom_H(GrHandle,ZoomRateX,ZoomRateY,x,y,Trans);
}
void kvarco::DrawZoomAlpha_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,long x,long y,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawZoomAlpha_H(GrHandle,ZoomRateX,ZoomRateY,x,y,Alpha,Trans);
}
void kvarco::DrawRotaZoom_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaZoom_H(GrHandle,ZoomRateX,ZoomRateY,ZoomRateY,cx,cy,Trans);
}
//void kvarco::DrawRotaZoom2_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,bool Trans);
void kvarco::DrawRotaZoomAlpha_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,int Alpha,bool Trans)
{
	USE_GR_HANDLE(GrHandle,GrName) DrawRotaZoomAlpha_H(GrHandle,ZoomRateX,ZoomRateY,Angle,cx,cy,Alpha,Trans);
}
//void kvarco::DrawRotaZoomAlpha2_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans);
