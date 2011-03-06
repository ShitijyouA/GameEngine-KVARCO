#pragma once
#include "KVARCO.h"

class CBaseActor
{
	bool Dead;
	xtal::AnyPtr Parent;

public:

	dPoint	Point;
	int		Z;

	DWORD ID;
	DWORD ActorType;
	xtal::StringPtr LayerName;

	xtal::FiberPtr Run;

	CBaseActor()
		:ID(KVARCO::MakeHandle()),Dead(false)
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

	~CBaseActor()
	{
		Parent	=xtal::null;
		Run		=xtal::null;
		//KVARCO::DebugOut("ActorDead\n");
	}

	static void bind(xtal::ClassPtr it)
	{
		USE_XDEFZ(CBaseActor);

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

typedef xtal::SmartPtr<CBaseActor> BaseActorPtr;