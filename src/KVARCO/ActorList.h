#pragma once
#include "BaseActor.h"
#include "Tag.h"

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
			sequenced			<	kvarco::tag::detail::Sequence																>,
			ordered_non_unique	<	kvarco::tag::detail::Z_depth	,global_fun<xtal::AnyPtr,DWORD,kvarco::GetZ_BaseActor>		>,
			hashed_unique		<	kvarco::tag::detail::ID			,global_fun<xtal::AnyPtr,DWORD,kvarco::GetID_BaseActor>		>,	//Z(LayerManagerと連携)
			ordered_non_unique	<	kvarco::tag::detail::Type		,global_fun<xtal::AnyPtr,DWORD,kvarco::GetType_BaseActor>	>	//ActorType
		>
	> ActorList;

typedef ActorList::index<kvarco::tag::Sequence>::type	ActorList_tag_Seq;
typedef ActorList::index<kvarco::tag::ID>::type			ActorList_tag_ID;
typedef ActorList::index<kvarco::tag::Z_depth>::type	ActorList_tag_Z;
typedef ActorList::index<kvarco::tag::Type>::type		ActorList_tag_Type;