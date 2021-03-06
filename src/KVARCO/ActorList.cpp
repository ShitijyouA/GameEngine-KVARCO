﻿#include "pch.h"
#include "ActorList.h"

namespace kvarco
{
	DWORD GetZ_BaseActor(xtal::AnyPtr actor)
	{
		BaseActorPtrX ap=xtal::unchecked_ptr_cast<BaseActor>(actor);
		return ap->Z;
	}

	DWORD GetID_BaseActor(xtal::AnyPtr actor)
	{
		BaseActorPtrX ap=xtal::unchecked_ptr_cast<BaseActor>(actor);
		return ap->ID;
	}

	DWORD GetType_BaseActor(xtal::AnyPtr actor)
	{
		BaseActorPtrX ap=xtal::unchecked_ptr_cast<BaseActor>(actor);
		return ap->ActorType;
	}
}