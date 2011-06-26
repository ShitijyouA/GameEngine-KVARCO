#include "pch.h"
#include "ActorManager.h"

void ActorManager::AddActor(xtal::AnyPtr actor,bool debug)
{
	if(!xtal::can_cast<BaseActor>(actor)) return;

	AllActors.push_back(actor);
	BaseActorPtrX bactor=xtal::unchecked_ptr_cast<BaseActor>(actor);
	LayerManager::GetInst()->AddActor(LayerManager::GetInst()->GetHandle(bactor->LayerName),actor);
}

void ActorManager::RunAll(bool debug,bool gc)
{
	ActorList_tag_Type& list=AllActors.get<tag_Type>();
	ActorList_tag_Type::iterator i=list.begin();
	DWORD size=list.size();
	while(i!=list.end())
	{
		//キャスト
		BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>(*i);	//AddActorで確認済み

		if(!xtal::is_null(it->Run)) XtalHelper::call(it->Run);

		if(xtal::is_null(it->Run) || !it->Run->is_alive() || it->IsDead())
		{
			if(debug)
			{
				char buf[200]; sprintf_s(buf,"!Dead:ID=%d ActorNum=%d!\n",it->ID,list.size());
				kvarco::DebugOut(buf);
			}
			
			//削除
			it->Run->send(Xid(halt)); xtal::vmachine()->catch_except();
			i=list.erase(i);
			LayerManager::GetInst()->EraseActor(LayerManager::GetInst()->GetHandle(it->LayerName),it);
			it->Run=xtal::null;	//Actorの息の根を止める

			//一応GCかけておく
			if(gc)	xtal::gc();

			continue;
		}

		if(i!=list.end()) i++;
	}
}

xtal::ArrayPtr ActorManager::GetByType(DWORD type)
{
	ActorList_tag_Type& list=AllActors.get<tag_Type>();
	typedef std::pair<ActorList_tag_Type::iterator,ActorList_tag_Type::iterator> IteRangeType;
	IteRangeType equal=list.equal_range(type);

	ActorList_tag_Type::iterator i;
	xtal::ArrayPtr ar=xtal::xnew<xtal::Array>();
	for(i=equal.first; i!=equal.second; i++)
	{
		ar->push_back(*i);
	}
	return ar;
}

xtal::ArrayPtr ActorManager::GetAllActor()
{
	xtal::ArrayPtr ar=xtal::xnew<xtal::Array>();

	ActorList_tag_Seq& list=AllActors.get<tag_Seq>();
	BOOST_FOREACH(const xtal::AnyPtr& i,list)
	{
		BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>(i);
		if(it->Run->is_alive() && !it->IsDead()) ar->push_back(i);
	}

	return ar;
}

void ActorManager::AddItem(xtal::StringPtr name,xtal::AnyPtr actor)
{
	if(!xtal::can_cast<BaseActor>(actor)) return;
	ActorsMap.insert(std::make_pair(name->c_str(),actor));
}

xtal::AnyPtr ActorManager::GetItem(xtal::StringPtr name)
{
	ActorMap::iterator i=ActorsMap.find(name->c_str());
	if(i==ActorsMap.end()) return xtal::null;

	BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>((*i).second);
	if(it->IsDead() || is_null(it->Run) || !it->Run->is_alive())
	{
		ActorsMap.erase(i);	//FIX ME
		return xtal::null;
	}

	return (*i).second;
}

void ActorManager::DeleteItem(xtal::StringPtr name)
{
	ActorMap::iterator i=ActorsMap.find(name->c_str());
	if(i==ActorsMap.end()) return;
	BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>((*i).second);
	ActorsMap.erase(i);
}

void ActorManager::CleanUpItemBox()
{
	ActorMap::iterator i=ActorsMap.begin();
	while(i!=ActorsMap.end())
	{
		BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>((*i).second);	//AddActorで確認済み
		if(xtal::is_null(it->Run) || !it->Run->is_alive() || it->IsDead())
		{
			i=ActorsMap.erase(i);
			continue;
		}
		i++;
	}
}

//ActorMngrPtr ActorManager::Inst=xtal::null;
//ActorMngrPtr ActorManager::Inst=NULL;
ActorMngrPtr ActorManager::GetInst()
{
	//if(xtal::is_null(Inst)) Inst=xtal::xnew<ActorManager>();
	//if(Inst==NULL) ActorManager::Inst=new ActorManager;
	static ActorManager Inst;
	return &Inst;
}

void ActorManager::ReleaseAllActor()
{
	//AllActorsから全て削除
	{
		ActorList_tag_Type& list=AllActors.get<tag_Type>();
		ActorList_tag_Type::iterator i=list.begin();
		while(i!=list.end())
		{
			//キャスト
			BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>(*i);	//AddActorで確認済み

			//削除
			i=list.erase(i);
			LayerManager::GetInst()->EraseActor(LayerManager::GetInst()->GetHandle(it->LayerName),it);
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

			BaseActorPtrX	it=xtal::unchecked_ptr_cast<BaseActor>((*i).second);	//AddActorで確認済み
			it->Run=xtal::null;
		}
	}
}

void ActorManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(ActorManager);

	it->def(Xid(AddActor),xtal::method(&AddActor)->param(2,Xid(debug),false));
	Xdef_method(RunAll);
	Xdef_method(GetByType);
	Xdef_method(GetAllActor);
	Xdef_method(AddItem);
	Xdef_method(GetItem);
	Xdef_method(DeleteItem);
	Xdef_method(CleanUpItemBox);
}