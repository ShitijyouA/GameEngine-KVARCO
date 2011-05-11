#pragma once
#include "Utillity.h"

class BaseScene
{
public:
	BaseScene()
	{
		Run	=xtal::null;
	}

	xtal::AnyPtr Run;

	static void bind(xtal::ClassPtr it)
	{
		USE_XDEFZ(BaseScene);
		Xdef_var(Run);
	}
};

typedef xtal::SmartPtr<BaseScene> BaseScenePtrX;