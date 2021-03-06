﻿#pragma once
#include "KVARCO.h"
#include "BaseScene.h"
#include "XtalHelper.h"
class SceneManager;
typedef SceneManager* SceneMngrPtr;

const DWORD MAX_SCENE_NUM =1024;
class SceneManager
{
	//vector<xtal::AnyPtr> Scenes;
	xtal::AnyPtr Scenes[MAX_SCENE_NUM];
	DWORD RunningScenePos;
	bool ToDoCleanPrevScene;

	void CleanPrevScene_();	//CleanPrevScene()の本体
public:
	GETTER(DWORD,RunningScenePos);

	DWORD SetNextScene(xtal::AnyPtr scene);
	void StartNextScene();					//ひとつだけ次のシーンに進む
	void StartScene(int rela_pos);			//相対的

	xtal::AnyPtr GetSceneRlt(int by);		//相対的
	xtal::AnyPtr GetSceneAbs(DWORD pos);	//絶対的

	void Run();
	void Draw();

	void CleanPrevScene() { ToDoCleanPrevScene=true;	}

	static void bind(xtal::ClassPtr it); 
	//singleton pattern
	//コンストラクタを書かなくてはならなかったのでマクロは使わずに
private:
	FRIENDS_FOR_XTAL(SceneManager)
	SceneManager();
public:
	static SceneMngrPtr GetInst();

	void ReleaseAllScene();
	void Release()
	{
		ReleaseAllScene();
//		SAFE_DELETE(Inst);
	}
};