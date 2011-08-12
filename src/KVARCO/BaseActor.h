#pragma once

class BaseActor
{
public:
	typedef xtal::AnyPtr BaseActorPtrX;

private:
	bool Dead;
	xtal::AnyPtr Parent;

public:
	int				Z;
	DWORD			ID;
	DWORD			ActorType;
	xtal::StringPtr	LayerName;
	xtal::FiberPtr	Run;

	BaseActor();

	void init(xtal::StringPtr belong_layer,DWORD actor_type,int z,xtal::AnyPtr parent);

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

typedef xtal::SmartPtr<BaseActor>	BaseActorPtrX;
