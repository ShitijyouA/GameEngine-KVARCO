#pragma once
#include "ScManager.h"
#include "XtalHelper.h"
#include "LoadingThread.h"

//FPS計測用定数
#define FRAME_COUNT			30

//優先順位を上げるときの目安
#define UpToHigh			20

//CGameの設定構造体
class CGameBootSetting
{
public:
	CGameBootSetting()
		:Title("Game"),UseIcon(0),
		FullScreen(true),
		WndWidth(640),WndHeight(480),
		RockFPS(false),AlwaysRun(true),
		UpPG_Priority(false),
		LoadFileList("")
	{;}

	xtal::String	Title;
	int		UseIcon;
	bool	FullScreen;

	int		WndWidth;
	int		WndHeight;

	bool	RockFPS;
	bool	AlwaysRun;

	bool	UpPG_Priority;

	string LoadFileList;
};

class CGame
{
	//固定フレームレート調整用
	LARGE_INTEGER LastHiCount;
	DWORD LastTickCount;
	float Elapsed;
	void ResetTime();

	//可変フレームレート調節用
	DWORD FrameTimes[FRAME_COUNT];
	float RealFPS;
	void SetRealFPS();

	bool ConfirmExit;	//終了確認

	int IconID;

	//画面サイズ、リフレッシュレート
	int WindowWidth, WindowHeight,
		FullScreenWidth, FullScreenHeight,
		RefreshRate;
	bool FullScreen;

	static DWORD NowProcessPriority;
	static void UpProcessPriority(DWORD priority);

public:

	//コンストラクタ
	CGame(CGameBootSetting gs,xtal::AnyPtr FrameWork);
	//初期化
	void Init();

	//終了
	void UnInit();

	// ゲームの実行
	void Main();
	
	// 各種設定
	float FPS;
	bool DropFrame;
	bool UpPriority;

	// 各種情報の取得
	int		GetRefreshRate()	{ return RefreshRate;	}
	bool	GetFullScreen()		{ return FullScreen;	}
	float	GetRealFPS()		{ return RealFPS;		}
	
	void SetConfirmExit(bool ce)	{ ConfirmExit	=ce;	} // 終了確認
	void SetFullScreen(bool fs)		{ FullScreen	=fs;	}
	
	//更新と描画
	void Run();
	void Draw();

	void DrawScene();

	CGameBootSetting Setting;
	xtal::AnyPtr GameFramework;

	static void bind();
};

namespace KVARCO
{
	//イメージ名とイメージハンドルを登録するmap
	extern unordered_map<string,GR_INFO>	ImageNameList;

	//ローディングクラスのポインタ。実行してないとNULL
	extern CLoadingThread*	LoadingThread;
	
	extern	void Boot(string IniFile);
	extern	CGameBootSetting InitFromIniFile(string path);
}

extern	shared_ptr<CGame>	Game;