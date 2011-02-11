#include "pch.h"
#include "OpenAL_Ogg.h"
#include <iostream>
using namespace std;

//OpenAL_Ogg
bool OpenAL_Ogg::OpenALInited=false;
void OpenAL_Ogg::Init()
{
	alutInit(NULL,NULL);
	OpenALInited=true;
}

void OpenAL_Ogg::UnInit()
{	
	alutExit();
	OpenALInited=false;
}

void OpenAL_Ogg::Check(string plus)
{
#ifndef NOT_CHECK
	if(!OpenALInited) return;
	ALenum error_code=alutGetError();
	if(tmp_err!=AL_NO_ERROR)
		throw format("Error %1% : %2%")% plus% alutGetErrorString(error_code);
#endif
}

//COpenAL_Ogg_Stream
COpenAL_Ogg_Stream::COpenAL_Ogg_Stream(string path,DWORD loop_point,bool repeat)
{
	if(!OpenAL_Ogg::OpenALInited) OpenAL_Ogg::Init();

	//メンバ変数の初期化
	for(int i=0; i<BUFFER_NUM; i++)	Buffers[i]=0;
	SourceID	=0;
	Format		=0;
	Frequency	=0;

	//Oggファイルを開く
	int open_res=ov_fopen(path.c_str(),&OggFile);
	if(open_res!=0)	throw string("Couldn't Open OggFile(at ov_open())");
	
	//ファイルの設定を取得
	vorbis_info* info=ov_info(&OggFile, -1);
	Format		=(info->channels==1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	Frequency	=info->rate;

	//バッファ・ソースの作成
	alGenBuffers(BUFFER_NUM,Buffers);
	OpenAL_Ogg::Check("on alGenBuffers()");
	alGenSources(1, &SourceID);
	OpenAL_Ogg::Check("on alGenSources()");

	DoRepeat	=repeat;
	LoopPoint	=loop_point;

	//再生の初期設定
	alSource3f(SourceID, AL_POSITION,		0.0, 0.0, 0.0);
	alSource3f(SourceID, AL_VELOCITY,		0.0, 0.0, 0.0);
	alSource3f(SourceID, AL_DIRECTION,		0.0, 0.0, 0.0);
	alSourcef (SourceID, AL_ROLLOFF_FACTOR,	0.0          );
	alSourcei (SourceID, AL_SOURCE_RELATIVE,AL_TRUE      );
}

//待機なんかで満足してんじゃねぇ
//命をかけてたった一つのOggファイルを再生してえんじゃないのかよ?
//だったら再生は全然終わってねえ!! 始まってすらいねえ!!
//ちっとぐらい長い処理時間で絶望してんじゃねえよ!!
//手を伸ばせば届くんだ
//いい加減に始めようぜ。OpenAL!

bool COpenAL_Ogg_Stream::Stream(ALuint buffers)
{
	char data[BUFFER_SIZE];
	int  size	=0;
	int  section=0;
	int  res	=0;

	while(size<BUFFER_SIZE)
	{
		res=ov_read(&OggFile, data + size, BUFFER_SIZE - size, ENDIAN, sizeof(short), 1, &section);

		if(res>0)		size+=res;
		else
		{
			if(res<0)		throw string("Error on ov_read()");
			else
			if(DoRepeat)	ov_pcm_seek(&OggFile,LoopPoint);
			else			break;
		}
	}
	
	alBufferData(buffers, Format, data, size, Frequency);
	OpenAL_Ogg::Check("on alBufferData()");
	
	if(size==0) return true;
	return false;
}

bool COpenAL_Ogg_Stream::Update()
{
	int processed	=0;
	bool res		=false;

	alGetSourcei(SourceID, AL_BUFFERS_PROCESSED, &processed);

	while(processed--)
	{
		ALuint buffers;

		alSourceUnqueueBuffers(SourceID, 1, &buffers);
		OpenAL_Ogg::Check("on alSourceUnqueueBuffers()");

		res=Stream(buffers);

		alSourceQueueBuffers(SourceID, 1, &buffers);
		OpenAL_Ogg::Check("on alSourceQueueBuffers()");
	}

	return res;
}

//終了するならtrue
void COpenAL_Ogg_Stream::PlayBack()
{
	if(PlayingNow()) return;
	    
	for(int i=0; i<BUFFER_NUM; i++)
		if(Stream(Buffers[i])) throw string("Can't more read");

	alSourceQueueBuffers(SourceID,BUFFER_NUM,Buffers);
	OpenAL_Ogg::Check("on PlayBack_alSourceQueueBuffers()");
	alSourcePlay(SourceID);
}

COpenAL_Ogg_Stream::~COpenAL_Ogg_Stream()
{
	alSourceStop(SourceID);

	//全てキューから取り出し削除
	int num=0;
	while (alGetSourcei(SourceID, AL_BUFFERS_PROCESSED, &num), num>0)
	{
		alSourceUnqueueBuffers(SourceID, 1, Buffers);
		alDeleteBuffers(1, Buffers);
	}

    ov_clear(&OggFile);
	alDeleteSources(1, &SourceID);
}