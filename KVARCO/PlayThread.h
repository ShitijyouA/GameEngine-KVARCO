#pragma once
#include "OpenAL_Ogg.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>

using namespace boost;

class CPlayThread : public COpenAL_Ogg_Stream
{
	volatile BYTE Volume;
	volatile BYTE PrevVolume;

	volatile bool DoStop;

	mutex ThreadSync;
	condition ThreadState;

public:
	CPlayThread(string path,DWORD loop_point,bool repeat)
			:COpenAL_Ogg_Stream(path,loop_point,repeat)
		{
			Volume=0;	PrevVolume=0;
			DoStop=false;
		}

	void Load()
	{
		COpenAL_Ogg_Stream::Load();
	}

	void Run()
		{
			while(!DoStop && !StreamingPlay())
			{
				mutex::scoped_lock look(ThreadSync);

				if(PrevVolume!=Volume)
				{
					COpenAL_Ogg_Stream::ChangeVolume(Volume);
					PrevVolume=Volume;
				}

				alutSleep(0.0001f);
				ThreadState.notify_all();
			}
			COpenAL_Ogg_Stream::Stop();
		}

	void ChangeVolume(BYTE volume)
		{
			mutex::scoped_lock look(ThreadSync);
			ThreadState.wait(ThreadSync);

			Volume=volume;

			ThreadState.notify_all();
		}

	void Stop()
		{
			DoStop=true;
		}
};