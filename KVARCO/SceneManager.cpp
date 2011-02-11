#include "pch.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	RunningScenePos		=0;
	ToDoCleanPrevScene	=false;
}

DWORD CSceneManager::SetNextScene(xtal::AnyPtr scene)
{
	if(RunningScenePos>=MAX_SCENE_NUM) return 0;

	Scenes[RunningScenePos+1]=scene;
	return RunningScenePos+1;
}

void CSceneManager::StartNextScene()
{
	if(RunningScenePos+1<MAX_SCENE_NUM) RunningScenePos++;
}

void CSceneManager::StartScene(int rela_pos)
{
	if(RunningScenePos+rela_pos>0 && RunningScenePos+rela_pos<MAX_SCENE_NUM)
		RunningScenePos+=rela_pos;
}

xtal::AnyPtr CSceneManager::GetSceneRlt(int by)
{
	if(RunningScenePos+by>0 && RunningScenePos+by<MAX_SCENE_NUM)
		return Scenes[RunningScenePos+by];
	return xtal::null;
}

xtal::AnyPtr CSceneManager::GetSceneAbs(DWORD pos)
{
	return Scenes[pos];
}

void CSceneManager::CleanPrevScene_()
{
	Scenes[0]=Scenes[RunningScenePos];	//‚¸‚ç‚·
	RunningScenePos=0;
	for(DWORD i=1; i<MAX_SCENE_NUM; i++)
	{
		BaseScenePtr it=xtal::ptr_cast<CBaseScene>(Scenes[RunningScenePos]);
		if(!xtal::is_null(it))
		{
			it->Run->send(Xid(halt)); xtal::vmachine()->catch_except();
			it->Run	=xtal::null;
		}

		Scenes[i]=xtal::null;
	}
}

void CSceneManager::Run()
{
	if(ToDoCleanPrevScene)
	{
		CleanPrevScene_();
		ToDoCleanPrevScene=false;
	}

	if(
		Scenes[RunningScenePos]!=xtal::null &&
		Scenes[RunningScenePos]!=xtal::undefined)
	{
		BaseScenePtr it=xtal::ptr_cast<CBaseScene>(Scenes[RunningScenePos]);
		XtalHelper::call(it->Run);
	}
}

void CSceneManager::Draw()
{
	if(
		Scenes[RunningScenePos]!=xtal::null &&
		Scenes[RunningScenePos]!=xtal::undefined)
	{
		KVARCO::SetDrawArea_default();
		Scenes[RunningScenePos]->send(Xid(Draw)); xtal::vmachine()->catch_except();
	}
}

//SceneMngrPtr CSceneManager::Inst=NULL;
SceneMngrPtr CSceneManager::GetInst()
{
	//if(Inst==NULL) Inst=new CSceneManager;
	static CSceneManager Inst;
	return &Inst;
}

void CSceneManager::ReleaseAllScene()
{
	for(DWORD i=0; i<MAX_SCENE_NUM; i++)
	{
		BaseScenePtr it=xtal::ptr_cast<CBaseScene>(Scenes[i]);
		if(!xtal::is_null(it) && !xtal::is_null(it->Run))
		{
			it->Run->send(Xid(halt)); xtal::vmachine()->catch_except();
			it->Run	=xtal::null;
		}

		Scenes[i]=xtal::null;
	}
}

void CSceneManager::bind(const xtal::ClassPtr it)
{
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,GetRunningScenePos);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,SetNextScene);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,StartNextScene);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,StartScene);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,GetSceneRlt);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,GetSceneAbs);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,CleanPrevScene);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,Run);
	BIND_XTAL_CLASSFUN_DEFNAME_IT(CSceneManager,Draw);
}