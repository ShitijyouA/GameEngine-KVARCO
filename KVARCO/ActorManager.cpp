#include "pch.h"
#include "ActorManager.h"

void CActorManager::AddActor(xtal::AnyPtr actor,bool debug)
{
	if(!xtal::can_cast<CBaseActor>(actor)) return;

	AllActors.push_back(actor);
	BaseActorPtr bactor=xtal::unchecked_ptr_cast<CBaseActor>(actor);
	CLayerManager::GetInst()->AddActor(CLayerManager::GetInst()->GetHandle(bactor->LayerName),actor);
}

void CActorManager::RunAll(bool debug,bool gc)
{
	ActorList_tag_Type& list=AllActors.get<tag_Type>();
	ActorList_tag_Type::iterator i=list.begin();
	DWORD size=list.size();
	while(i!=list.end())
	{
		//キャスト
		BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>(*i);	//AddActorで確認済み

		if(!xtal::is_null(it->Run)) XtalHelper::call(it->Run);

		if(xtal::is_null(it->Run) || !it->Run->is_alive() || it->IsDead())
		{
			if(debug)
			{
				char buf[200]; sprintf_s(buf,"!Dead:ID=%d ActorNum=%d!\n",it->ID,list.size());
				KVARCO::DebugOut(buf);
			}
			
			//削除
			it->Run->send(Xid(halt)); xtal::vmachine()->catch_except();
			i=list.erase(i);
			CLayerManager::GetInst()->EraseActor(CLayerManager::GetInst()->GetHandle(it->LayerName),it);
			it->Run=xtal::null;	//Actorの息の根を止める

			//一応GCかけておく
			if(gc)	xtal::gc();

			continue;
		}

		if(i!=list.end()) i++;
	}
}

xtal::ArrayPtr CActorManager::GetByType(DWORD type)
{
	ActorList_tag_Type& list=AllActors.get<tag_Type>();
	pair<ActorList_tag_Type::iterator,ActorList_tag_Type::iterator> equal=list.equal_range(type);

	ActorList_tag_Type::iterator i;
	xtal::ArrayPtr ar=xtal::xnew<xtal::Array>();
	for(i=equal.first; i!=equal.second; i++)
	{
		ar->push_back(*i);
	}
	return ar;
}

xtal::ArrayPtr CActorManager::GetAllActor()
{
	xtal::ArrayPtr ar=xtal::xnew<xtal::Array>();

	ActorList_tag_Seq& list=AllActors.get<tag_Seq>();
	BOOST_FOREACH(const xtal::AnyPtr& i,list)
	{
		BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>(i);
		if(it->Run->is_alive() && !it->IsDead()) ar->push_back(i);
	}

	return ar;
}

void CActorManager::AddItem(xtal::StringPtr name,xtal::AnyPtr actor)
{
	if(!xtal::can_cast<CBaseActor>(actor)) return;
	ActorsMap.insert(make_pair(name->c_str(),actor));
}

xtal::AnyPtr CActorManager::GetItem(xtal::StringPtr name)
{
	ActorMap::iterator i=ActorsMap.find(name->c_str());
	if(i==ActorsMap.end()) return xtal::null;

	BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>((*i).second);
	if(it->IsDead() || is_null(it->Run) || !it->Run->is_alive())
	{
		ActorsMap.erase(i);	//FIX ME
		return xtal::null;
	}

	return (*i).second;
}

void CActorManager::DeleteItem(xtal::StringPtr name)
{
	ActorMap::iterator i=ActorsMap.find(name->c_str());
	if(i==ActorsMap.end()) return;
	BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>((*i).second);
	ActorsMap.erase(i);
}

void CActorManager::CleanUpItemBox()
{
	ActorMap::iterator i=ActorsMap.begin();
	while(i!=ActorsMap.end())
	{
		BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>((*i).second);	//AddActorで確認済み
		if(xtal::is_null(it->Run) || !it->Run->is_alive() || it->IsDead())
		{
			i=ActorsMap.erase(i);
			continue;
		}
		i++;
	}
}

//ActorMngrPtr CActorManager::Inst=xtal::null;
//ActorMngrPtr CActorManager::Inst=NULL;
ActorMngrPtr CActorManager::GetInst()
{
	//if(xtal::is_null(Inst)) Inst=xtal::xnew<CActorManager>();
	//if(Inst==NULL) CActorManager::Inst=new CActorManager;
	static CActorManager Inst;
	return &Inst;
}

void CActorManager::ReleaseAllActor()
{
	//AllActorsから全て削除
	{
		ActorList_tag_Type& list=AllActors.get<tag_Type>();
		ActorList_tag_Type::iterator i=list.begin();
		while(i!=list.end())
		{
			//キャスト
			BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>(*i);	//AddActorで確認済み

			//削除
			i=list.erase(i);
			CLayerManager::GetInst()->EraseActor(CLayerManager::GetInst()->GetHandle(it->LayerName),it);
			it->Run=xtal::null;	//Actorの息の根を止める
		}
	}

	//ActorsMapから全て削除
	{
		CleanUpItemBox();
		ActorMap::iterator i=ActorsMap.begin();
		while(i!=ActorsMap.end())
		{
			i=ActorsMap.erase(i);

			BaseActorPtr	it=xtal::unchecked_ptr_cast<CBaseActor>((*i).second);	//AddActorで確認済み
			it->Run=xtal::null;
		}
	}
}

void CActorManager::bind(xtal::ClassPtr it)
{
	it->def(Xid(AddActor),
		xtal::method(&CActorManager::AddActor)->param(2,Xid(debug),false));

	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,RunAll);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,GetByType);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,GetAllActor);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,AddItem);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,GetItem);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,DeleteItem);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CActorManager,CleanUpItemBox);
}