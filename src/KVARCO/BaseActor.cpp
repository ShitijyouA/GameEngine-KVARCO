#include "pch.h"
#include "BaseActor.h"
#include "KVARCO.h"

BaseActor::BaseActor()
	:ID(kvarco::MakeHandle()),Dead(false)
{
	Z=			0;
	ActorType=	0;
	LayerName=	xtal::StringPtr("root");
	Parent=		xtal::null;

	Run=		xtal::null;
}

void BaseActor::init(xtal::StringPtr belong_layer,DWORD actor_type,int z,xtal::AnyPtr parent)
{
	LayerName	=belong_layer;
	ActorType	=actor_type;
	Z			=z;
	Parent		=parent;
}
