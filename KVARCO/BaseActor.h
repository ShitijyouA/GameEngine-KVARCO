#pragma once
#include "KVARCO.h"

class CBaseActor
{
	bool Dead;
	xtal::AnyPtr Parent;

public:

	dPOINT	Point;
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
		XTAL_BIND_VAR_DEFNAME(CBaseActor,Point);
		XTAL_BIND_VAR_DEFNAME(CBaseActor,Z);
		XTAL_BIND_VAR_DEFNAME(CBaseActor,ID);
		XTAL_BIND_VAR_DEFNAME(CBaseActor,ActorType);
		XTAL_BIND_VAR_DEFNAME(CBaseActor,Parent);
		XTAL_BIND_VAR_DEFNAME(CBaseActor,LayerName);
		XTAL_BIND_VAR_DEFNAME(CBaseActor,Run);

		BIND_XTAL_CLASSFUN_DEFNAME_IT(CBaseActor,init);
		BIND_XTAL_CLASSFUN_DEFNAME_IT(CBaseActor,Die);
		BIND_XTAL_CLASSFUN_DEFNAME_IT(CBaseActor,IsDead);
	}
};

typedef xtal::SmartPtr<CBaseActor> BaseActorPtr;