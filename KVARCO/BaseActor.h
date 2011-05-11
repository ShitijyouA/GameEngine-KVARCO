#pragma once
#include "KVARCO.h"

class BaseActor
{
	bool Dead;
	xtal::AnyPtr Parent;

public:

	fPoint	Point;
	int		Z;

	DWORD ID;
	DWORD ActorType;
	xtal::StringPtr LayerName;

	xtal::FiberPtr Run;

	BaseActor()
		:ID(kvarco::MakeHandle()),Dead(false)
	{
		Z=			0;
		ActorType=	0;
		LayerName=	xtal::StringPtr("root");
		Parent=		xtal::null;

		Run=		xtal::null;
	}

	void init(xtal::StringPtr belong_layer,DWORD actor_type,int z,xtal::AnyPtr parent)
	{
		LayerName	=belong_layer;
		ActorType	=actor_type;
		Z			=z;
		Parent		=parent;
	}

	void Die()		{ Dead=true;		}
	bool IsDead()	{ return Dead;		}

	~BaseActor()
	{
		Parent	=xtal::null;
		Run		=xtal::null;
		//kvarco::DebugOut("ActorDead\n");
	}

	static void bind(xtal::ClassPtr it)
	{
		USE_XDEFZ(BaseActor);

		Xdef_var(Point);
		Xdef_var(Z);
		Xdef_var(ID);
		Xdef_var(ActorType);
		Xdef_var(Parent);
		Xdef_var(LayerName);
		Xdef_var(Run);

		Xdef_method(init);
		Xdef_method(Die);
		Xdef_method(IsDead);
	}
};

typedef xtal::SmartPtr<BaseActor> BaseActorPtrX;