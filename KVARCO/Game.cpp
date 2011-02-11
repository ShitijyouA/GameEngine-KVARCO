#include "pch.h"
#include "BaseActor.h"
#include "XtalHelper.h"
#include "LoadingThread.h"
#include "Layer2.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Game.h"
shared_ptr<CGame>	Game;

unordered_map<string,GR_INFO>	KVARCO::ImageNameList;
CLoadingThread*					KVARCO::LoadingThread;

//�X�N���v�g���[�h�Ȃǂ̂݁B�摜���[�h�Ȃǂ͍s��Ȃ�
namespace KVARCO
{
	void Boot(string IniFile)
	{
		OutputLog("*********************************************************");
		OutputLog("    -KVARCO ver1.00- %s","2011-02-11");
		OutputLog("���샍�O");
		OutputLog("*********************************************************\n");

		OutputLog("----------------[Boot]----------------\n");

		//���������[�N�����o
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		XtalHelper::Init();
		OutputLog("Xtal��������");

		CGameBootSetting params=InitFromIniFile(KVARCO::ExePath+IniFile);
		params.FullScreen=!params.FullScreen;
		OutputLog("�N���p�����[�^��ݒ�");

		CGame::bind();
		OutputLog("�e��o�C���h����");

		xtal::ArrayPtr list=CScriptManager::GetInst()->LoadOneFile(params.LoadFileList.c_str()).to_a();
		CScriptManager::GetInst()->LoadFiles(list);
		OutputLog("�X�N���v�g�����[�h");

		xtal::AnyPtr framework=xtal::lib()->member(Xid(GameFramework));
		Game=shared_ptr<CGame>(new CGame(params,framework));
		Game->Init();
		OutputLog("\nKVARCO����������");
		
		//�Q�[���J�n
		OutputLog("\n----------------[Game]----------------\n");
	}

	// �t�@�C��������p�X�����o���ĕԂ�
	string GetFilePath(string s)
	{
		int pos=s.rfind("\\");
		if (pos==(int)string::npos) return "";
		return s.substr(0, pos+1);
	}

	// ���s�t�@�C���̃p�X��Ԃ��i������\���t���j
	string GetExePath()
	{
		char buf[MAX_PATH+1];
		GetModuleFileName(NULL, buf, MAX_PATH);
		return GetFilePath(buf);
	}

	string ExePath=GetExePath();
}

//���t���[���̌o�ߎ��Ԃ�FRAME_COUNT��v�����A���̕��ς��o���B�o�͂�FPS�̌`
void CGame::SetRealFPS()
{
	for(int i=FRAME_COUNT-1; i>=1; i--)
	{
		FrameTimes[i]=FrameTimes[i-1];
	}
	FrameTimes[0]=GetTickCount();
	if(FrameTimes[FRAME_COUNT-1]==0)	return;

	RealFPS=1000/((float )(FrameTimes[0]-FrameTimes[FRAME_COUNT-1])/(FRAME_COUNT-1));
	ZeroMemory(&FrameTimes,sizeof(FrameTimes));
}

void CGame::ResetTime()
{	
	LARGE_INTEGER freq, count;
	if (QueryPerformanceFrequency(&freq) && QueryPerformanceCounter(&count))
	{
		LastHiCount=count;
	}
	else
	{
		LastTickCount=GetTickCount();
	}
}


DWORD CGame::NowProcessPriority=NORMAL_PRIORITY_CLASS;
void CGame::UpProcessPriority(DWORD priority)
{
	if(NowProcessPriority<=priority)
	{
		NowProcessPriority=priority;
		SetPriorityClass(GetCurrentProcess(),priority);
	}
}

