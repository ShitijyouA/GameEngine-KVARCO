#pragma once
#include "ScriptManager.h"
#include "XtalHelper.h"
#include "LoadThread.h"

//FPS計測用定数
#define FRAME_COUNT			30

//優先順位を上げるときの目安
#define UpToHigh			20

//Gameの設定構造体
class GameBootSetting
{
public:
	GameBootSetting()
		:Title("Game"),UseIcon(0),
		FullScreen(true),
		WndWidth(640),WndHeight(480),
		RockFPS(false),AlwaysRun(true),
		UpPG_Priority(false),
		LoadFileList("")
	{;}

	std::string	Title;
	std::string LoadFileList;

	int		UseIcon;
	
	int		WndWidth;
	int		WndHeight;
	bool	FullScreen;

	bool	RockFPS;
	bool	AlwaysRun;

	bool	UpPG_Priority;
};

typedef xtal::SmartPtr<GameBootSetting> GameBootSettingPtr;

class Game
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
	static Game* Inst;

public:

	//Boot
	static void Boot(const fsys::path& ini_file);

	//コンストラクタ
	Game(GameBootSetting gs,xtal::AnyPtr FrameWork);

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

	GameBootSetting Setting;
	xtal::AnyPtr GameFramework;

	static void bind();

	//必ず先にBootを呼ぶこと
	static Game* GetInst()
	{
		assert(Inst!=NULL);
		return Inst;
	}
};

namespace kvarco
{
	//ローディングクラスのポインタ。実行してないとNULL
	extern LoadThread*	LoadingThread;

	extern	GameBootSetting InitFromIniFile(const fsys::path& path);
	extern	GameBootSettingPtr GetGameSetting();
}
