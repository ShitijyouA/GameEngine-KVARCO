#pragma once
#include "BaseActor.h"
#include "Layer2.h"
#include "ActorList.h"
#include "GeoStruct.h"

class CActorManager;
//typedef xtal::SmartPtr<CActorManager> ActorMngrPtr;
typedef CActorManager* ActorMngrPtr;
typedef boost::unordered_map<string,xtal::AnyPtr>	ActorMap;

class CActorManager
{
	ActorList	AllActors;
	ActorMap	ActorsMap;
public:
	//ActorManage
	void AddActor(xtal::AnyPtr actor,bool debug=false);
	void RunAll(bool debug=false,bool gc=false);
	xtal::ArrayPtr GetByType(DWORD type);
	xtal::ArrayPtr GetAllActor();

	//ItemBox
	void AddItem(xtal::StringPtr name,xtal::AnyPtr actor);
	xtal::AnyPtr GetItem(xtal::StringPtr name);
	void DeleteItem(xtal::StringPtr name);
	void CleanUpItemBox();

	static void bind(xtal::ClassPtr it);

	SINGLETON_PATTERNX(CActorManager,ActorMngrPtr)
	void ReleaseAllActor();
	void Release()
	{
		ReleaseAllActor();
		//SAFE_DELETE(Inst);
	}
};