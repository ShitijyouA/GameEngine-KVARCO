#pragma once
#include "KVARCO.h"

class CBezier
{
	DWORD Pos;
	vector<dPointPtrX> Points;
public:
	CBezier(xtal::ArrayPtr Points,float speed,DWORD max_=1024);
	dPointPtrX GetPoint(DWORD pos);
	dPointPtrX GetNextPoint();

	static void bind(xtal::ClassPtr it);
};