#pragma once
#include "GeoStruct.h"

class ColPolygon;
typedef xtal::SmartPtr<ColPolygon> ColPolygonPtrX;

class ColPolygon
{
public:
	typedef std::vector<fPoint> PolygonType;

protected:
	PolygonType polygon_;		//各頂点
	lRect		aabb_;			//AABB

	bool ColWithPoint(fPoint Point);
	bool ColWithPoint(float x,float y);

	static ColPolygonPtrX VoidPolygon;

	virtual void SetAABB();
	bool CheckAABB(ColPolygonPtrX col);						//大雑把な判定をする

public:
	ColPolygon(DWORD poly_num);								//頂点数
	virtual void SetBox(float width,float height);			//矩形をセット
	virtual void SetPolygon(float radius,float offset_deg);	//正poly_num角形をセット。sizeは半径
	virtual void SetPolygon2(xtal::ArrayPtr polygon);

	ColPolygonPtrX Movep(fPoint cPoint,float deg);			//回転、移動
	ColPolygonPtrX Move(float cx,float cy,float deg);

	bool Check(ColPolygonPtrX col);
	ColPolygonPtrX Cancel()	{	return VoidPolygon;	}

	void DrawCollision(xtal::String layer_name,DWORD color);

	static void Release()
		{	VoidPolygon=xtal::null;	}
};
