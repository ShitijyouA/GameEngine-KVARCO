#include "pch.h"
#include "AudioManager.h"

AudioManager::AudioManager()
{
	BGM_Volume	=100;
	SE_Volume	=100;
	CurrentBGM	=xtal::null;
}

void AudioManager::LoadAudio(AudioItemPtrX item,string name)
{
	AudioMap_i i=FindItem(name);
	if(i==AudioItemMap.end())
	{
		item->Load();
		AudioItemMap.insert(make_pair(name,item));
	}
}

AudioMap_i AudioManager::FindItem(string& name)
{
	AudioMap_i i=AudioItemMap.find(name);
	return i;
}

void AudioManager::UnLoadAudio(string name)
{
	AudioMap_i i=FindItem(name);
	i->second->Stop();

	i->second=xtal::null;
	AudioItemMap.erase(i);
}

void AudioManager::PlayAudio(string name)
{
	AudioMap_i i=FindItem(name);

	//dynamic_castで種類判別
	BGM_ItemPtr tmp=xtal::ptr_cast<BGM_Item>(i->second);

	//BGMは同時に一曲のみ
	if(!xtal::is_null(tmp) && CurrentBGM!=tmp)
	{
		if(!xtal::is_null(CurrentBGM))	CurrentBGM->Stop();
		CurrentBGM=tmp;
	}
	
	if(!xtal::is_null(tmp))		i->second->Play(BGM_Volume);
	else						i->second->Play(SE_Volume);
}

void AudioManager::StopAudio(string name)
{
	AudioMap_i i=FindItem(name);
	i->second->Stop();
}

void AudioManager::SetBGM_Volume(const BYTE x)
{
	BGM_Volume=x;
	if(!xtal::is_null(CurrentBGM)) CurrentBGM->SetVolume(x);
}

//BGMかどうかは分からないので一時停止になるかは不定
//SEでもエラーにはならない
void AudioManager::PauseBGM(string name)
{
	AudioMap_i i=FindItem(name);
	if(i!=AudioItemMap.end()) i->second->Pause();
}

void AudioManager::ReleaseAllAudioItem()
{
	CurrentBGM=xtal::null;

	typedef pair<const string,AudioItemPtrX> AudioItemMapItem;
	BOOST_FOREACH(AudioItemMapItem& i,AudioItemMap)
	{
		i.second->Stop();
		i.second=xtal::null;
	}
	AudioItemMap.clear();
}

void AudioManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(AudioManager);

	Xdef_methodx(LoadAudio);
	Xdef_methodx(UnLoadAudio);
	Xdef_methodx(PlayAudio);
	Xdef_methodx(StopAudio);
	Xdef_methodx(PauseBGM);
	Xdef_method(SetBGM_Volume);
	Xdef_method(GetBGM_Volume);
	Xdef_method(SetSE_Volume);
	Xdef_method(GetSE_Volume);
}