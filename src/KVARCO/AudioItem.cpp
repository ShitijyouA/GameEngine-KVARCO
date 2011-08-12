#include "pch.h"
#include "AudioItem.h"

BGM_Item::BGM_Item(xtal::StringPtr path,DWORD loop_point,bool do_repeat)
:Player(fsys::absolute(path->c_str(),kvarco::ExePath),loop_point,do_repeat)
{
}

void BGM_Item::Load()
{
	Player.Load();
}

void BGM_Item::Play()
{
	Player.SetDoStop(false);
	player_thread_=boost::thread(&PlayThread::Run,&Player);
}

void BGM_Item::Play(BYTE volume)
{
	Play();
	SetVolume(volume);
}

void BGM_Item::Stop()
{
	Player.Stop();
	player_thread_.join();
	player_thread_.interrupt();
}

void BGM_Item::Pause()
{
	Player.Pause();
}

void BGM_Item::SetVolume(BYTE vol)
{
	Player.ChangeVolume(vol);
}

SE_Item::SE_Item(xtal::StringPtr path)
:Source(fsys::absolute(path->c_str(),kvarco::ExePath))
{
	Player=NULL;
	Loaded=false;
}

void SE_Item::Load()
{
	if(Loaded) return;

	Source.Load();
	Loaded	=true;
	Volume	=100;
}

void SE_Item::SetVolume(BYTE vol)
{
	Volume=vol;
}

void SE_Item::Play()
{
	if(!Loaded) return;

	Player=new SimplePlay_Thread(&Source,Volume);
	player_thread_=boost::thread(&SimplePlay_Thread::Run,Player);
}

void SE_Item::Play(BYTE volume)
{
	SetVolume(volume);
	Play();
}

void SE_Item::Stop()
{
	Player->Stop();
	player_thread_.join();
	player_thread_.interrupt();
}