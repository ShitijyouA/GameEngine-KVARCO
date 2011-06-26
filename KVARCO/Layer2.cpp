#include "pch.h"
#include "Layer2.h"

Layer::Layer()
{
	char buf[256];
	sprintf_s(buf,"Layer%d",kvarco::MakeHandle());
	Init(buf);
}

Layer::Layer(xtal::String name)
{
	Init(name);
}

void Layer::Init(xtal::String name)
{
	Z		=0;
	Name	=name;
	Handle	=kvarco::MakeHandle();
}

void Layer::DrawThis()
{
	kvarco::SetDrawArea(Area);

	//Z値が小さいものから描画
	ActorList_tag_Z& list=Actors.get<tag_Z>();
	BOOST_FOREACH(const xtal::AnyPtr& i,list)
	{
		XtalHelper::send(i,Xid(Draw));
	}

	kvarco::SetDrawArea_default();
}

fPoint Layer::TransPointLocal_p(fPoint Point)
{
	fPoint res;
	res.x=Point.x-Area.left;
	res.y=Point.y-Area.top;
	return res;
}

fPoint Layer::TransPointGlobal_p(fPoint Point)
{
	fPoint res;
	res.x=Point.x+Area.left;
	res.y=Point.y+Area.top;
	return res;
}
fPoint Layer::TransPointLocal(float x,float y)
{
	fPoint tmp;
	tmp.x=x;
	tmp.y=y;
	return TransPointLocal_p(tmp);
}
fPoint	Layer::TransPointGlobal(float x,float y)
{
	fPoint tmp;
	tmp.x=x;
	tmp.y=y;
	return TransPointGlobal_p(tmp);
}

void Layer::AddActor(ActorPtrX actor)
{
	//BaseActorを継承してなかったら弾く
	if(!xtal::can_cast<BaseActor>(actor))	return;
	Actors.push_back(actor);
}

void Layer::EraseActor(ActorPtrX actor)
{
	DWORD id=kvarco::GetID_BaseActor(actor);
	ActorList_tag_ID& list=Actors.get<tag_ID>();
	ActorList_tag_ID::iterator i=list.find(id);
	if(i==list.end()) return;
	list.erase(i);
}

void Layer::ReleaseAllActor()
{
	ActorList::iterator i=Actors.begin();
	while(i!=Actors.end())
	{
		BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>(*i);
		it->Run=xtal::null;
		i=Actors.erase(i);
	}
}

//LayerManager
Layer* LayerManager::GetPtr(xtal::StringPtr layer_name)
{
	if(xtal::is_undefined(layer_name) || xtal::is_null(layer_name))	return NULL;

	LayerMap_tag_Name& layer_name_list=Layers.get<tag_Name>();
	LayerMap::iterator i=layer_name_list.find(layer_name->c_str());
	if(i==layer_name_list.end())	return NULL;
	return (*i);
}

Layer* LayerManager::GetPtr(int layer_handle)
{
	LayerMap_tag_Handle& layer_handle_list=Layers.get<tag_Handle>();
	LayerMap_tag_Handle::iterator i=layer_handle_list.find(layer_handle);
	if(i==layer_handle_list.end())	return NULL;
	return (*i);
}

int	LayerManager::GetHandle(xtal::StringPtr layer_name)
{
	if(xtal::is_undefined(layer_name) || xtal::is_null(layer_name))	return 0;

	Layer* layer=GetPtr(layer_name);
	if(layer!=NULL) return layer->GetHandle();
	return 0;
}

void LayerManager::DrawAll()
{
	LayerMap_tag_Z& layer_z_list=Layers.get<tag_Z>();

#if 1		//beginからendへ
	LayerMap_tag_Z::iterator i;
	for(i=layer_z_list.begin(); i!=layer_z_list.end(); i++)
#else
	LayerMap_tag_Z::reverse_iterator i;
	for(i=layer_z_list.rbegin(); i!=layer_z_list.end(); i++)
#endif
	{
		(*i)->DrawThis();
	}
}

void LayerManager::DrawLayer(int layer_handle)
{
	GetPtr(layer_handle)->DrawThis();
}

//ここからはLayerのメンバ関数のアダプター

template<typename T1,typename void(Layer::*Func)(const T1)>
inline void SetByLayerHandle(int layer_handle,const T1& val)
{
	Layer* ptr=LayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) (ptr->*Func)(val);
}

template<typename T1,typename T1(Layer::*Func)() const>
inline T1 GetByLayerHandle_const(int layer_handle)
{
	Layer* ptr=LayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return (ptr->*Func)();
	return T1();
}

template<typename T1,typename T1(Layer::*Func)()>
inline T1 GetByLayerHandle(int layer_handle)
{
	Layer* ptr=LayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return (ptr->*Func)();
	return T1();
}

