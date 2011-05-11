#pragma once
#include "OpenAL_Ogg.h"
#include <vector>

class CSource
{
	friend class CSimplePlay;

	OggVorbis_File	OggFile;
	ALenum			Format;
	ALsizei			Frequency;
	vector<char>	Buffer;

public:
	CSource(const fsys::path& path);
	void Load();
	~CSource()
		{
			Buffer.clear();
		}
};

class CSimplePlay
{
protected:
	CSource*	Source;

	ALuint		SourceID;
	ALuint		BufferID;

public:
	CSimplePlay(CSource* source,char volume=100);

	void Play();
	virtual void Stop();
	virtual bool IsStopped();
};

class CSimplePlay_Thread : public CSimplePlay
{
	boost::mutex ThreadSync;
	boost::condition ThreadState;

	volatile bool DoStop;
	volatile bool Stopped;

public:
	CSimplePlay_Thread(CSource* source,char volume=100)
		:CSimplePlay(source,volume)
		{
			DoStop	=false;
			Stopped	=false;
		}

	void Stop()
		{
			DoStop=true;
		}

	bool IsStopped()
		{
			return Stopped;
		}

	void Run()
		{
			Play();
			while(!IsStopped())
			{
				boost::mutex::scoped_lock look(ThreadSync);

				if(DoStop) CSimplePlay::Stop();
				Stopped=CSimplePlay::IsStopped();

				alutSleep(0.0001f);
				ThreadState.notify_all();
			}
		}
};
