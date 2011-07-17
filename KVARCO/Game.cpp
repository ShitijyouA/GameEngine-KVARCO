#include "pch.h"
#include "BaseActor.h"
#include "XtalHelper.h"
#include "LoadThread.h"
#include "Layer2.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Game.h"
#include "Collision.h"

LoadThread* kvarco::LoadingThread;

namespace kvarco
{

// ファイル名からパスを取り出して返す
std::string GetFilePath(std::string s)
{
	int pos=s.rfind("\\");
	if (pos==(int)std::string::npos) return "";
	return s.substr(0, pos+1);
}

// 実行ファイルのパスを返す（末尾に\が付く）
std::string GetExePath()
{
	char buf[MAX_PATH+1];
	GetModuleFileName(NULL, buf, MAX_PATH);
	return GetFilePath(buf);
}

fsys::path ExePath=fsys::path(GetExePath().c_str());

GameBootSettingPtr GetGameSetting()
{
	return &(Game::GetInst()->Setting);
}
}

Game* Game::Inst=NULL;
void Game::Boot(const fsys::path& ini_file)
{
	using kvarco::OutputLog;

	OutputLog("*********************************************************");
	OutputLog("    -KVARCO ver1.12- %s","2011-06-27");
	OutputLog("動作ログ");
	OutputLog("*********************************************************\n");

	OutputLog("----------------[Boot]----------------\n");

	//メモリリークを検出
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	XtalHelper::Init();
	OutputLog("Xtalを初期化");

	GameBootSetting params=kvarco::InitFromIniFile(fsys::absolute(ini_file,kvarco::ExePath));
	params.FullScreen=!params.FullScreen;
	OutputLog("起動パラメータを設定");

	Game::bind();
	OutputLog("各種バインド完了");

	xtal::ArrayPtr list=ScriptManager::GetInst()->LoadOneFile(params.LoadFileList.c_str()).to_a();
	ScriptManager::GetInst()->LoadFiles(list);
	OutputLog("スクリプトをロード");

	xtal::AnyPtr framework=xtal::lib()->member(Xid(GameFramework));
	Inst=new Game(params,framework);
	Game::GetInst()->Init();
	OutputLog("\nKVARCO初期化完了");
	
	//ゲーム開始
	OutputLog("\n----------------[Game]----------------\n");
}

//毎フレームの経過時間をFRAME_COUNT回計測し、その平均を出す。出力はFPSの形
void Game::SetRealFPS()
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

void Game::ResetTime()
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


DWORD Game::NowProcessPriority=NORMAL_PRIORITY_CLASS;
void Game::UpProcessPriority(DWORD priority)
{
	if(NowProcessPriority<=priority)
	{
		NowProcessPriority=priority;
		SetPriorityClass(GetCurrentProcess(),priority);
	}
}

Game::Game(GameBootSetting gs,xtal::AnyPtr Framework)
{
	GameFramework=Framework;
	Setting=gs;

	SetWindowIconID(gs.UseIcon);
	SetMainWindowText(gs.Title.c_str());

	//画面解像度
	WindowWidth=		gs.WndWidth;
	WindowHeight=		gs.WndWidth;

	//フルスクリーンでの解像度
	FullScreenWidth=	StdWndWidth;
	FullScreenHeight=	StdWndHeight;

	//一秒間の更新回数
	FPS=60;			RefreshRate=60;

	//固定フレームレート用
	Elapsed=		0;
	DropFrame=		false;
	ZeroMemory(&LastHiCount,sizeof(LastHiCount));
	LastTickCount=	0;

	//フレームレート計測用
	ZeroMemory(&FrameTimes,sizeof(FrameTimes));
	RealFPS=		60;

	ConfirmExit=	false;
	SetAlwaysRunFlag((int )gs.AlwaysRun);

	//実行優先順位を上げるかどうか
	UpPriority=		gs.UpPG_Priority;
	
	//フルスクリーン
	FullScreen=gs.FullScreen;
	ChangeWindowMode((int )gs.FullScreen);

	//初期化
	if( DxLib_Init() == -1 )
	{
		MessageBox(NULL,"DirectXの初期化に失敗しました","エラー",0);
		exit(0);
	}

	//ウィンドウ設定
	SetWindowSizeExtendRate(gs.WndWidth/StdWndWidth);

	Setting=gs;
	DxLib::SetMultiThreadFlag(TRUE);

	//出来ればこのタイミングでShowWindow()
}

void Game::Init()
{
	XtalHelper::send(GameFramework,Xid(Init));
}

void Game::Run()
{
	Input::GetInst()->Update();

	xtal::AnyPtr end=XtalHelper::send(GameFramework,Xid(Run));
	if((bool )end) Game::GetInst()->SetConfirmExit(true);
}

void Game::Draw()
{
	XtalHelper::send(GameFramework,Xid(Draw));
}

void Game::Main()
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

void Game::DrawScene()
{
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_BACK);
	
	Draw();
	
	ScreenFlip();
}

void Game::UnInit()
{
	kvarco::OutputLog("\n----------------[Release]----------------\n");
	GameFramework=xtal::null;
	kvarco::OutputLog("GameFrameworkを開放");

	//ローディングスレッド実行中に終了した場合はkill
	if(!kvarco::IsLoadingEnd())
	{
		LoadThread::Release();
		SAFE_DELETE(kvarco::LoadingThread);
	}

	kvarco::OutputLog("--各種シングルトンクラスの開放");
	Input::GetInst()		 ->Release();	kvarco::OutputLog("Inputクラスを開放");
	ActorManager::	GetInst()->Release();	kvarco::OutputLog("ActorManagerクラスを開放");
	LayerManager::	GetInst()->Release();	kvarco::OutputLog("LayerManagerクラスを開放");
	SceneManager::	GetInst()->Release();	kvarco::OutputLog("SceneManagerクラスを開放");
	AudioManager::	GetInst()->Release();	kvarco::OutputLog("AudioManagerクラスを開放");
	ScriptManager::	GetInst()->Release();	kvarco::OutputLog("ScriptManagerクラスを開放");
	ColPolygon::			   Release();	kvarco::OutputLog("ColPolygon::VoidPolygonを開放");
	kvarco::OutputLog("----------------");

	xtal::uninitialize();	kvarco::OutputLog("Xtalを開放");
	DxLib::DxLib_End();		kvarco::OutputLog("DirectXを開放");

	OpenAL_Ogg::UnInit();	kvarco::OutputLog("OpenALを開放");

	kvarco::OutputLog("----------------");
	kvarco::OutputLog("KVARCOを開放");
	kvarco::OutputLog("\n----------------[End]----------------");
	fclose(kvarco::LogFile);
}
