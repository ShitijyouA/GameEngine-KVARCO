#pragma once

template<typename TempIdType>
class BasicActor_impl
{
public:
	typedef TempIdType					IdType;
	typedef kvarco::uint32_t			TypeOfActorType;
	typedef xtal::SmartPtr<BasicActor>	BasicActorPtrX;

private:
	bool			is_dead_;
	IdType			id_;
	TypeOfActorType	type_;

public:
//    int				Z;
//    xtal::StringPtr	LayerName;
//    xtal::FiberPtr	Run;

	BasicActor();

	void init(TypeOfActorType actor_type);

	void Die()		{ is_dead_=true;	}
	bool IsDead()	{ return is_dead_;	}

	~BasicActor()
		{
//            Run		=xtal::null;
			//kvarco::DebugOut("ActorDead\n");
		}

	static void bind(xtal::ClassPtr& it)
		{
			USE_XDEFZ(BasicActor);

//            Xdef_var(Z);
			Xdef_var(ID);
			Xdef_var(ActorType);
//            Xdef_var(LayerName);
//            Xdef_var(Run);

			Xdef_method(init);
			Xdef_method(Die);
			Xdef_method(IsDead);
		}
};

template<typename ActorType>
class ScopedActorKiller
{
	typedef xtal::SmartPtr<ActorType> ActorPtrX;

	ActorTypeX actor_instance_;

public:
	ScopedActorKiller(ActorTypeX inst)
		:actor_instance_(inst)
		{}

	virtual ~ScopedActorKiller()
		{
			actor_instance_.Die();
		}
};

template<TempIdType>
class RunableActor
	:BasicActor<TempIdType>
{
};