template<typename T1,typename P2,typename T1(Layer::*Func)(P2)>
inline T1 ByLayerHandle1(int layer_handle,P2& val0)
{
	Layer* ptr=LayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return (ptr->*Func)(val0);
	return T1();
}

//そのインスタンス
void	LayerManager::SetArea(int layer_handle,fRect area)
{	SetByLayerHandle<fRect,&Layer::SetArea>(layer_handle,area);	}
void	LayerManager::SetZ(int layer_handle,int z)
{	SetByLayerHandle<int,&Layer::SetZ>		(layer_handle,z);		}

fRectPtrX	LayerManager::GetArea(int layer_handle)
{	return xtal::xnew<fRect>(GetByLayerHandle_const<fRect,&Layer::GetArea>(layer_handle));		}
int		LayerManager::GetZ(int layer_handle)
{	return GetByLayerHandle_const<int,&Layer::GetZ>(layer_handle);				}
xtal::StringPtr LayerManager::GetName(int layer_handle)
{	return xtal::xnew<xtal::String>(GetByLayerHandle_const<xtal::String,&Layer::GetName>(layer_handle));	}

fPointPtrX	LayerManager::TransPointLocal_p(int layer_handle,fPoint Point)
{	return xtal::xnew<fPoint>(ByLayerHandle1<fPoint,fPoint,&Layer::TransPointLocal_p>(layer_handle,Point));	}
fPointPtrX	LayerManager::TransPointGlobal_p(int layer_handle,fPoint Point)
{	return xtal::xnew<fPoint>(ByLayerHandle1<fPoint,fPoint,&Layer::TransPointGlobal_p>(layer_handle,Point));	}
fPointPtrX	LayerManager::TransPointLocal(int layer_handle,float x,float y)
{
	Layer* ptr=LayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return xtal::xnew<fPoint>(ptr->TransPointLocal(x,y));
	return fPointPtrX();
}

fPointPtrX	LayerManager::TransPointGlobal(int layer_handle,float x,float y)
{
	Layer* ptr=LayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return xtal::xnew<fPoint>(ptr->TransPointGlobal(x,y));
	return fPointPtrX();
}

void	LayerManager::AddActor(int layer_handle,ActorPtrX actor)
{	 SetByLayerHandle<ActorPtrX,&Layer::AddActor>(layer_handle,actor);		}
void	LayerManager::EraseActor(int layer_handle,ActorPtrX actor)
{	 SetByLayerHandle<ActorPtrX,&Layer::EraseActor>(layer_handle,actor);	}

//Layerの操作
int LayerManager::NewLayer(xtal::StringPtr layer_name,int z,fRect area)
{
	if(xtal::is_undefined(layer_name) || xtal::is_null(layer_name))	return 0;

	Layer* layer;
	if(!Layers.empty())
	{
		layer=GetPtr(layer_name);
		if(layer!=NULL) return layer->GetHandle();
	}

	if(LayerPool.empty()) layer=new Layer(layer_name->c_str());
	else
	{
		layer=LayerPool.front();
		LayerPool.pop();
		layer->Init(layer_name->c_str());
	}

	layer->SetZ(z);
	layer->SetArea(area);
	Layers.insert(layer);

	return layer->GetHandle();
}

void LayerManager::DeleteLayer(xtal::String layer_name)
{
	LayerMap_tag_Name& layer_name_list=Layers.get<tag_Name>();
	LayerMap::iterator i=layer_name_list.find(std::string(layer_name.c_str()));

	LayerPool.push(*i);
	Layers.erase(i);
}

void LayerManager::ReleaseAllLayer()
{
	LayerMap::iterator i=Layers.begin();
	while(i!=Layers.end())
	{
		(*i)->ReleaseAllActor();
		i=Layers.erase(i);
	}

	for(DWORD ii=0; ii<LayerPool.size(); ii++)	LayerPool.pop();
}

//LayerMngrPtr LayerManager::Inst=NULL;
LayerMngrPtr LayerManager::GetInst()
{
	//if(Inst==NULL)	LayerManager::Inst=new LayerManager;
	static LayerManager Inst;
	return &Inst;
}

void LayerManager::bind(const xtal::ClassPtr it)
{
	USE_XDEFZ(LayerManager);

	Xdef_method(DrawAll);
	Xdef_method(DrawLayer);
	Xdef_method(SetArea);
	Xdef_method(SetZ);
	Xdef_method(GetArea);
	Xdef_method(GetZ);
	Xdef_method(GetName);
	Xdef_method(TransPointLocal_p);
	Xdef_method(TransPointGlobal_p);
	Xdef_method(TransPointLocal);
	Xdef_method(TransPointGlobal);
	Xdef_method(AddActor);
	Xdef_method(EraseActor);
	Xdef_method(NewLayer);
	Xdef_method(DeleteLayer);
	Xdef_method(GetHandle);
}