#pragma once
#include "OpenAL_Ogg.h"
using boost::timed_mutex;

class PlayThread : public OpenAL_Ogg_Stream
{
	volatile BYTE Volume;
	volatile BYTE PrevVolume;

	volatile bool DoStop;

	boost::timed_mutex	ThreadSync;
	boost::condition	ThreadState;

public:
	PlayThread(fsys::path& path,DWORD loop_point,bool repeat)
			:OpenAL_Ogg_Stream(path,loop_point,repeat)
		{
			Volume=0;	PrevVolume=0;
			DoStop=false;
		}

	void Load()
		{
			OpenAL_Ogg_Stream::Load();
		}

	void Run()
		{
			while(!DoStop && !StreamingPlay())
			{
				const posix_time::milliseconds wait_time(5);
				timed_mutex::scoped_timed_lock lock(ThreadSync,wait_time);
				ThreadState.timed_wait(lock,wait_time);

				if(PrevVolume!=Volume)
				{
					OpenAL_Ogg_Stream::ChangeVolume(Volume);
					PrevVolume=Volume;
				}

				alutSleep(0.001f);
				ThreadState.notify_all();
			}

			OpenAL_Ogg_Stream::Stop();
		}

	void ChangeVolume(BYTE volume)
		{
			timed_mutex::scoped_timed_lock lock(ThreadSync,posix_time::milliseconds(5));

			Volume=volume;

			ThreadState.notify_all();
		}

	void SetDoStop(bool do_stop)
		{
			timed_mutex::scoped_timed_lock lock(ThreadSync,posix_time::milliseconds(5));

			DoStop=do_stop;

			ThreadState.notify_all();
		}

	void Stop()
		{
			SetDoStop(true);
		}
};
