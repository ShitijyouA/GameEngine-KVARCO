#pragma once
#include "PlayThread.h"
#include "SimplePlay.h"
#include "Utillity.h"
#include "KVARCO.h"

class AudioManager;

class AudioItem
{
protected:
	friend class AudioManager;

	boost::thread player_thread_;
	virtual void SetVolume(BYTE vol)	{}
public:
	virtual void Play()					=0;
	virtual void Play(BYTE volume)		=0;
	virtual void Stop()					=0;

	virtual void Load()					{}
	virtual void Pause()				{}
	virtual void ChangeVolume(BYTE vol) {}

	virtual ~AudioItem()				{}
};

class BGM_Item : public AudioItem
{
	friend class AudioManager;

	void SetVolume(BYTE vol);
	PlayThread Player;

public:
	BGM_Item(xtal::StringPtr path,DWORD loop_point,bool do_repeat=true);

	void Play();
	void Play(BYTE volume);
	void Stop();
	void Pause();
	void Load();

	~BGM_Item()
	{
		Stop();
	}
};

class SE_Item : public AudioItem
{
	friend class AudioManager;

	BYTE Volume;
	void SetVolume(BYTE vol);

	bool Loaded;
	AudioSource Source;
	SimplePlay_Thread* Player;

public:
	SE_Item(xtal::StringPtr path);

	void Load();
	void Play();
	void Play(BYTE volume);
	void Stop();

	~SE_Item()
		{
			Stop();
			SAFE_DELETE(Player);
		}
};
