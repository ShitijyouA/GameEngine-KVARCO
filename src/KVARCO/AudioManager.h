#pragma once
#include "KVARCO.h"
#include "Utillity.h"
#include "AudioItem.h"

class AudioManager
{
public:
	typedef xtal::SmartPtr<AudioItem>						AudioItemPtrX;
	typedef xtal::SmartPtr<BGM_Item>						BGM_ItemPtr;
	typedef boost::unordered_map<std::string,AudioItemPtrX>	AudioMapType;
	typedef AudioMapType::iterator							AudioMapIteratorType;
	typedef AudioManager*									AudioMngrPtr;

private:
	AudioMapType audio_item_map_;

	BYTE BGM_volume_;
	BYTE SE_volume_;

	AudioMapIteratorType FindItem(std::string& name);
	void ReleaseAllAudioItem();

	//同時に再生できるBGMアイテムは一つのみ
	BGM_ItemPtr current_BGM_item_;

public:
	void LoadAudio(AudioItemPtrX item,std::string name);
	void LoadAudioX(xtal::AnyPtr item,xtal::StringPtr name)
		{
			AudioItemPtrX tmp=xtal::ptr_cast<AudioItem>(item);
			LoadAudio(tmp,name->c_str());
		}

	void UnLoadAudio(std::string name);
	void UnLoadAudioX(xtal::StringPtr name)
		{
			UnLoadAudio(name->c_str());
		}

	void PlayAudio(std::string name);
	void NAME_IN_X(PlayAudio)(xtal::StringPtr name)
		{
			PlayAudio(name->c_str());
		}

	void StopAudio(std::string name);
	void NAME_IN_X(StopAudio)(xtal::StringPtr name)
		{
			StopAudio(name->c_str());
		}

	void PauseBGM(std::string name);
	void NAME_IN_X(PauseBGM)(xtal::StringPtr name)
		{
			PauseBGM(name->c_str());
		}

	SET_GET(BYTE,SE_volume_)
	SETTER_(BYTE,BGM_volume_)
	GETTER(BYTE,BGM_volume_)

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