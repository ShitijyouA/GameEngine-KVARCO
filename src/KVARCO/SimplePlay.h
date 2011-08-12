#pragma once
#include "OpenAL_Ogg.h"
#include <vector>

class AudioSource
{
	friend class SimplePlay;

	OggVorbis_File		OggFile;
	ALenum				Format;
	ALsizei				Frequency;
	std::vector<char>	Buffer;

public:
	AudioSource(const fsys::path& path);
	void Load();
	~AudioSource()
		{
			Buffer.clear();
		}
};

class SimplePlay
{
protected:
	AudioSource*	Source;

	ALuint		SourceID;
	ALuint		BufferID;

public:
	SimplePlay(AudioSource* source,char volume=100);

	void Play();
	virtual void Stop();
	virtual bool IsStopped();
};

class SimplePlay_Thread : public SimplePlay
{
	boost::mutex ThreadSync;
	boost::condition ThreadState;

	volatile bool DoStop;
	volatile bool Stopped;

public:
	SimplePlay_Thread(AudioSource* source,char volume=100)
		:SimplePlay(source,volume)
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

				if(DoStop) SimplePlay::Stop();
				Stopped=SimplePlay::IsStopped();

				alutSleep(0.0001f);
				ThreadState.notify_all();
			}
		}
};
