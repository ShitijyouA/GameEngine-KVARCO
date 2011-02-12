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
		XTAL_BIND_VAR_DEFNAME(CBaseScene,Run);
	}
};

typedef xtal::SmartPtr<CBaseScene> BaseScenePtr;