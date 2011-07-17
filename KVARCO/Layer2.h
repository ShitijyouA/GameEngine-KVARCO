#pragma once

class Layer;
class LayerManager;

typedef xtal::SmartPtr<Layer>	LayerPtrX;
typedef LayerManager*			LayerMngrPtr;

#include "BaseActor.h"
#include "ActorList.h"
#include "Tag.h"
#include "Utillity.h"

class Layer
{
	fRect		Area;
	ActorList	Actors;

	int Z;
	xtal::String Name;
	int Handle;

public:
	Layer(xtal::StringPtr name);
	Layer();
	void Init(xtal::StringPtr name);	//Call from LayerManager only.

	void DrawThis();
	SET_GET(fRect,Area)

	SET_GET(int,Z)
	GETTER(xtal::String,Name)

#pragma optimize("",off)
	std::string GetName_s() const { return Name.c_str(); }
#pragma optimize("",on)

	GETTER(int,Handle)

	fPoint TransPointLocal_p(fPointPtrX Point);
	fPoint TransPointGlobal_p(fPointPtrX Point);
	fPoint TransPointLocal(float x,float y);
	fPoint TransPointGlobal(float x,float y);

	void AddActor(BaseActor::BaseActorPtrX actor);
	void EraseActor(BaseActor::BaseActorPtrX actor);

	void ReleaseAllActor();
};

typedef multi_index_container
	<
		Layer*,
		indexed_by
		<
			ordered_unique<kvarco::tag::detail::Name,	const_mem_fun<Layer,std::string,&Layer::GetName_s	> >,
			ordered_unique<kvarco::tag::detail::Z_depth,const_mem_fun<Layer,int,		&Layer::GetZ		> >,
			ordered_unique<kvarco::tag::detail::Handle,	const_mem_fun<Layer,int,		&Layer::GetHandle	> >
		>
	> LayerMap;

typedef LayerMap::index<kvarco::tag::Name>::type	LayerMap_tag_Name;
typedef LayerMap::index<kvarco::tag::Z_depth>::type	LayerMap_tag_Z;
typedef LayerMap::index<kvarco::tag::Handle>::type	LayerMap_tag_Handle;

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

	void			SetArea		(int layer_handle,fRectPtrX area);
	void			SetZ		(int layer_handle,int z);
	fRectPtrX		GetArea		(int layer_handle);
	int				GetZ		(int layer_handle);
	xtal::StringPtr GetName		(int layer_handle);

	fPointPtrX TransPointLocal_p(int layer_handle,fPointPtrX Point);
	fPointPtrX TransPointGlobal_p(int layer_handle,fPointPtrX Point);
	fPointPtrX TransPointLocal(int layer_handle,float x,float y);
	fPointPtrX TransPointGlobal(int layer_handle,float x,float y);

	void AddActor	(int layer_handle,BaseActor::BaseActorPtrX actor);
	void EraseActor	(int layer_handle,BaseActor::BaseActorPtrX actor);

	//Layer	//Use LayerPool
	int NewLayer(xtal::StringPtr layer_name,int z,fRectPtrX area);
	void DeleteLayer(xtal::StringPtr layer_name);

	//utility
	bool IsInLayer(const std::string& layer_name,const fRect& area,float margin=0.0f);
	bool IsInLayerX(const xtal::String& layer_name,const fRectPtrX& area,float margin=0.0f);

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
