#pragma once
#include "Utillity.h"

class CBaseScene
{
public:
	CBaseScene()
	{
		Run	=xtal::null;
	}

	xtal::AnyPtr Run;

	static void bind(xtal::ClassPtr it)
	{
		USE_XDEFZ(CBaseScene);
		Xdef_var(Run);
	}
};

typedef xtal::SmartPtr<CBaseScene> BaseScenePtrX;