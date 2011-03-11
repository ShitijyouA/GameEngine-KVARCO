#include "pch.h"
#include "AudioItem.h"

CBGM_Item::CBGM_Item(xtal::StringPtr path,DWORD loop_point,bool do_repeat)
	:Player(KVARCO::ExePath+string(path->c_str()),loop_point,do_repeat)
{
}

void CBGM_Item::Load()
{
	Player.Load();
}

void CBGM_Item::Play()
{
	PlayThread=thread(bind(&CPlayThread::Run,&Player));
}

void CBGM_Item::Play(BYTE volume)
{
	Play();
	SetVolume(volume);
}

void CBGM_Item::Stop()
{
	Player.Stop();
	PlayThread.join();
	PlayThread.interrupt();
}

void CBGM_Item::Pause()
{
	Player.Pause();
}

void CBGM_Item::SetVolume(BYTE vol)
{
	Player.ChangeVolume(vol);
}

CSE_Item::CSE_Item(xtal::StringPtr path)
	:Source(KVARCO::ExePath+string(path->c_str()))
{
	Player=NULL;
	Loaded=false;
}

void CSE_Item::Load()
{
	if(Loaded) return;

	Source.Load();
	Loaded	=true;
	Volume	=100;
}

void CSE_Item::SetVolume(BYTE vol)
{
	Volume=vol;
}

void CSE_Item::Play()
{
	if(!Loaded) return;

	Player=new CSimplePlay_Thread(&Source,Volume);
	PlayThread=thread(bind(&CSimplePlay_Thread::Run,Player));
}

void CSE_Item::Play(BYTE volume)
{
	SetVolume(volume);
	Play();
}

void CSE_Item::Stop()
{
	Player->Stop();
	PlayThread.join();
	PlayThread.interrupt();
}