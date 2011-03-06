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

//スクリプトロードなどのみ。画像ロードなどは行わない
namespace KVARCO
{
	void Boot(string IniFile)
	{
		OutputLog("*********************************************************");
		OutputLog("    -KVARCO ver1.00- %s","2011-02-11");
		OutputLog("動作ログ");
		OutputLog("*********************************************************\n");

		OutputLog("----------------[Boot]----------------\n");

		//メモリリークを検出
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		XtalHelper::Init();
		OutputLog("Xtalを初期化");

		CGameBootSetting params=InitFromIniFile(KVARCO::ExePath+IniFile);
		params.FullScreen=!params.FullScreen;
		OutputLog("起動パラメータを設定");

		CGame::bind();
		OutputLog("各種バインド完了");

		xtal::ArrayPtr list=CScriptManager::GetInst()->LoadOneFile(params.LoadFileList.c_str()).to_a();
		CScriptManager::GetInst()->LoadFiles(list);
		OutputLog("スクリプトをロード");

		xtal::AnyPtr framework=xtal::lib()->member(Xid(GameFramework));
		Game=shared_ptr<CGame>(new CGame(params,framework));
		Game->Init();
		OutputLog("\nKVARCO初期化完了");
		
		//ゲーム開始
		OutputLog("\n----------------[Game]----------------\n");
	}

	// ファイル名からパスを取り出して返す
	string GetFilePath(string s)
	{
		int pos=s.rfind("\\");
		if (pos==(int)string::npos) return "";
		return s.substr(0, pos+1);
	}

	// 実行ファイルのパスを返す（末尾に\が付く）
	string GetExePath()
	{
		char buf[MAX_PATH+1];
		GetModuleFileName(NULL, buf, MAX_PATH);
		return GetFilePath(buf);
	}

	string ExePath=GetExePath();

	GameBootSettingPtr GetGameSetting()
	{
		return &Game->Setting;
	}
}

//毎フレームの経過時間をFRAME_COUNT回計測し、その平均を出す。出力はFPSの形
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
		MessageBox(NULL,"DiRectXの初期化に失敗しました","エラー",0);
		exit(0);
	}

	//ウィンドウ設定
	SetWindowSizeExtendRate(gs.WndWidth/StdWndWidth);

	Setting=gs;
	DxLib::SetMultiThreadFlag(TRUE);

	//出来ればこのタイミングでShowWindow()
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
	KVARCO::OutputLog("GameFrameworkを開放");

	//ローディングスレッド実行中に終了した場合はkill
	if(!KVARCO::IsLoadingEnd())
	{
		CLoadingThread::Release();
		SAFE_DELETE(KVARCO::LoadingThread);
	}

	KVARCO::OutputLog("--各種シングルトンクラスの開放");
	CInput::GetInst()		->Release();	KVARCO::OutputLog("CInputクラスを開放");
	CActorManager::GetInst()->Release();	KVARCO::OutputLog("CActorManagerクラスを開放");
	CLayerManager::GetInst()->Release();	KVARCO::OutputLog("CLayerManagerクラスを開放");
	CSceneManager::GetInst()->Release();	KVARCO::OutputLog("CSceneManagerクラスを開放");
	CAudioManager::GetInst()->Release();	KVARCO::OutputLog("CAudioManagerクラスを開放");
	CColPolygon::Destroy();					KVARCO::OutputLog("CColPolygon::VoidPolygonを開放");
	KVARCO::OutputLog("----------------");

	xtal::uninitialize();	KVARCO::OutputLog("Xtalを開放");
	DxLib::DxLib_End();		KVARCO::OutputLog("DiRectXを開放");

	OpenAL_Ogg::UnInit();	KVARCO::OutputLog("OpenALを開放");

	KVARCO::OutputLog("----------------");
	KVARCO::OutputLog("KVARCOを開放");
	KVARCO::OutputLog("\n----------------[End]----------------");
	fclose(KVARCO::LogFile);
}