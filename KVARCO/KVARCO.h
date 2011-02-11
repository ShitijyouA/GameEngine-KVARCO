#pragma once

//�W���E�B���h�E�T�C�Y
const DWORD StdWndWidth		=640;
const DWORD StdWndHeight	=480;

//SAFE_DELETE
template<typename T>
inline void SAFE_DELETE(T*& x) { delete x; x=NULL; }

//���w�萔
#ifndef M_PI
const float M_PI		=3.141592653589793;
const float M_PI2		=6.283185307179586;
const float M_SQRT1_2	=0.707106781186547;
#endif

const float D2R			=0.017453292519943;
const float R2D			=57.29577951308232;

const string	LOG_FILE_NAME	="kvarco_log.txt";
const DWORD		LOG_MAX_LENGTH	=256;

#include "GeoStruct.h"
#include "Utillity.h"
#include "RealRandom.h"
#include "Warning.h"
#include "Input.h"
#include "Collision.h"

struct GR_INFO
{
	int		GrHandle;
	lRECT	Size;
};

namespace KVARCO
{
	extern void Boot(string LoadFileList);		//�Q�[���̋N��
	extern string ExePath;							//���̎��s�t�@�C��������t�H���_�̐�΃p�X
	extern string GetFilePath(string s);
	extern string GetExePath();

	//Handle�l
	extern DWORD MakeHandle();

	//���͊֌W
	extern DWORD GetKeyState(DWORD Key,int PlayerNo_=0);

	//�`��͈͊֌W
	extern void SetDrawArea(lRECT Area);
	extern void SetDrawArea(dRECT Area);
	extern void SetDrawArea_default();

	//�摜�֌W�֐�
	//GrName��������NewName��"NULL"(������)���ƁA�O���[�o���e�[�u���ɓo�^����Ȃ�
	extern int	LoadGraph(xtal::String GrName,xtal::String RelaPath);	//���[�h

	//�O���t�B�b�N�l�[������̕ϊ�
	extern int		GetGrHandle(xtal::String GrName);	//�O���t�B�b�N�l�[������O���t�B�b�N�n���h���ւ̕ϊ�
	extern lRECTPtr	GetGrSize(xtal::String GrName);
	extern GR_INFO	GetGrInfo(xtal::String GrName);
	extern GR_INFO*	GetGrInfo_p(xtal::String GrName);	//�O���t�B�b�N�e�[�u���ɂ���C���X�^���X�̃|�C���^

		//�O���t�B�b�N�l�[���w��^
	extern int	LoadCutGraph_H(xtal::String NewName,int GrHandle,long x,long y,long w,long h);		//���[�h�ς݂̉摜����ꕔ��؂�o���A�V�����摜�Ƃ��ė��p�o����悤�ɂ���B�؂�o�������폜�����Ɨ��p�ł��Ȃ��Ȃ�
	extern void	DeleteGraph_H(int GrHandle);	//�摜�폜

		//�O���t�B�b�N�n���h���w��^
	extern int	LoadCutGraph_N(xtal::String NewName,xtal::String GrName,long x,long y,long w,long h);		//���[�h�ς݂̉摜����ꕔ��؂�o���A�V�����摜�Ƃ��ė��p�o����悤�ɂ���B�؂�o�������폜�����Ɨ��p�ł��Ȃ��Ȃ�
	extern void	DeleteGraph_N(xtal::String GrName);	//�摜�폜

	//�摜�u�����h�ݒ�(�y�ʉ�)
	extern inline void SetDrawBlendModeLight(BYTE mode,BYTE param);

	//�`��֐�
		//�O���t�B�b�N�n���h���w��^
	extern void DrawGraph_H(int GrHandle,long x,long y,bool Trans=true,bool call_alpha=false);
	extern void DrawRotaGraph_H(int GrHandle,float Angle,long cx,long cy,bool Trans=true,bool call_alpha=false);
	extern void DrawRotaGraph2_H(int GrHandle,float Angle,long x,long y,long cx,long cy,bool Trans=true,bool call_alpha=false);
	extern void DrawGraphAlpha_H(int GrHandle,long x,long y,int Alpha,bool Trans=true);
	extern void DrawRotaGraphAlpha_H(int GrHandle,float Angle,long cx,long cy,int Alpha,bool Trans=true);
	extern void DrawRotaGraphAlpha2_H(int GrHandle,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans=true);

