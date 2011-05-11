#pragma once
#include "KVARCO.h"
#include "Utillity.h"
#include "AudioItem.h"

typedef xtal::SmartPtr<AudioItem> AudioItemPtrX;
typedef xtal::SmartPtr<BGM_Item> BGM_ItemPtr;
typedef boost::unordered_map<string,AudioItemPtrX> AudioMap;
typedef AudioMap::iterator AudioMap_i;

class AudioManager;
typedef AudioManager* AudioMngrPtr;

class AudioManager
{
	AudioMap AudioItemMap;

	BYTE BGM_Volume;
	BYTE SE_Volume;

	AudioMap_i FindItem(string& name);
	void ReleaseAllAudioItem();

	//同時に再生できるBGMアイテムは一つのみ
	BGM_ItemPtr CurrentBGM;

public:
	void LoadAudio(AudioItemPtrX item,string name);
	void LoadAudioX(xtal::AnyPtr item,xtal::StringPtr name)
		{
			AudioItemPtrX tmp=xtal::ptr_cast<AudioItem>(item);
			LoadAudio(tmp,name->c_str());
		}

	void UnLoadAudio(string name);
	void UnLoadAudioX(xtal::StringPtr name)
		{
			UnLoadAudio(name->c_str());
		}

	void PlayAudio(string name);
	void NAME_IN_X(PlayAudio)(xtal::StringPtr name)
		{
			PlayAudio(name->c_str());
		}

	void StopAudio(string name);
	void NAME_IN_X(StopAudio)(xtal::StringPtr name)
		{
			StopAudio(name->c_str());
		}

	void PauseBGM(string name);
	void NAME_IN_X(PauseBGM)(xtal::StringPtr name)
		{
			PauseBGM(name->c_str());
		}

	SET_GET(BYTE,SE_Volume)
	SETTER_(BYTE,BGM_Volume)
	GETTER(BYTE,BGM_Volume)

	void Release()
		{
			ReleaseAllAudioItem();
		}

	static void bind(xtal::ClassPtr it);

	//Singleton Pattern
	//コンストラクタが自前なので非マクロ
private:
	FRIENDS_FOR_XTAL(AudioManager)

	AudioManager();

public:
	static AudioMngrPtr GetInst()
		{
			static AudioManager Inst;
			return &Inst;
		}

	~AudioManager()
		{
			Release();
		}
};