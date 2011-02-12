#pragma once
#include "KVARCO.h"
#include "AudioItem.h"

typedef xtal::SmartPtr<CAudioItem> CAudioItemPtr;
typedef xtal::SmartPtr<CBGM_Item> CBGM_ItemPtr;
typedef unordered_map<string,CAudioItemPtr> AudioMap;
typedef AudioMap::iterator AudioMap_i;

class CAudioManager;
typedef CAudioManager* AudioMngrPtr;

class CAudioManager
{
	AudioMap AudioItemMap;

	BYTE BGM_Volume;
	BYTE SE_Volume;

	AudioMap_i FindItem(string& name);
	void ReleaseAllAudioItem();

	//同時に再生できるBGMアイテムは一つのみ
	CBGM_ItemPtr CurrentBGM;

public:
	void LoadAudio(CAudioItemPtr item,string name);
	void LoadAudioX(xtal::AnyPtr item,xtal::StringPtr name)
		{
			CAudioItemPtr tmp=xtal::ptr_cast<CAudioItem>(item);
			LoadAudio(tmp,name->c_str());
		}

	void UnLoadAudio(string name);
	void UnLoadAudioX(xtal::StringPtr name)
		{
			UnLoadAudio(name->c_str());
		}

	void PlayAudio(string name);
	void PlayAudioX(xtal::StringPtr name)
		{
			PlayAudio(name->c_str());
		}

	void StopAudio(string name);
	void StopAudioX(xtal::StringPtr name)
		{
			StopAudio(name->c_str());
		}

	void PauseBGM(string name);
	void PauseBGMX(xtal::StringPtr name)
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
	friend xtal::SmartPtr<CAudioManager> xtal::xnew();
	friend class xtal::SmartPtr<CAudioManager>;
	friend struct xtal::XNew<CAudioManager>;
	friend struct xtal::XXNew<CAudioManager,3>;

	CAudioManager();

public:
	static AudioMngrPtr GetInst()
		{
			static CAudioManager Inst;
			return &Inst;
		}

	~CAudioManager()
		{
			Release();
		}
};