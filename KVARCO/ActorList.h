#pragma once
#include "ScManager.h"
#include "BaseActor.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
using namespace boost;
using namespace boost::multi_index;

typedef xtal::AnyPtr		ActorPtr;
struct tag_ID	{};
struct tag_Z	{};
struct tag_Type	{};
namespace KVARCO
{
	extern DWORD GetZ_BaseActor(xtal::AnyPtr actor);
	extern DWORD GetID_BaseActor(xtal::AnyPtr actor);
	extern DWORD GetType_BaseActor(xtal::AnyPtr actor);
}

typedef multi_index_container
	<
		xtal::AnyPtr,
		indexed_by
		<
			sequenced<			tag<tag_Seq> >,
			ordered_non_unique<	tag<tag_Z>,			global_fun<xtal::AnyPtr,DWORD,KVARCO::GetZ_BaseActor>	>,
			hashed_unique<		tag<tag_ID>,		global_fun<xtal::AnyPtr,DWORD,KVARCO::GetID_BaseActor>	>,	//Z(LayerManager‚Æ˜AŒg)
			ordered_non_unique<	tag<tag_Type>,		global_fun<xtal::AnyPtr,DWORD,KVARCO::GetType_BaseActor> >	//ActorType
		>
	> ActorList;

typedef ActorList::index<tag_Seq>::type		ActorList_tag_Seq;
typedef ActorList::index<tag_ID>::type		ActorList_tag_ID;
typedef ActorList::index<tag_Z>::type		ActorList_tag_Z;
typedef ActorList::index<tag_Type>::type	ActorList_tag_Type;