#pragma once
#include "KVARCO.h"

class CBezier
{
	DWORD Pos;
	vector<dPOINTPtr> Points;
public:
	CBezier(xtal::ArrayPtr points,float speed,DWORD max_=1024);
	dPOINTPtr GetPoint(DWORD pos);
	dPOINTPtr GetNextPoint();

	static void bind(xtal::ClassPtr it);
};