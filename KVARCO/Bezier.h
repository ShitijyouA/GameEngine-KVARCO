#pragma once
#include "KVARCO.h"

class CBezier
{
	DWORD Pos;
	vector<dPointPtr> Points;
public:
	CBezier(xtal::ArrayPtr Points,float speed,DWORD max_=1024);
	dPointPtr GetPoint(DWORD pos);
	dPointPtr GetNextPoint();

	static void bind(xtal::ClassPtr it);
};