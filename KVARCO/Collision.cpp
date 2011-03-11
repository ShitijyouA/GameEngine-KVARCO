#include "pch.h"
#include "Collision.h"
#include "ActorManager.h"

ColPolygonPtrX CColPolygon::VoidPolygon=xtal::null;

CColPolygon::CColPolygon(DWORD poly_num)
{
	if(xtal::is_null(VoidPolygon) && poly_num!=0)
	{
		VoidPolygon=xtal::xnew<CColPolygon>(0);
		VoidPolygon->Polygon.clear();
	}

	if(poly_num<=2) poly_num=3;
	Polygon.resize(poly_num+1); Polygon.clear();
	for(DWORD i=0; i<(poly_num+1); i++) Polygon.push_back(dPoint());
}

void CColPolygon::SetAABB()
{
	AABB.top	=LONG_MIN;	AABB.bottom	=LONG_MAX;
	AABB.left	=LONG_MIN;	AABB.right	=LONG_MAX;

	BOOST_FOREACH(dPoint& i,Polygon)
	{
		long ix=static_cast<long>(i.x);
		long iy=static_cast<long>(i.y);

		if(AABB.top		<=iy) AABB.top		=iy;
		if(AABB.bottom	>=iy) AABB.bottom	=iy;
		if(AABB.left	<=ix) AABB.left		=ix;
		if(AABB.right	>=ix) AABB.right	=ix;
	}
}

bool CColPolygon::CheckAABB(ColPolygonPtrX col)
{
	return
	(	AABB.top	>col->AABB.bottom	&&
		AABB.bottom	<col->AABB.top		&&
		AABB.left	>col->AABB.right	&&
		AABB.right	<col->AABB.left		);
}

bool CColPolygon::ColWithPoint(float x,float y)
{
	DWORD size=Polygon.size();
	if(size <= 2) return false;
	DWORD count = 0;
	float lastx = Polygon[0].x;
	float lasty = Polygon[0].y;
	float curx;
	float cury;
	for(DWORD i=size; i>0;lastx = curx,lasty =cury)
	{
		--i;
		curx = Polygon[i].x;
		cury = Polygon[i].y;
		if(cury == lasty)				continue;

		float leftx;
		if(curx < lastx)
		{
			if(x >= lastx)				continue;
			leftx = curx;
		}
		else
		{
			if(x >= curx)				continue;
			leftx = lastx;
		}

		float test1, test2;
		if(cury < lasty)
		{
			if(y < cury || y >= lasty)	continue;
			if(x < leftx)
			{
				count++;
				continue;
			}
			test1 = x - curx;
			test2 = y - cury;
		}
		else
		{
			if(y < lasty || y >= cury)	continue;
			if(x < leftx)
			{
				count++;
				continue;
			}
			test1 = x - lastx;
			test2 = y - lasty;
		}

		if(test1 < (test2 / (lasty - cury) * (lastx - curx)))
			count++;
	}
	return count&1;
}

bool CColPolygon::ColWithPoint(dPoint Point)
{
	return ColWithPoint(Point.x,Point.y);
}

bool CColPolygon::Check(ColPolygonPtrX col)
{
	if(Polygon.size()<=2)	return false;
	if(!CheckAABB(col))		return false;

	for(DWORD i=0; i<Polygon.size()-1; i++)
		if(col->ColWithPoint(Polygon[i]))	return true;

	for(DWORD i=0; i<col->Polygon.size(); i++)
		if(ColWithPoint(col->Polygon[i]))	return true;

	return false;
}

ColPolygonPtrX CColPolygon::Move(float cx,float cy,float deg)
{
	if(Polygon.size() <= 2) return xtal::null;

	DWORD size=Polygon.size()-1;
	ColPolygonPtrX out=xtal::xnew<CColPolygon>(size);

	//先に計算しておく
	if(deg==0.0)
	{
		for(DWORD i=0; i<size; ++i)
		{
			out->Polygon[i].x=Polygon[i].x+	cx;
			out->Polygon[i].y=Polygon[i].y+	cy;
		}
	}
	else
	{
		float rad	=deg*D2R;
		float sin_	=sin(rad);
		float cos_	=cos(rad);

		for(DWORD i=0; i<size; ++i)
		{
			out->Polygon[i].x=-sin_*Polygon[i].y+cos_*Polygon[i].x+cx;
			out->Polygon[i].y= cos_*Polygon[i].y+sin_*Polygon[i].x+cy;
		}
	}

	out->SetAABB();
	out->Polygon[size]=out->Polygon[0];

	return out;
}

ColPolygonPtrX CColPolygon::Movep(dPoint cPoint,float deg)
{
	return Move(cPoint.x,cPoint.y,deg);
}

void CColPolygon::SetBox(float width,float height)
{
	if(Polygon.size()!=5) return;

	float width_2=width*0.5f,height_2=height*0.5f;
	Polygon[0].x=-width_2;	Polygon[0].y=-height_2;
	Polygon[1].x=width_2;	Polygon[1].y=-height_2;
	Polygon[2].x=width_2;	Polygon[2].y=height_2;
	Polygon[3].x=-width_2;	Polygon[3].y=height_2;
	Polygon[4]=Polygon[0];
}

void CColPolygon::SetPolygon(float radius,float offset_deg)
{
	if(Polygon.size() <= 2) return;

	DWORD polygon=Polygon.size()-1;
	float crad=M_PI2/polygon;
	float rad=-offset_deg*D2R;
	for(DWORD i=0; i<polygon; i++)
	{
		Polygon[i].x=cos(rad)*radius;
		Polygon[i].y=sin(rad)*radius;
		rad+=crad;
	}
	Polygon[polygon]=Polygon[0];
}

void CColPolygon::SetPolygon2(xtal::ArrayPtr polygon)
{
	if(xtal::is_undefined(polygon))	return;
	if(polygon->size()<=2)			return;
	Polygon.clear();

	for(DWORD i=0; i<polygon->size(); ++i)
	{
		xtal::SmartPtr<dPoint> tmp=xtal::unchecked_ptr_cast<dPoint>(polygon->at(i));
		Polygon.push_back(dPoint(tmp->x,tmp->y));
	}

	DWORD end_index		=Polygon.size()-1;
	Polygon[end_index].x=Polygon[0].x;
	Polygon[end_index].y=Polygon[0].y;
}

void CColPolygon::DrawCollision(xtal::String layer_name,DWORD color)
{
	if(Polygon.size() <= 2) return;

	dRectPtrX area=CLayerManager::GetInst()->GetArea(CLayerManager::GetInst()->GetHandle(layer_name.c_str()));
	for(DWORD i=0; i<Polygon.size()-1; i++)
	{
		KVARCO::DrawLine(
			static_cast<int>(Polygon[i].x	+area->left),
			static_cast<int>(Polygon[i].y	+area->top),
			static_cast<int>(Polygon[i+1].x	+area->left),
			static_cast<int>(Polygon[i+1].y	+area->top),
			color
			);
	}
}