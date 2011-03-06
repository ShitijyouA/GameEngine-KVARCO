#pragma once

//標準ウィンドウサイズ
const DWORD StdWndWidth		=640;
const DWORD StdWndHeight	=480;

//SAFE_DELETE
template<typename T>
inline void SAFE_DELETE(T*& x) { delete x; x=NULL; }

//数学定数
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
	lRect	Size;
};

namespace KVARCO
{
	extern void Boot(string LoadFileList);		//ゲームの起動
	extern string ExePath;							//この実行ファイルがあるフォルダの絶対パス
	extern string GetFilePath(string s);
	extern string GetExePath();

	//Handle値
	extern DWORD MakeHandle();

	//入力関係
	extern DWORD GetKeyState(DWORD Key,int PlayerNo_=0);

	//描画範囲関係
	extern void SetDrawArea(lRect Area);
	extern void SetDrawArea(dRect Area);
	extern void SetDrawArea_default();

	//画像関係関数
	//GrNameもしくはNewNameが"NULL"(文字列)だと、グローバルテーブルに登録されない
	extern int	LoadGraph(xtal::String GrName,xtal::String RelaPath);	//ロード

	//グラフィックネームからの変換
	extern int		GetGrHandle(xtal::String GrName);	//グラフィックネームからグラフィックハンドルへの変換
	extern lRectPtr	GetGrSize(xtal::String GrName);
	extern GR_INFO	GetGrInfo(xtal::String GrName);
	extern GR_INFO*	GetGrInfo_p(xtal::String GrName);	//グラフィックテーブルにあるインスタンスのポインタ

		//グラフィックネーム指定型
	extern int	LoadCutGraph_H(xtal::String NewName,int GrHandle,long x,long y,long w,long h);		//ロード済みの画像から一部を切り出し、新しい画像として利用出来るようにする。切り出し元が削除されると利用できなくなる
	extern void	DeleteGraph_H(int GrHandle);	//画像削除

		//グラフィックハンドル指定型
	extern int	LoadCutGraph_N(xtal::String NewName,xtal::String GrName,long x,long y,long w,long h);		//ロード済みの画像から一部を切り出し、新しい画像として利用出来るようにする。切り出し元が削除されると利用できなくなる
	extern void	DeleteGraph_N(xtal::String GrName);	//画像削除

	//画像ブレンド設定(軽量化)
	extern inline void SetDrawBlendModeLight(BYTE mode,BYTE param);

	//描画関数
		//グラフィックハンドル指定型
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

		//グラフィックネーム指定型
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

	//文字列描画
	extern DWORD GetColorHandle(int r,int g,int b);
	extern void DrawString( long x, long y, xtal::String str, int Color, int EdgeColor = 0 );

	//図形描画
	extern void DrawLine(long x1,long y1,long x2,long y2, int color,bool thickness=false);
	extern void DrawdRect(dRect Rect, bool fill,int color);

	//ローディング
	extern void StartGraphLoading(xtal::AnyPtr LoadPathList);
	extern bool IsLoadingEnd(void);

	//便利関数
	extern xtal::StringPtr SplitOption(xtal::String Src,xtal::String Opt);	//[]内の指定オプションを取り出す。"-OPTION ARG"の形
	extern xtal::StringPtr SplitWords(xtal::String Src);	//[]の後の文字列を取り出す。前後の空白は削除される

	//乱数(mt19937使用)
	extern CRealRandom RealRandom;
	extern void RandSeed();
	extern float Rand(float min_,float max_);
	extern int RandInt(int min_,int max_);

	//Info
	extern float GetNowFPS();

	//バインド
	extern void bind();
	extern void GE_KeyCode_bind(xtal::ClassPtr it);

	//デバッグ
	extern FILE* LogFile;
	extern void OutputLog(const char* format_str,...);
	extern void DebugOut(xtal::StringPtr str);

	//終了
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