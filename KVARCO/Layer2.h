#pragma once

class Layer;
class LayerManager;

typedef xtal::SmartPtr<Layer>	LayerPtrX;
typedef LayerManager*	LayerMngrPtr;
#include "ActorList.h"

class Layer
{
	fRect		Area;
	ActorList	Actors;

	int Z;
	xtal::String Name;
	int Handle;

public:
	Layer(xtal::String name);
	Layer();
	void Init(xtal::String name);	//Call from LayerManager only.

	void DrawThis();
	SET_GET(fRect,Area)

	SET_GET(int,Z)
	GETTER(xtal::String,Name)

#pragma optimize("",off)
	std::string GetName_s() const { return Name.c_str(); }
#pragma optimize("",on)

	GETTER(int,Handle)

	fPoint TransPointLocal_p(fPoint Point);
	fPoint TransPointGlobal_p(fPoint Point);
	fPoint TransPointLocal(float x,float y);
	fPoint TransPointGlobal(float x,float y);

	void AddActor(ActorPtrX actor);
	void EraseActor(ActorPtrX actor);

	void ReleaseAllActor();
};

struct tag_Name		{};
struct tag_Handle	{};
typedef multi_index_container
	<
		Layer*,
		indexed_by
		<
			ordered_unique<tag<tag_Name>,	const_mem_fun<Layer,std::string,&Layer::GetName_s	> >,
			ordered_unique<tag<tag_Z>,		const_mem_fun<Layer,int,		&Layer::GetZ		> >,
			ordered_unique<tag<tag_Handle>,	const_mem_fun<Layer,int,		&Layer::GetHandle	> >
		>
	> LayerMap;

typedef LayerMap::index<tag_Name>::type		LayerMap_tag_Name;
typedef LayerMap::index<tag_Z>::type		LayerMap_tag_Z;
typedef LayerMap::index<tag_Handle>::type	LayerMap_tag_Handle;

class LayerManager
{
	LayerMap			Layers;
	std::queue<Layer*>	LayerPool;

public:
	Layer* GetPtr(int layer_handle);
	Layer* GetPtr(xtal::StringPtr layer_name);

	int		GetHandle(xtal::StringPtr layer_name);

	void	DrawAll();
	void	DrawLayer	(int layer_handle);

	void			SetArea		(int layer_handle,fRect area);
	void			SetZ		(int layer_handle,int z);
	fRectPtrX		GetArea		(int layer_handle);
	int				GetZ		(int layer_handle);
	xtal::StringPtr GetName		(int layer_handle);

	fPointPtrX	TransPointLocal_p(int layer_handle,fPoint Point);
	fPointPtrX	TransPointGlobal_p(int layer_handle,fPoint Point);
	fPointPtrX	TransPointLocal(int layer_handle,float x,float y);
	fPointPtrX	TransPointGlobal(int layer_handle,float x,float y);

	void	AddActor	(int layer_handle,ActorPtrX actor);
	void	EraseActor	(int layer_handle,ActorPtrX actor);

	//Layer	//Use LayerPool
	int		NewLayer(xtal::StringPtr layer_name,int z,fRect area);
	void	DeleteLayer(xtal::String layer_name);

	static void	bind(const xtal::ClassPtr it);
	SINGLETON_PATTERNX(LayerManager,LayerMngrPtr)
	void ReleaseAllLayer();
	void Release()
	{
		ReleaseAllLayer();
		//SAFE_DELETE(Inst);
	}
};

#include "ActorManager.h"
