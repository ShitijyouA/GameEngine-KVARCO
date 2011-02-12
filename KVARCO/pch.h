// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。


#ifdef _DEBUG
	#pragma comment(lib,"xtallib_d.lib")
#else
	#pragma comment(lib,"xtallib.lib")
#endif

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include "OpenAL_Ogg_Default.h"
#include "LibraryHeader.h"
#include "GeoStruct.h"