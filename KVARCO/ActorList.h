#pragma once
#include "ScriptManager.h"
#include "BaseActor.h"

typedef xtal::AnyPtr		ActorPtrX;
struct tag_ID	{};
struct tag_Z	{};
struct tag_Type	{};
namespace kvarco
{
	extern DWORD GetZ_BaseActor(xtal::AnyPtr actor);
	extern DWORD GetID_BaseActor(xtal::AnyPtr actor);
	extern DWORD GetType_BaseActor(xtal::AnyPtr actor);
}

typedef boost::multi_index::multi_index_container
	<
		xtal::AnyPtr,
		indexed_by
		<
			sequenced<			tag<tag_Seq> >,
			ordered_non_unique<	tag<tag_Z>,			global_fun<xtal::AnyPtr,DWORD,kvarco::GetZ_BaseActor>	>,
			hashed_unique<		tag<tag_ID>,		global_fun<xtal::AnyPtr,DWORD,kvarco::GetID_BaseActor>	>,	//Z(LayerManagerと連携)
			ordered_non_unique<	tag<tag_Type>,		global_fun<xtal::AnyPtr,DWORD,kvarco::GetType_BaseActor> >	//ActorType
		>
	> ActorList;

typedef ActorList::index<tag_Seq>::type		ActorList_tag_Seq;
typedef ActorList::index<tag_ID>::type		ActorList_tag_ID;
typedef ActorList::index<tag_Z>::type		ActorList_tag_Z;
typedef ActorList::index<tag_Type>::type	ActorList_tag_Type;