	extern void DrawZoom_H(int GrHandle,float ZoomRateX,float ZoomRateY,long x,long y,bool Trans=true,bool call_alpha=false);
	extern void DrawZoomAlpha_H(int GrHandle,float ZoomRateX,float ZoomRateY,long x,long y,int Alpha,bool Trans=true);
	extern void DrawRotaZoom_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,bool Trans=true,bool call_alpha=false);
	//extern void DrawRotaZoom2_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,bool Trans=true);
	extern void DrawRotaZoomAlpha_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,int Alpha,bool Trans=true);
	//extern void DrawRotaZoomAlpha2_H(int GrHandle,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans=true);

		//�O���t�B�b�N�l�[���w��^
	extern void DrawGraph_N(xtal::String GrName,long x,long y,bool Trans=true);
	extern void DrawRotaGraph_N(xtal::String GrName,float Angle,long cx,long cy,bool Trans=true);
	extern void DrawRotaGraph2_N(xtal::String GrName,float Angle,long x,long y,long cx,long cy,bool Trans=true);
	extern void DrawGraphAlpha_N(xtal::String GrName,long x,long y,int Alpha,bool Trans=true);
	extern void DrawRotaGraphAlpha_N(xtal::String GrName,float Angle,long cx,long cy,int Alpha,bool Trans=true);
	extern void DrawRotaGraphAlpha2_N(xtal::String GrName,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans=true);

	extern void DrawZoom_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,long x,long y,bool Trans=true);
	extern void DrawZoomAlpha_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,long x,long y,int Alpha,bool Trans=true);
	extern void DrawRotaZoom_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,bool Trans=true);
	//extern void DrawRotaZoom2_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,bool Trans=true);
	extern void DrawRotaZoomAlpha_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long cx,long cy,int Alpha,bool Trans=true);
	//extern void DrawRotaZoomAlpha2_N(xtal::String GrName,float ZoomRateX,float ZoomRateY,float Angle,long x,long y,long cx,long cy,int Alpha,bool Trans=true);

	//������`��
	extern DWORD GetColorHandle(int r,int g,int b);
	extern void DrawString( long x, long y, xtal::String str, int Color, int EdgeColor = 0 );

	//�}�`�`��
	extern void DrawLine(long x1,long y1,long x2,long y2, int color,bool thickness=false);
	extern void DrawdRECT(dRECT rect, bool fill,int color);

	//���[�f�B���O
	extern void StartGraphLoading(xtal::AnyPtr LoadPathList);
	extern bool IsLoadingEnd(void);

	//�֗��֐�
	extern xtal::StringPtr SplitOption(xtal::String Src,xtal::String Opt);	//[]���̎w��I�v�V���������o���B"-OPTION ARG"�̌`
	extern xtal::StringPtr SplitWords(xtal::String Src);	//[]�̌�̕���������o���B�O��̋󔒂͍폜�����

	//����(mt19937�g�p)
	extern CRealRandom RealRandom;
	extern void RandSeed();
	extern float Rand(float min_,float max_);
	extern int RandInt(int min_,int max_);

	//Info
	extern float GetNowFPS();

	//�o�C���h
	extern void bind();
	extern void GE_KeyCode_bind(xtal::ClassPtr it);

	//�f�o�b�O
	extern FILE* LogFile;
	extern void OutputLog(const char* format_str,...);
	extern void DebugOut(xtal::StringPtr str);

	//�I��
	extern void Exit();
};

enum GE_KeyCode
{
	KEY_LEFT=0,
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,

	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_Q,
	KEY_W,
	KEY_ESC,
	KEY_SPACE,
	KEY_SHIFT,

	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,

	KEY_DELETE,
	KEY_INSERT,
};