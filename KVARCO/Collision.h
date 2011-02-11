#pragma once
#include "GeoStruct.h"

class CColPolygon;
typedef xtal::SmartPtr<CColPolygon> CColPolygonPtr;

typedef vector<dPOINT> POLYGON;
class CColPolygon
{
protected:
//	friend class CColPolygon;
	POLYGON Polygon;		//e¸_
	lRECT	AABB;			//AABB

	bool ColWithPoint(dPOINT point);
	bool ColWithPoint(float x,float y);

	static CColPolygonPtr VoidPolygon;

	virtual void SetAABB();
	bool CheckAABB(CColPolygonPtr col);						//åGcÈ»èð·é
public:
	CColPolygon(DWORD poly_num);							//¸_
	virtual void SetBox(float width,float height);			//é`ðZbg
	virtual void SetPolygon(float radius,float offset_deg);	//³poly_nump`ðZbgBsizeÍ¼a
	virtual void SetPolygon2(xtal::ArrayPtr polygon);

	CColPolygonPtr Movep(dPOINT cpoint,float deg);			//ñ]AÚ®
	CColPolygonPtr Move(float cx,float cy,float deg);

	bool Check(CColPolygonPtr col);
	CColPolygonPtr Cancel()	{	return VoidPolygon;	}

	void DrawCollision(xtal::String layer_name,DWORD color);

	static void Destroy()
		{	VoidPolygon=xtal::null;	}
};
