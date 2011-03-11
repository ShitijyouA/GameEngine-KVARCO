#include "pch.h"
#include "Layer2.h"

CLayer::CLayer()
{
	char buf[256];
	sprintf_s(buf,"Layer%d",KVARCO::MakeHandle());
	Init(buf);
}

CLayer::CLayer(xtal::String name)
{
	Init(name);
}

void CLayer::Init(xtal::String name)
{
	Z		=0;
	Name	=name;
	Handle	=KVARCO::MakeHandle();
}

void CLayer::DrawThis()
{
	KVARCO::SetDrawArea(Area);

	//Z値が小さいものから描画
	ActorList_tag_Z& list=Actors.get<tag_Z>();
	BOOST_FOREACH(const xtal::AnyPtr& i,list)
	{
		XtalHelper::send(i,Xid(Draw));
	}

	KVARCO::SetDrawArea_default();
}

dPoint CLayer::TransPointLocal_p(dPoint Point)
{
	dPoint res;
	res.x=Point.x-Area.left;
	res.y=Point.y-Area.top;
	return res;
}

dPoint CLayer::TransPointGlobal_p(dPoint Point)
{
	dPoint res;
	res.x=Point.x+Area.left;
	res.y=Point.y+Area.top;
	return res;
}
dPoint CLayer::TransPointLocal(float x,float y)
{
	dPoint tmp;
	tmp.x=x;
	tmp.y=y;
	return TransPointLocal_p(tmp);
}
dPoint	CLayer::TransPointGlobal(float x,float y)
{
	dPoint tmp;
	tmp.x=x;
	tmp.y=y;
	return TransPointGlobal_p(tmp);
}

void CLayer::AddActor(ActorPtrX actor)
{
	//BaseActorを継承してなかったら弾く
	if(!xtal::can_cast<CBaseActor>(actor))	return;
	Actors.push_back(actor);
}

void CLayer::EraseActor(ActorPtrX actor)
{
	DWORD id=KVARCO::GetID_BaseActor(actor);
	ActorList_tag_ID& list=Actors.get<tag_ID>();
	ActorList_tag_ID::iterator i=list.find(id);
	if(i==list.end()) return;
	list.erase(i);
}

void CLayer::ReleaseAllActor()
{
	ActorList::iterator i=Actors.begin();
	while(i!=Actors.end())
	{
		BaseActorPtrX	it=xtal::unchecked_ptr_cast<CBaseActor>(*i);
		it->Run=xtal::null;
		i=Actors.erase(i);
	}
}

//CLayerManager
CLayer* CLayerManager::GetPtr(xtal::StringPtr layer_name)
{
	if(xtal::is_undefined(layer_name) || xtal::is_null(layer_name))	return NULL;

	LayerMap_tag_Name& layer_name_list=Layers.get<tag_Name>();
	LayerMap::iterator i=layer_name_list.find(layer_name->c_str());
	if(i==layer_name_list.end())	return NULL;
	return (*i);
}

CLayer* CLayerManager::GetPtr(int layer_handle)
{
	LayerMap_tag_Handle& layer_handle_list=Layers.get<tag_Handle>();
	LayerMap_tag_Handle::iterator i=layer_handle_list.find(layer_handle);
	if(i==layer_handle_list.end())	return NULL;
	return (*i);
}

int	CLayerManager::GetHandle(xtal::StringPtr layer_name)
{
	if(xtal::is_undefined(layer_name) || xtal::is_null(layer_name))	return 0;

	CLayer* layer=GetPtr(layer_name);
	if(layer!=NULL) return layer->GetHandle();
	return 0;
}

void CLayerManager::DrawAll()
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

void CLayerManager::DrawLayer(int layer_handle)
{
	GetPtr(layer_handle)->DrawThis();
}

//ここからはCLayerのメンバ関数のアダプター

template<typename T1,typename void(CLayer::*Func)(const T1)>
inline void SetByLayerHandle(int layer_handle,const T1& val)
{
	CLayer* ptr=CLayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) (ptr->*Func)(val);
}

template<typename T1,typename T1(CLayer::*Func)() const>
inline T1 GetByLayerHandle_const(int layer_handle)
{
	CLayer* ptr=CLayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return (ptr->*Func)();
	return T1();
}

