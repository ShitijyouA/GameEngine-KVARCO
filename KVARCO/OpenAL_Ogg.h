#pragma once
#include "OpenAL_Ogg_Default.h"
#include <cassert>

//def switch
#define NOT_CHECK	//Check関数でのエラーチェックをoffに
					//(エラーチェックで例外が発生する可能性がある)

const DWORD BUFFER_NUM	=2;
const DWORD BUFFER_SIZE	=4096*4;

namespace OpenAL_Ogg
{
	void Init();
	void UnInit();
	void Check(std::string plus);
	extern bool OpenALInited;
}

class OpenAL_Ogg_Stream
{
	friend class PlayThread;

	OggVorbis_File	OggFile;

	ALuint		Buffers[BUFFER_NUM];
	ALuint		SourceID;
	ALenum		Format;
	ALsizei		Frequency;

	bool Stream(ALuint buffer);
	void PlayBack();
	bool Update();

	fsys::path	Path;
	bool		DoRepeat;
	DWORD		LoopPoint;
	bool		Loaded;

public:
	OpenAL_Ogg_Stream(fsys::path& abs_path,DWORD loop_point,bool repeat);
	~OpenAL_Ogg_Stream();

	void Load();

	//再生
	bool StreamingPlay()			//再生終了でtrueを返す
		{
			if(Update())		return true;
			if(!PlayingNow())	PlayBack();
			return false;
		}


	//Getter&Setter
	void ChangeVolume(BYTE volume)	//Max:100
		{
			if(volume>100) return;
			alSourcef(SourceID,AL_GAIN,(float )(volume*0.01));
		}

	void ChangePosition(float x,float y,float z)
		{ alSource3f(SourceID,AL_POSITION,x,y,z); }

	double GetTortalTime()
		{ return ov_time_total(&OggFile,-1); }

	double GetNowTime()
		{ return ov_time_tell(&OggFile); }

	bool PlayingNow()
		{
			ALenum state;
			alGetSourcei(SourceID, AL_SOURCE_STATE, &state);

			return (state==AL_PLAYING)||(state==AL_PAUSED);
		}

	virtual void Stop()
		{
			alSourcef(SourceID,AL_GAIN,0.0f);
			alSourceStop(SourceID);
			ov_pcm_seek(&OggFile,0);
		}

	virtual void Pause()
	{
		ALenum state;
		alGetSourcei(SourceID, AL_SOURCE_STATE, &state);

		if(state==AL_PLAYING)	alSourcePause(SourceID);
		else
		if(state==AL_PAUSED )	alSourcePlay(SourceID);
	}

	ALuint GetSourceID()
		{	return SourceID;	}
};