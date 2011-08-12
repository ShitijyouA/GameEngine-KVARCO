#include "pch.h"
#include "AudioManager.h"

AudioManager::AudioManager()
{
	BGM_volume_			=100;
	SE_volume_			=100;
	current_BGM_item_	=xtal::null;
}

void AudioManager::LoadAudio(AudioItemPtrX item,std::string name)
{
	kvarco::OutputLog("%sをロード",name.c_str());
	AudioMapIteratorType i=FindItem(name);
	if(i==audio_item_map_.end())
	{
		item->Load();
		audio_item_map_.insert(std::make_pair(name,item));
	}
}

AudioManager::AudioMapIteratorType AudioManager::FindItem(std::string& name)
{
	AudioMapIteratorType i=audio_item_map_.find(name);
	return i;
}

void AudioManager::UnLoadAudio(std::string name)
{
	kvarco::OutputLog("%sをアンロード",name.c_str());
	AudioMapIteratorType i=FindItem(name);
	i->second->Stop();

	i->second=xtal::null;
	audio_item_map_.erase(i);
}

void AudioManager::PlayAudio(std::string name)
{
	AudioMapIteratorType i=FindItem(name);

	//dynamic_castで種類判別
	BGM_ItemPtr tmp=xtal::ptr_cast<BGM_Item>(i->second);

	//BGMは同時に一曲のみ
	if(!xtal::is_null(tmp) && current_BGM_item_!=tmp)
	{
		if(!xtal::is_null(current_BGM_item_))	current_BGM_item_->Stop();
		current_BGM_item_=tmp;
	}
	
	if(!xtal::is_null(tmp))		i->second->Play(BGM_volume_);
	else						i->second->Play(SE_volume_);
}

void AudioManager::StopAudio(std::string name)
{
	AudioMapIteratorType i=FindItem(name);
	i->second->Stop();
}

void AudioManager::SetBGM_volume_(const BYTE x)
{
	BGM_volume_=x;
	if(!xtal::is_null(current_BGM_item_)) current_BGM_item_->SetVolume(x);
}

//BGMかどうかは分からないので一時停止になるかは不定
//SEでもエラーにはならない
void AudioManager::PauseBGM(std::string name)
{
	AudioMapIteratorType i=FindItem(name);
	if(i!=audio_item_map_.end()) i->second->Pause();
}

void AudioManager::ReleaseAllAudioItem()
{
	current_BGM_item_=xtal::null;

	typedef std::pair<const std::string,AudioItemPtrX> audio_item_map_Item;
	BOOST_FOREACH(audio_item_map_Item& i,audio_item_map_)
	{
		i.second->Stop();
		i.second=xtal::null;
	}
	audio_item_map_.clear();
}

void AudioManager::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(AudioManager);

	Xdef_methodx(LoadAudio);
	Xdef_methodx(UnLoadAudio);
	Xdef_methodx(PlayAudio);
	Xdef_methodx(StopAudio);
	Xdef_methodx(PauseBGM);
	Xdef_method(SetBGM_volume_);
	Xdef_method(GetBGM_volume_);
	Xdef_method(SetSE_volume_);
	Xdef_method(GetSE_volume_);
}
