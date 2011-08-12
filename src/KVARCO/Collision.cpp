#include "pch.h"
#include "Collision.h"
#include "ActorManager.h"
#include "KVARCO.h"

ColPolygonPtrX ColPolygon::VoidPolygon=xtal::null;

ColPolygon::ColPolygon(DWORD poly_num)
{
	if(xtal::is_null(VoidPolygon) && poly_num!=0)
	{
		VoidPolygon=xtal::xnew<ColPolygon>(0);
		VoidPolygon->polygon_.clear();
	}

	if(poly_num<=2) poly_num=3;
	polygon_.resize(poly_num+1); polygon_.clear();
	for(DWORD i=0; i<(poly_num+1); i++) polygon_.push_back(fPoint());
}

void ColPolygon::SetAABB()
{
	aabb_.top_	=LONG_MIN;	aabb_.bottom_	=LONG_MAX;
	aabb_.left_	=LONG_MIN;	aabb_.right_	=LONG_MAX;

	BOOST_FOREACH(fPoint& i,polygon_)
	{
		long ix=static_cast<long>(i.x);
		long iy=static_cast<long>(i.y);

		if(aabb_.top_	<=iy)	aabb_.top_		=iy;
		if(aabb_.bottom_>=iy)	aabb_.bottom_	=iy;
		if(aabb_.left_	<=ix)	aabb_.left_		=ix;
		if(aabb_.right_	>=ix)	aabb_.right_	=ix;
	}
}

bool ColPolygon::CheckAABB(ColPolygonPtrX col)
{
	return
	(	aabb_.top_		>col->aabb_.bottom_	&&
		aabb_.bottom_	<col->aabb_.top_	&&
		aabb_.left_		>col->aabb_.right_	&&
		aabb_.right_	<col->aabb_.left_	);
}

bool ColPolygon::ColWithPoint(float x,float y)
{
	DWORD size=polygon_.size();
	if(size <= 2) return false;
	DWORD count = 0;
	float lastx = polygon_[0].x;
	float lasty = polygon_[0].y;
	float curx;
	float cury;
	for(DWORD i=size; i>0;lastx = curx,lasty =cury)
	{
		--i;
		curx = polygon_[i].x;
		cury = polygon_[i].y;
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

bool ColPolygon::ColWithPoint(fPoint Point)
{
	return ColWithPoint(Point.x,Point.y);
}

bool ColPolygon::Check(ColPolygonPtrX col)
{
	if(polygon_.size()<=2)	return false;
	if(!CheckAABB(col))		return false;

	for(DWORD i=0; i<polygon_.size()-1; i++)
		if(col->ColWithPoint(polygon_[i]))	return true;

	for(DWORD i=0; i<col->polygon_.size(); i++)
		if(ColWithPoint(col->polygon_[i]))	return true;

	return false;
}

ColPolygonPtrX ColPolygon::Move(float cx,float cy,float deg)
{
	if(polygon_.size() <= 2) return xtal::null;

	DWORD size=polygon_.size()-1;
	ColPolygonPtrX out=xtal::xnew<ColPolygon>(size);

	//先に計算しておく
	if(deg==0.0)
	{
		for(DWORD i=0; i<size; ++i)
		{
			out->polygon_[i].x=polygon_[i].x+	cx;
			out->polygon_[i].y=polygon_[i].y+	cy;
		}
	}
	else
	{
		Radian angle(deg);
		float sin_	=sin(angle.GetAsRadian());
		float cos_	=cos(angle.GetAsRadian());

		for(DWORD i=0; i<size; ++i)
		{
			out->polygon_[i].x=-sin_*polygon_[i].y+cos_*polygon_[i].x+cx;
			out->polygon_[i].y= cos_*polygon_[i].y+sin_*polygon_[i].x+cy;
		}
	}

	out->SetAABB();
	out->polygon_[size]=out->polygon_[0];

	return out;
}

ColPolygonPtrX ColPolygon::Movep(fPoint cPoint,float deg)
{
	return Move(cPoint.x,cPoint.y,deg);
}

ColPolygonPtrX ColPolygon::Zoom(float scale)
{
	DWORD size=polygon_.size()-1;
	if(size < 2) return xtal::null;
	ColPolygonPtrX out=xtal::xnew<ColPolygon>(size);
	
	for(DWORD i=0; i<=size; ++i)
	{
		out->polygon_[i].x=polygon_[i].x;
		out->polygon_[i].y=polygon_[i].y;

		if(scale!=1.0)
		{
			out->polygon_[i].x*=scale;
			out->polygon_[i].y*=scale;
		}
	}

	out->SetAABB();
	return out;
}

void ColPolygon::SetBox(float width,float height)
{
	if(polygon_.size()!=5) return;

	float width_2=width*0.5f,height_2=height*0.5f;
	polygon_[0].x=-width_2;	polygon_[0].y=-height_2;
	polygon_[1].x=width_2;	polygon_[1].y=-height_2;
	polygon_[2].x=width_2;	polygon_[2].y=height_2;
	polygon_[3].x=-width_2;	polygon_[3].y=height_2;
	polygon_[4]=polygon_[0];
}

void ColPolygon::SetPolygon(float radius,float offset_deg)
{
	if(polygon_.size() <= 2) return;

	DWORD poly_size=polygon_.size()-1;
	Degree crad(360.0f/static_cast<float>(poly_size));
	Degree tmp_offset(offset_deg);

	for(DWORD i=0; i<poly_size; i++)
	{
		polygon_[i].x=std::cos(tmp_offset.GetAsRadian())*radius;
		polygon_[i].y=std::sin(tmp_offset.GetAsRadian())*radius;
		tmp_offset+=crad;
	}
	polygon_[poly_size]=polygon_[0];
}

void ColPolygon::SetPolygon2(xtal::ArrayPtr polygon)
{
	if(xtal::is_undefined(polygon))	return;
	if(polygon->size()<=2)			return;
	polygon_.clear();

	for(DWORD i=0; i<polygon->size(); ++i)
	{
		xtal::SmartPtr<fPoint> tmp=xtal::unchecked_ptr_cast<fPoint>(polygon->at(i));
		polygon_.push_back(fPoint(tmp->x,tmp->y));
	}

	DWORD end_index		=polygon_.size()-1;
	polygon_[end_index].x=polygon_[0].x;
	polygon_[end_index].y=polygon_[0].y;
}

void ColPolygon::DrawCollision(xtal::String layer_name,DWORD color)
{
	if(polygon_.size() <= 2) return;

	fRectPtrX area=LayerManager::GetInst()->GetArea(LayerManager::GetInst()->GetHandle(layer_name.c_str()));
	for(DWORD i=0; i<polygon_.size()-1; i++)
	{
		kvarco::DrawLine(
			static_cast<int>(polygon_[i].x	+area->left_	),
			static_cast<int>(polygon_[i].y	+area->top_		),
			static_cast<int>(polygon_[i+1].x+area->left_	),
			static_cast<int>(polygon_[i+1].y+area->top_		),
			color
			);
	}
}

void ColPolygon::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(ColPolygon);

	Xdef_method(SetBox);
	Xdef_method(SetPolygon);
	Xdef_method(SetPolygon2);
	Xdef_method(Movep);
	Xdef_method(Move);
	Xdef_method(Zoom);
	Xdef_method(Check);
	Xdef_method(Cancel);
	Xdef_method(DrawCollision);
}