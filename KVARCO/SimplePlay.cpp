#include "pch.h"
#include "SimplePlay.h"

CSource::CSource(string path)
{
	if(!OpenAL_Ogg::OpenALInited) OpenAL_Ogg::Init();

	//�����o�ϐ��̏�����
	Format		=0;
	Frequency	=0;

	//Ogg�t�@�C�����J��
	int open_res=ov_fopen(path.c_str(),&OggFile);
	if(open_res!=0)	throw string("Couldn't Open OggFile(at ov_open())");
	
	//�t�@�C���̐ݒ���擾
	vorbis_info* info=ov_info(&OggFile, -1);
	Format		=(info->channels==1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	Frequency	=info->rate;
}

void CSource::Load()
{
	//�ǂݍ���
	char data[BUFFER_SIZE];
	int section	=0;
	int bytes	=0;
	do
    {
		bytes =ov_read(&OggFile,data,BUFFER_SIZE,ENDIAN,sizeof(short),1,&section);

		if(bytes<0)
		{
			ov_clear(&OggFile);
			throw string("Error on ov_read()");
		}

		Buffer.insert(Buffer.end(),data,data+bytes);
    }
    while(bytes>0);

	ov_clear(&OggFile);
}

CSimplePlay::CSimplePlay(CSource* source,char volume)
{
	Source=source;

	//�\�[�X�ƃo�b�t�@�̍쐬
	alGenBuffers(1, &BufferID);
	alGenSources(1, &SourceID);

	//�Đ��̏����ݒ�
	alSource3f(SourceID, AL_POSITION,		0.0, 0.0, 0.0);
	alSource3f(SourceID, AL_VELOCITY,		0.0, 0.0, 0.0);
	alSource3f(SourceID, AL_DIRECTION,		0.0, 0.0, 0.0);
	alSourcef (SourceID, AL_ROLLOFF_FACTOR,	0.0          );
	alSourcei (SourceID, AL_SOURCE_RELATIVE,AL_TRUE      );
	alSourcef (SourceID, AL_GAIN,			volume*0.01f);

	//�o�b�t�@�ɍĐ��f�[�^���Z�b�g
	alBufferData(BufferID,Source->Format,&Source->Buffer[0],static_cast<ALsizei>(Source->Buffer.size()),Source->Frequency);

	//�\�[�X�Ƀo�b�t�@���Z�b�g
	alSourcei(SourceID, AL_BUFFER, BufferID);
}

void CSimplePlay::Play()
{
	alSourcePlay(SourceID);
}

void CSimplePlay::Stop()
{
	alSourcef(SourceID,AL_GAIN,0.0f);
	alSourceStop(SourceID);
}

bool CSimplePlay::IsStopped()
{
	ALint state;
	alGetSourcei(SourceID,AL_SOURCE_STATE,&state);

	if(state==AL_PLAYING) return false;
	else
	if(state==AL_STOPPED)
	{
		Stop();
		alDeleteBuffers(1,&BufferID);
		alDeleteSources(1,&SourceID);
	}

	return true;
}