#pragma once
#include "PlayThread.h"
#include "SimplePlay.h"
#include "Utillity.h"
#include "KVARCO.h"

class CAudioManager;

class CAudioItem
{
protected:
	friend class CAudioManager;

	thread PlayThread;
	virtual void SetVolume(BYTE vol)	{}
public:
	virtual void Play()					=0;
	virtual void Play(BYTE volume)		=0;
	virtual void Stop()					=0;

	virtual void Load()					{}
	virtual void Pause()				{}
	virtual void ChangeVolume(BYTE vol) {}

	virtual ~CAudioItem()				{}
};

class CBGM_Item : public CAudioItem
{
	friend class CAudioManager;

	void SetVolume(BYTE vol);
	CPlayThread Player;

public:
	CBGM_Item(xtal::StringPtr path,DWORD loop_point,bool do_repeat=true);

	void Play();
	void Play(BYTE volume);
	void Stop();
	void Pause();

	~CBGM_Item()
	{
		Stop();
	}
};

class CSE_Item : public CAudioItem
{
	friend class CAudioManager;

	BYTE Volume;
	void SetVolume(BYTE vol);

	bool Loaded;
	CSource Source;
	CSimplePlay_Thread* Player;

public:
	CSE_Item(xtal::StringPtr path);

	void Load();
	void Play();
	void Play(BYTE volume);
	void Stop();

	~CSE_Item()
		{
			Stop();
			SAFE_DELETE(Player);
		}
};