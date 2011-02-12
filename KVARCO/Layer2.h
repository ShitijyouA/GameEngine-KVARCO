#pragma once

class CLayer;
class CLayerManager;

typedef xtal::SmartPtr<CLayer>	LayerPtr;
typedef CLayerManager*	LayerMngrPtr;
#include "ActorList.h"

class CLayer
{
	dRECT		Area;
	ActorList	Actors;

	int Z;
	xtal::String Name;
	int Handle;

public:
	CLayer(xtal::String name);
	CLayer();
	void Init(xtal::String name);	//Call from CLayerManager only.

	void DrawThis();
	SET_GET(dRECT,Area)

	SET_GET(int,Z)
	GETTER(xtal::String,Name)

#pragma optimize("",off)
	string GetName_s() const { return Name.c_str(); }
#pragma optimize("",on)

	GETTER(int,Handle)

	dPOINT TransPointLocal_p(dPOINT point);
	dPOINT TransPointGlobal_p(dPOINT point);
	dPOINT TransPointLocal(float x,float y);
	dPOINT TransPointGlobal(float x,float y);

	void AddActor(ActorPtr actor);
	void EraseActor(ActorPtr actor);

	void ReleaseAllActor();
};

struct tag_Name		{};
struct tag_Handle	{};
typedef multi_index_container
	<
		CLayer*,
		indexed_by
		<
			ordered_unique<tag<tag_Name>,	const_mem_fun<CLayer,string,&CLayer::GetName_s	> >,
			ordered_unique<tag<tag_Z>,		const_mem_fun<CLayer,int,	&CLayer::GetZ		> >,
			ordered_unique<tag<tag_Handle>,	const_mem_fun<CLayer,int,	&CLayer::GetHandle	> >
		>
	> LayerMap;

typedef LayerMap::index<tag_Name>::type		LayerMap_tag_Name;
typedef LayerMap::index<tag_Z>::type		LayerMap_tag_Z;
typedef LayerMap::index<tag_Handle>::type	LayerMap_tag_Handle;

class CLayerManager
{
	LayerMap		Layers;
	queue<CLayer*>	LayerPool;

public:
	CLayer* GetPtr(int layer_handle);
	CLayer* GetPtr(xtal::StringPtr layer_name);

	int		GetHandle(xtal::StringPtr layer_name);

	void	DrawAll();
	void	DrawLayer	(int layer_handle);

	void		SetArea		(int layer_handle,dRECT area);
	void		SetZ		(int layer_handle,int z);
	dRECTPtr	GetArea		(int layer_handle);
	int			GetZ		(int layer_handle);
	xtal::StringPtr GetName(int layer_handle);

	dPOINTPtr	TransPointLocal_p(int layer_handle,dPOINT point);
	dPOINTPtr	TransPointGlobal_p(int layer_handle,dPOINT point);
	dPOINTPtr	TransPointLocal(int layer_handle,float x,float y);
	dPOINTPtr	TransPointGlobal(int layer_handle,float x,float y);

	void	AddActor	(int layer_handle,ActorPtr actor);
	void	EraseActor	(int layer_handle,ActorPtr actor);

	//Layer	//Use LayerPool
	int		NewLayer(xtal::StringPtr layer_name,int z,dRECT area);
	void	DeleteLayer(xtal::String layer_name);

	static void	bind(const xtal::ClassPtr it);
	SINGLETON_PATTERNX(CLayerManager,LayerMngrPtr)
	void ReleaseAllLayer();
	void Release()
	{
		ReleaseAllLayer();
		//SAFE_DELETE(Inst);
	}
};

#include "ActorManager.h"