CGame::CGame(CGameBootSetting gs,xtal::AnyPtr Framework)
{
	GameFramework=Framework;
	Setting=gs;

	SetWindowIconID(gs.UseIcon);
	SetMainWindowText(gs.Title.c_str());

	//��ʉ𑜓x
	WindowWidth=		gs.WndWidth;
	WindowHeight=		gs.WndWidth;

	//�t���X�N���[���ł̉𑜓x
	FullScreenWidth=	StdWndWidth;
	FullScreenHeight=	StdWndHeight;

	//��b�Ԃ̍X�V��
	FPS=60;			RefreshRate=60;

	//�Œ�t���[�����[�g�p
	Elapsed=		0;
	DropFrame=		false;
	ZeroMemory(&LastHiCount,sizeof(LastHiCount));
	LastTickCount=	0;

	//�t���[�����[�g�v���p
	ZeroMemory(&FrameTimes,sizeof(FrameTimes));
	RealFPS=		60;

	ConfirmExit=	false;
	SetAlwaysRunFlag((int )gs.AlwaysRun);

	//���s�D�揇�ʂ��グ�邩�ǂ���
	UpPriority=		gs.UpPG_Priority;
	
	//�t���X�N���[��
	FullScreen=gs.FullScreen;
	ChangeWindowMode((int )gs.FullScreen);

	//������
	if( DxLib_Init() == -1 )
	{
		MessageBox(NULL,"DirectX�̏������Ɏ��s���܂���","�G���[",0);
		exit(0);
	}

	//�E�B���h�E�ݒ�
	SetWindowSizeExtendRate(gs.WndWidth/StdWndWidth);

	Setting=gs;
	DxLib::SetMultiThreadFlag(TRUE);

	//�o����΂��̃^�C�~���O��ShowWindow()
}

void CGame::Init()
{
	XtalHelper::send(GameFramework,Xid(Init));
}

void CGame::Run()
{
	CInput::GetInst()->Update();

	xtal::AnyPtr end=XtalHelper::send(GameFramework,Xid(Run));
	if((bool )end) Game->SetConfirmExit(true);
}

void CGame::Draw()
{
	XtalHelper::send(GameFramework,Xid(Draw));
}

void CGame::Main()
{

	ResetTime();
	while(ProcessMessage() != -1)
	{
		LARGE_INTEGER freq, count;
		DWORD tick;
		if (QueryPerformanceFrequency(&freq) && QueryPerformanceCounter(&count))
		{
			Elapsed+=FPS*(count.QuadPart-LastHiCount.QuadPart)/freq.QuadPart;
			LastHiCount=count;
		}
		else
		{
			tick=GetTickCount();
			Elapsed+=FPS*(tick-LastTickCount)/1000;
			LastTickCount=tick;
		}

		if(RealFPS<=UpToHigh && UpPriority)	UpProcessPriority(HIGH_PRIORITY_CLASS);

		if (Elapsed>=1.0)
		{
			if (DropFrame)
			{
				for (; Elapsed>=1.0; Elapsed-=1.0)
				{
					SetRealFPS();
					Run();
				}
			}
			else
			{
				Elapsed=0;

				SetRealFPS();
				Run();
			}
			if(ConfirmExit) break;
			DrawScene();
		}

		Sleep(0);
		if(ConfirmExit) break;
	}
	
	UnInit();

}

void CGame::DrawScene()
{
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_BACK);
	
	Draw();
	
	ScreenFlip();
}

void CGame::UnInit()
{
	KVARCO::OutputLog("\n----------------[Release]----------------\n");
	GameFramework=xtal::null;
	KVARCO::OutputLog("GameFramework���J��");

	//���[�f�B���O�X���b�h���s���ɏI�������ꍇ��kill
	if(!KVARCO::IsLoadingEnd())
	{
		CLoadingThread::Release();
		SAFE_DELETE(KVARCO::LoadingThread);
	}

	KVARCO::OutputLog("--�e��V���O���g���N���X�̊J��");
	CInput::GetInst()		->Release();	KVARCO::OutputLog("CInput�N���X���J��");
	CActorManager::GetInst()->Release();	KVARCO::OutputLog("CActorManager�N���X���J��");
	CLayerManager::GetInst()->Release();	KVARCO::OutputLog("CLayerManager�N���X���J��");
	CSceneManager::GetInst()->Release();	KVARCO::OutputLog("CSceneManager�N���X���J��");
	CAudioManager::GetInst()->Release();	KVARCO::OutputLog("CAudioManager�N���X���J��");
	CColPolygon::Destroy();					KVARCO::OutputLog("CColPolygon::VoidPolygon���J��");
	KVARCO::OutputLog("----------------");

	xtal::uninitialize();	KVARCO::OutputLog("Xtal���J��");
	DxLib::DxLib_End();		KVARCO::OutputLog("DirectX���J��");

	OpenAL_Ogg::UnInit();	KVARCO::OutputLog("OpenAL���J��");

	KVARCO::OutputLog("----------------");
	KVARCO::OutputLog("KVARCO���J��");
	KVARCO::OutputLog("\n----------------[End]----------------");
	fclose(KVARCO::LogFile);
}