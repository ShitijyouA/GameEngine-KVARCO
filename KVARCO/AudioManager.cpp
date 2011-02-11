#include "pch.h"
#include "AudioManager.h"

CAudioManager::CAudioManager()
{
	BGM_Volume	=100;
	SE_Volume	=100;
	CurrentBGM	=xtal::null;
}

void CAudioManager::LoadAudio(CAudioItemPtr item,string name)
{
	item->Load();
	AudioItemMap.insert(make_pair(name,item));
}

AudioMap_i CAudioManager::FindItem(string& name)
{
	AudioMap_i i=AudioItemMap.find(name);
	return i;
}

void CAudioManager::UnLoadAudio(string name)
{
	AudioMap_i i=FindItem(name);
	i->second->Stop();

	i->second=xtal::null;
	AudioItemMap.erase(i);
}

void CAudioManager::PlayAudio(string name)
{
	AudioMap_i i=FindItem(name);

	//dynamic_cast‚ÅŽí—Þ”»•Ê
	CBGM_ItemPtr tmp=xtal::ptr_cast<CBGM_Item>(i->second);

	if(!xtal::is_null(tmp))		i->second->Play(BGM_Volume);
	else						i->second->Play(SE_Volume);

	//BGM‚Í“¯Žž‚Éˆê‹È‚Ì‚Ý
	if(tmp)
	{
		if(!xtal::is_null(CurrentBGM)) CurrentBGM->Stop();
		CurrentBGM=tmp;
	}
}

void CAudioManager::StopAudio(string name)
{
	AudioMap_i i=FindItem(name);
	i->second->Stop();
}

void CAudioManager::SetBGM_Volume(const BYTE x)
{
	BGM_Volume=x;
	if(!xtal::is_null(CurrentBGM)) CurrentBGM->SetVolume(x);
}

//BGM‚©‚Ç‚¤‚©‚Í•ª‚©‚ç‚È‚¢‚Ì‚ÅˆêŽž’âŽ~‚É‚È‚é‚©‚Í•s’è
//SE‚Å‚àƒGƒ‰[‚É‚Í‚È‚ç‚È‚¢
void CAudioManager::PauseBGM(string name)
{
	AudioMap_i i=FindItem(name);
	if(i!=AudioItemMap.end()) i->second->Pause();
}

void CAudioManager::ReleaseAllAudioItem()
{
	CurrentBGM=xtal::null;

	typedef pair<const string,CAudioItemPtr> AudioItemMapItem;
	BOOST_FOREACH(AudioItemMapItem& i,AudioItemMap)
	{
		i.second->Stop();
		i.second=xtal::null;
	}
	AudioItemMap.clear();
}

void CAudioManager::bind(xtal::ClassPtr it)
{
#define BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(_fun,_name)\
	it->def(Xid(_name),xtal::method(&CAudioManager::_fun))

	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(LoadAudioX,LoadAudio);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(UnLoadAudioX,UnLoadAudio);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(PlayAudioX,PlayAudio);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(StopAudioX,StopAudio);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(PauseBGMX,PauseBGM);

	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(SetBGM_Volume,SetBGM_Volume);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(GetBGM_Volume,GetBGM_Volume);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(SetSE_Volume,SetSE_Volume);
	BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR(GetSE_Volume,GetSE_Volume);

#undef BIND_XTAL_CLASSFUN_RENAME_IT_AUDIO_MNGR
}