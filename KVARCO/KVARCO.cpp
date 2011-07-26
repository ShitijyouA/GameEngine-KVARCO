#include "pch.h"
#include "Game.h"
#include "KVARCO.h"
#include "XtalHelper.h"
#include "LoadThread.h"
#include "Texture.h"
#include "Collision.h"

namespace kvarco
{

DWORD MakeHandle()
{
	static DWORD Num=0;
	Num++;
	return Num;
}

//入力関係
DWORD kvarco::GetKeyState(DWORD Key,int PlayerNo_)
{
	const InputStatePtr state=Input::GetInst()->GetState(PlayerNo_);
	if(Key<=KEY_DOWN)
	{
		switch(Key)
		{
			case KEY_LEFT:
				return state->Left;
			case KEY_UP:
				return state->Up;
			case KEY_RIGHT:
				return state->Right;
			case KEY_DOWN:
				return state->Down;
			break;
		}
	}
	else	return state->Buttons[Key-KEY_Z];

	return 0;
}

//描画範囲関係
void SetDrawArea(lRect Area)
{
	DxLib::SetDrawArea(Area.left_,Area.top_,Area.right_,Area.bottom_);
}

void SetDrawArea(fRect Area)
{
	DxLib::SetDrawArea(
		static_cast<int>(Area.left_),
		static_cast<int>(Area.top_),
		static_cast<int>(Area.right_),
		static_cast<int>(Area.bottom_)
		);
}

void SetDrawArea_default()
{
	lRect area;
	area.left_	=0;
	area.top_	=0;
	area.right_	=Game::GetInst()->Setting.WndWidth;
	area.bottom_=Game::GetInst()->Setting.WndHeight;

	kvarco::SetDrawArea(area);
}

void SetDrawBlendModeLight(BYTE mode,BYTE param)
{
	static BYTE NowMode=DX_BLENDMODE_NOBLEND;
	static BYTE NowParam=255;

	if(NowMode!=mode || NowParam!=param)
	{
		DxLib::SetDrawBlendMode(mode,param);
		NowMode=mode; NowParam=param;
	}
}

//描画
void DrawLine(long x1,long y1,long x2,long y2, int color,bool thickness)
{
	DxLib::DrawLine(x1,y1,x2,y2,color,(int )thickness);
}

void DrawfRect(fRect Rect, bool fill,int color)
{
	DxLib::DrawBox(
		static_cast<int>(Rect.top_),
		static_cast<int>(Rect.left_),
		static_cast<int>(Rect.bottom_),
		static_cast<int>(Rect.right_),
		static_cast<int>(fill),color
		);
}

void DrawString( long x, long y, xtal::StringPtr str, int Color, int EdgeColor)
{
	DxLib::DrawString(x,y,str->c_str(),Color,EdgeColor);
}

//Tools
typedef std::string::size_type str_index;
xtal::StringPtr SplitOption(const xtal::StringPtr& src_x,const xtal::StringPtr& opt_x)
{
	std::string src_	=src_x->c_str();
	std::string option	=opt_x->c_str();

	//[]の部分を切り出す
	str_index psh,pse;
	psh=src_.find("[");pse=src_.find_last_of("]");
	if(psh==std::string::npos || pse==std::string::npos) return "";
	std::string src=src_.substr(psh+1,pse-1);

	str_index ph,nph;
	str_index src_size=src.size();

	ph=src.find(option);				//指定された-PARAMの位置
	if(ph==std::string::npos) return xtal::StringPtr("");
	ph+=option.size();

	std::string arg;
	for(nph=ph; nph<src_size; nph++)
	{
		char c=src[nph];
		if(c=='-')	break;
		else
		if(c!=' ' && c!='\t' && c!='\n')
		{
			const char c=src[nph];
			arg.append(&c,1);
		}
	}

	return xtal::StringPtr(arg.c_str());
}

DWORD GetColorHandle(int r,int g,int b)
{
	return DxLib::GetColor(r,g,b);
}

xtal::StringPtr SplitWords(const xtal::StringPtr& src_x)
{
	std::string src_=src_x->c_str();

	str_index pse;
	pse=src_.find_last_of("]");
	if(pse==std::string::npos) return xtal::StringPtr("");

	std::string src=src_.substr(pse+1,std::string::npos);
	str_index src_size=src.size();

	//両端から
	str_index i,ii;
	for(i=0; i<src_size; i++)
	{
		char c=src[i];
		if((c!=' ' && c!='\t' && c!='\n')) break;
	}

	for(ii=src_size; ii>0; ii--)
	{
		char c=src[ii];
		if((c!=' ' && c!='\t' && c!='\n')) break;
	}

	return xtal::StringPtr(src.substr(i,ii).c_str());
}

//LoadingThread
void StartGraphLoading(xtal::AnyPtr LoadList)
{
	OutputLog("テクスチャ読み込み用のスレッドを起動");
	kvarco::LoadingThread=new LoadThread(LoadList);
}

bool IsLoadingEnd()
{
	if(kvarco::LoadingThread==NULL)			return true;
	else
	if(!kvarco::LoadingThread->IsEnded())	return false;

	OutputLog("スレッド終了。開放します");
	SAFE_DELETE(kvarco::LoadingThread);
	return true;
}

//Random
RealRandom RandomGenerator;

void RandSeed()
{
	RandomGenerator.Seed(static_cast<DWORD>(time(NULL)));
}

float Rand(float min_,float max_)
{
	return RandomGenerator.Random(min_,max_);
}

int RandInt(int min_,int max_)
{
	return static_cast<int>(
		RandomGenerator.Random(static_cast<float>(min_),static_cast<float>(max_))
	);
}

//Info
float GetNowFPS()
{
	return Game::GetInst()->GetRealFPS();
}

//Debug
FILE* LogFile;

void OutputLog(const char* format_str,...)
{
	if(LogFile==NULL)
		fopen_s(&LogFile,fsys::absolute(LOG_FILE_NAME,kvarco::ExePath).string().c_str(),"w");

	char buf[LOG_MAX_LENGTH];

	va_list VariablesList;
	va_start(VariablesList,format_str);
	
	vsprintf_s(buf,sizeof(buf),format_str,VariablesList);

	va_end(VariablesList);

	fprintf(LogFile,buf);
	fprintf(LogFile,"\n");
	fflush(LogFile);
}

void DebugOut(xtal::StringPtr str)
{
	OutputLog(str->c_str());
	OutputDebugString(str->c_str()); OutputDebugString("\n");
}

void Exit()
{
	Game::GetInst()->SetConfirmExit(true);
}

//Bind to Xtal
void bind()
{
	//クラスでなくネームスペースでもXdefマクロを使うためのゴリ押し
	#define Self	kvarco
	#define it		xtal::lib()

	Xdef_fun(MakeHandle);
	Xdef_fun(GetKeyState);

	//文字列描画
	Xdef_fun(GetColorHandle);
	Xdef_fun(DrawString);

	//図形描画
	Xdef_fun(DrawLine);
	Xdef_fun(DrawfRect);

	//Info
	Xdef_fun(GetNowFPS);
	Xdef_fun(GetGameSetting);

	//ローディング
	Xdef_fun(StartGraphLoading);
	Xdef_fun(IsLoadingEnd);
	Xdef_fun(DrawGraph);
	Xdef_fun(DrawGraphAllDisabled);
	Xdef_fun(DrawGraphEnabledFeatures);

	//便利関数
	Xdef_fun(SplitOption);
	Xdef_fun(SplitWords);
	
	//乱数(mt19937使用)
	Xdef_fun(RandSeed);
	Xdef_fun(Rand);
	Xdef_fun(RandInt);
	
	Xdef_fun(DebugOut);

	Xdef_fun(Exit);

	#undef Self
	#undef it
}

void GE_KeyCode_bind(xtal::ClassPtr it)
{
	#define int_t	xtal::int_t		//boostを使う環境ではint_tが曖昧になる(boost::int_tがあるため)
	#define Self					//GE_KeyCodeを設定してもいいが、警告がうるさい

	Xdef_const(KEY_LEFT);
	Xdef_const(KEY_UP);
	Xdef_const(KEY_RIGHT);
	Xdef_const(KEY_DOWN);

	Xdef_const(KEY_Z);
	Xdef_const(KEY_X);
	Xdef_const(KEY_C);
	Xdef_const(KEY_A);
	Xdef_const(KEY_S);
	Xdef_const(KEY_D);
	Xdef_const(KEY_Q);
	Xdef_const(KEY_W);
	Xdef_const(KEY_ESC);
	Xdef_const(KEY_SPACE);
	
	Xdef_const(KEY_F1);
	Xdef_const(KEY_F2);
	Xdef_const(KEY_F3);
	Xdef_const(KEY_F4);
	Xdef_const(KEY_F5);
	Xdef_const(KEY_F6);
	Xdef_const(KEY_F7);
	Xdef_const(KEY_F8);
	Xdef_const(KEY_F9);
	Xdef_const(KEY_F10);
	Xdef_const(KEY_F11);
	Xdef_const(KEY_F12);
	Xdef_const(KEY_F13);
	Xdef_const(KEY_F14);
	Xdef_const(KEY_F15);
	Xdef_const(KEY_F16);

	Xdef_const(KEY_SHIFT);
	Xdef_const(KEY_DELETE);
	Xdef_const(KEY_INSERT);

	#undef Self
	#undef int_t
}

} //namespace kvarco