template<typename T1,typename T1(CLayer::*Func)()>
inline T1 GetByLayerHandle(int layer_handle)
{
	CLayer* ptr=CLayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return (ptr->*Func)();
	return T1();
}

template<typename T1,typename P2,typename T1(CLayer::*Func)(P2)>
inline T1 ByLayerHandle1(int layer_handle,P2& val0)
{
	CLayer* ptr=CLayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return (ptr->*Func)(val0);
	return T1();
}

//そのインスタンス
void	CLayerManager::SetArea(int layer_handle,dRect area)
{	SetByLayerHandle<dRect,&CLayer::SetArea>(layer_handle,area);	}
void	CLayerManager::SetZ(int layer_handle,int z)
{	SetByLayerHandle<int,&CLayer::SetZ>		(layer_handle,z);		}

dRectPtrX	CLayerManager::GetArea(int layer_handle)
{	return xtal::xnew<dRect>(GetByLayerHandle_const<dRect,&CLayer::GetArea>(layer_handle));		}
int		CLayerManager::GetZ(int layer_handle)
{	return GetByLayerHandle_const<int,&CLayer::GetZ>(layer_handle);				}
xtal::StringPtr CLayerManager::GetName(int layer_handle)
{	return xtal::xnew<xtal::String>(GetByLayerHandle_const<xtal::String,&CLayer::GetName>(layer_handle));	}

dPointPtrX	CLayerManager::TransPointLocal_p(int layer_handle,dPoint Point)
{	return xtal::xnew<dPoint>(ByLayerHandle1<dPoint,dPoint,&CLayer::TransPointLocal_p>(layer_handle,Point));	}
dPointPtrX	CLayerManager::TransPointGlobal_p(int layer_handle,dPoint Point)
{	return xtal::xnew<dPoint>(ByLayerHandle1<dPoint,dPoint,&CLayer::TransPointGlobal_p>(layer_handle,Point));	}
dPointPtrX	CLayerManager::TransPointLocal(int layer_handle,float x,float y)
{
	CLayer* ptr=CLayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return xtal::xnew<dPoint>(ptr->TransPointLocal(x,y));
	return dPointPtrX();
}

dPointPtrX	CLayerManager::TransPointGlobal(int layer_handle,float x,float y)
{
	CLayer* ptr=CLayerManager::GetInst()->GetPtr(layer_handle);
	if(ptr!=NULL) return xtal::xnew<dPoint>(ptr->TransPointGlobal(x,y));
	return dPointPtrX();
}

void	CLayerManager::AddActor(int layer_handle,ActorPtrX actor)
{	 SetByLayerHandle<ActorPtrX,&CLayer::AddActor>(layer_handle,actor);		}
void	CLayerManager::EraseActor(int layer_handle,ActorPtrX actor)
{	 SetByLayerHandle<ActorPtrX,&CLayer::EraseActor>(layer_handle,actor);	}

//Layerの操作
int CLayerManager::NewLayer(xtal::StringPtr layer_name,int z,dRect area)
{
	if(xtal::is_undefined(layer_name) || xtal::is_null(layer_name))	return 0;

	CLayer* layer;
	if(!Layers.empty())
	{
		layer=GetPtr(layer_name);
		if(layer!=NULL) return layer->GetHandle();
	}

	if(LayerPool.empty()) layer=new CLayer(layer_name->c_str());
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

void CLayerManager::DeleteLayer(xtal::String layer_name)
{
	LayerMap_tag_Name& layer_name_list=Layers.get<tag_Name>();
	LayerMap::iterator i=layer_name_list.find(string(layer_name.c_str()));

	LayerPool.push(*i);
	Layers.erase(i);
}

void CLayerManager::ReleaseAllLayer()
{
	LayerMap::iterator i=Layers.begin();
	while(i!=Layers.end())
	{
		(*i)->ReleaseAllActor();
		i=Layers.erase(i);
	}

	for(DWORD ii=0; ii<LayerPool.size(); ii++)	LayerPool.pop();
}

//LayerMngrPtr CLayerManager::Inst=NULL;
LayerMngrPtr CLayerManager::GetInst()
{
	//if(Inst==NULL)	CLayerManager::Inst=new CLayerManager;
	static CLayerManager Inst;
	return &Inst;
}

void CLayerManager::bind(const xtal::ClassPtr it)
{
	USE_XDEFZ(CLayerManager);

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