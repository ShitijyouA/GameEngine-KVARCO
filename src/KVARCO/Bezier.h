#pragma once
#include "KVARCO.h"

class Bezier
{
	DWORD Pos;
	std::vector<fPointPtrX> Points;
public:
	Bezier(xtal::ArrayPtr Points,float speed,DWORD max_=1024);
	fPointPtrX GetPoint(DWORD pos);
	fPointPtrX GetNextPoint();

	static void bind(xtal::ClassPtr it);
};