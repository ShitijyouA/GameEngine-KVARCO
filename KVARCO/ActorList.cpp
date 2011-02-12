#include "pch.h"
#include "ActorList.h"

namespace KVARCO
{
	DWORD GetZ_BaseActor(xtal::AnyPtr actor)
	{
		BaseActorPtr ap=xtal::unchecked_ptr_cast<CBaseActor>(actor);
		return ap->Z;
	}

	DWORD GetID_BaseActor(xtal::AnyPtr actor)
	{
		BaseActorPtr ap=xtal::unchecked_ptr_cast<CBaseActor>(actor);
		return ap->ID;
	}

	DWORD GetType_BaseActor(xtal::AnyPtr actor)
	{
		BaseActorPtr ap=xtal::unchecked_ptr_cast<CBaseActor>(actor);
		return ap->ActorType;
	}
}