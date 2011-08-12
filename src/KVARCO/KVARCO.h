#pragma once
#include "Config.h"

//標準ウィンドウサイズ
const DWORD StdWndWidth		=640;
const DWORD StdWndHeight	=480;

//SAFE_DELETE
template<typename T>
inline void SAFE_DELETE(T*& x) { delete x; x=NULL; }

const float D2R			=0.017453292519943;
const float R2D			=57.29577951308232;

const fsys::path	LOG_FILE_NAME	="kvarco_log.txt";
const DWORD			LOG_MAX_LENGTH	=256;

namespace kvarco
{

//Handle値
extern DWORD MakeHandle();

//この実行ファイルがあるフォルダの絶対パス
extern fsys::path ExePath;

}

#include "GeoStruct.h"
#include "Utillity.h"
#include "RealRandom.h"
#include "Input.h"
#include "CharacterParam.h"

namespace kvarco
{
	extern void Boot(const fsys::path& LoadFileList);	//ゲームの起動
	extern std::string GetFilePath(std::string s);
	extern std::string GetExePath();

	//入力関係
	extern DWORD GetKeyState(DWORD Key,int PlayerNo_=0);

	//描画範囲関係
	extern void SetDrawArea(lRect Area);
	extern void SetDrawArea(fRect Area);
	extern void SetDrawArea_default();

	//画像ブレンド設定(軽量)
	extern inline void SetDrawBlendModeLight(BYTE mode,BYTE param);

	//描画関数
	//CharacterParam指定型
	extern void DrawGraph(const CharParamPtrX& char_param);	/// char_paramの該当するメンバがデフォルト値でない機能だけ有効
	extern void DrawGraphAllDisabled(const CharParamPtrX& char_param);	/// 全部の機能が無効なDrawGraphEnabledFeatures()と同じ
	extern void DrawGraphEnabledFeatures(const CharParamPtrX& char_param,bool enable_alpha=true,bool enable_rota=false,bool enable_zoom=false);

	//文字列描画
	extern DWORD GetColorHandle(int r,int g,int b);
	extern void DrawString( long x, long y, xtal::StringPtr str, int Color, int EdgeColor = 0 );

	//図形描画
	extern void DrawLine(long x1,long y1,long x2,long y2, int color,bool thickness=false);
	extern void DrawfRect(fRect Rect, bool fill,int color);

	//ローディング
	extern void StartGraphLoading(xtal::AnyPtr LoadPathList);
	extern bool IsLoadingEnd(void);

	//便利関数
	extern xtal::StringPtr SplitOption(const xtal::StringPtr& Src,const xtal::StringPtr& Opt);	//[]内の指定オプションを取り出す。"-OPTION ARG"の形
	extern xtal::StringPtr SplitWords(const xtal::StringPtr& Src);								//[]の後の文字列を取り出す。前後の空白は削除される

	//乱数(mt19937使用)
	extern RealRandom RandomGenerator;
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

namespace detail
{
	extern void DrawGraphRaw(int GrHandle,long x,long y,bool Trans,bool call_alpha);
	extern void DrawRotaGraph(int GrHandle,float Angle,long cx,long cy,bool Trans,bool call_alpha);
	extern void DrawGraphAlpha(int GrHandle,long x,long y,int Alpha,bool Trans);
	extern void DrawRotaGraphAlpha(int GrHandle,float Angle,long cx,long cy,int Alpha,bool Trans);
	extern void DrawZoom(const CharParamPtrX& char_param,bool trans,bool call_alpha);
	extern void DrawZoomAlpha(const CharParamPtrX& char_param);
	extern void DrawRotaZoom(const CharParamPtrX& char_param,bool Trans,bool call_alpha);
	extern void DrawRotaZoomAlpha(const CharParamPtrX& char_param,bool Trans);
}

}

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
