#pragma once
#include "KVARCO.h"
#include "BaseScene.h"
#include "XtalHelper.h"
class CSceneManager;
typedef CSceneManager* SceneMngrPtr;

const DWORD MAX_SCENE_NUM =1024;
class CSceneManager
{
	//vector<xtal::AnyPtr> Scenes;
	xtal::AnyPtr Scenes[MAX_SCENE_NUM];
	DWORD RunningScenePos;
	bool ToDoCleanPrevScene;

	void CleanPrevScene_();	//CleanPrevScene()�̖{��
public:
	GETTER(DWORD,RunningScenePos);

	DWORD SetNextScene(xtal::AnyPtr scene);
	void StartNextScene();					//�ЂƂ������̃V�[���ɐi��
	void StartScene(int rela_pos);			//���ΓI

	xtal::AnyPtr GetSceneRlt(int by);		//���ΓI
	xtal::AnyPtr GetSceneAbs(DWORD pos);	//��ΓI

	void Run();
	void Draw();

	void CleanPrevScene() { ToDoCleanPrevScene=true;	}

	static void bind(const xtal::ClassPtr it); 
	//singleton pattern
	//�R���X�g���N�^�������Ȃ��Ă͂Ȃ�Ȃ������̂Ń}�N���͎g�킸��
private:
	friend xtal::SmartPtr<CSceneManager> xtal::xnew();
	friend class xtal::SmartPtr<CSceneManager>;
	friend struct xtal::XNew<CSceneManager>;
	friend struct xtal::XXNew<CSceneManager,3>;
	CSceneManager();
	//static SceneMngrPtr Inst;
public:
	static SceneMngrPtr GetInst();

	void ReleaseAllScene();
	void Release()
	{
		ReleaseAllScene();
//		SAFE_DELETE(Inst);
	}
};