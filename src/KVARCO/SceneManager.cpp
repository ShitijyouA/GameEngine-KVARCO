#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	RunningScenePos		=0;
	ToDoCleanPrevScene	=false;
}

DWORD SceneManager::SetNextScene(xtal::AnyPtr scene)
{
	if(RunningScenePos>=MAX_SCENE_NUM) return 0;

	Scenes[RunningScenePos+1]=scene;
	return RunningScenePos+1;
}

void SceneManager::StartNextScene()
{
	if(RunningScenePos+1<MAX_SCENE_NUM) RunningScenePos++;

	xtal::full_gc();
}

void SceneManager::StartScene(int rela_pos)
{
	if(RunningScenePos+rela_pos>0 && RunningScenePos+rela_pos<MAX_SCENE_NUM)
		RunningScenePos+=rela_pos;

	xtal::full_gc();
}

xtal::AnyPtr SceneManager::GetSceneRlt(int by)
{
	if(RunningScenePos+by>0 && RunningScenePos+by<MAX_SCENE_NUM)
		return Scenes[RunningScenePos+by];
	return xtal::null;
}

xtal::AnyPtr SceneManager::GetSceneAbs(DWORD pos)
{
	return Scenes[pos];
}

void SceneManager::CleanPrevScene_()
{
	Scenes[0]=Scenes[RunningScenePos];	//ずらす
	RunningScenePos=0;
	for(DWORD i=1; i<MAX_SCENE_NUM; i++)
	{
		BaseScenePtrX it=xtal::ptr_cast<BaseScene>(Scenes[RunningScenePos]);
		if(!xtal::is_null(it))
		{
			it->Run->send(Xid(halt)); xtal::vmachine()->catch_except();
			it->Run	=xtal::null;
		}

		Scenes[i]=xtal::null;
	}

	xtal::full_gc();
}

void SceneManager::Run()
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
		BaseScenePtrX it=xtal::ptr_cast<BaseScene>(Scenes[RunningScenePos]);
		XtalHelper::call(it->Run);
	}
}

void SceneManager::Draw()
{
	if(
		Scenes[RunningScenePos]!=xtal::null &&
		Scenes[RunningScenePos]!=xtal::undefined)
	{
		kvarco::SetDrawArea_default();
		Scenes[RunningScenePos]->send(Xid(Draw)); xtal::vmachine()->catch_except();
	}
}

SceneMngrPtr SceneManager::GetInst()
{
	static SceneManager Inst;
	return &Inst;
}

void SceneManager::ReleaseAllScene()
{
	for(DWORD i=0; i<MAX_SCENE_NUM; i++)
	{
		BaseScenePtrX it=xtal::ptr_cast<BaseScene>(Scenes[i]);
		if(!xtal::is_null(it) && !xtal::is_null(it->Run))
		{
			it->Run->send(Xid(halt)); xtal::vmachine()->catch_except();
			it->Run	=xtal::null;
		}

		Scenes[i]=xtal::null;
	}
}

void SceneManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(SceneManager);

	Xdef_method(GetRunningScenePos);
	Xdef_method(SetNextScene);
	Xdef_method(StartNextScene);
	Xdef_method(StartScene);
	Xdef_method(GetSceneRlt);
	Xdef_method(GetSceneAbs);
	Xdef_method(CleanPrevScene);
	Xdef_method(Run);
	Xdef_method(Draw);
}