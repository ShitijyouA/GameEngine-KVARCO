#pragma once
#include "ScManager.h"
#include "XtalHelper.h"
#include "LoadingThread.h"

//FPS�v���p�萔
#define FRAME_COUNT			30

//�D�揇�ʂ��グ��Ƃ��̖ڈ�
#define UpToHigh			20

//CGame�̐ݒ�\����
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
	//�Œ�t���[�����[�g�����p
	LARGE_INTEGER LastHiCount;
	DWORD LastTickCount;
	float Elapsed;
	void ResetTime();

	//�σt���[�����[�g���ߗp
	DWORD FrameTimes[FRAME_COUNT];
	float RealFPS;
	void SetRealFPS();

	bool ConfirmExit;	//�I���m�F

	int IconID;

	//��ʃT�C�Y�A���t���b�V�����[�g
	int WindowWidth, WindowHeight,
		FullScreenWidth, FullScreenHeight,
		RefreshRate;
	bool FullScreen;

	static DWORD NowProcessPriority;
	static void UpProcessPriority(DWORD priority);

public:

	//�R���X�g���N�^
	CGame(CGameBootSetting gs,xtal::AnyPtr FrameWork);
	//������
	void Init();

	//�I��
	void UnInit();

	// �Q�[���̎��s
	void Main();
	
	// �e��ݒ�
	float FPS;
	bool DropFrame;
	bool UpPriority;

	// �e����̎擾
	int		GetRefreshRate()	{ return RefreshRate;	}
	bool	GetFullScreen()		{ return FullScreen;	}
	float	GetRealFPS()		{ return RealFPS;		}
	
	void SetConfirmExit(bool ce)	{ ConfirmExit	=ce;	} // �I���m�F
	void SetFullScreen(bool fs)		{ FullScreen	=fs;	}
	
	//�X�V�ƕ`��
	void Run();
	void Draw();

	void DrawScene();

	CGameBootSetting Setting;
	xtal::AnyPtr GameFramework;

	static void bind();
};

namespace KVARCO
{
	//�C���[�W���ƃC���[�W�n���h����o�^����map
	extern unordered_map<string,GR_INFO>	ImageNameList;

	//���[�f�B���O�N���X�̃|�C���^�B���s���ĂȂ���NULL
	extern CLoadingThread*	LoadingThread;
	
	extern	void Boot(string IniFile);
	extern	CGameBootSetting InitFromIniFile(string path);
}

extern	shared_ptr<CGame>	Game;