#include "pch.h"
#include "Game.h"
#include "KVARCO.h"
#include "BaseActor.h"
#include "XtalHelper.h"
#include "LoadThread.h"

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
	DxLib::SetDrawArea(Area.left,Area.top,Area.right,Area.bottom);
}

void SetDrawArea(fRect Area)
{
	DxLib::SetDrawArea(
		static_cast<int>(Area.left),
		static_cast<int>(Area.top),
		static_cast<int>(Area.right),
		static_cast<int>(Area.bottom)
		);
}

void SetDrawArea_default()
{
	lRect area;
	area.left	=0;
	area.top	=0;
	area.right	=Game::GetInst()->Setting.WndWidth;
	area.bottom	=Game::GetInst()->Setting.WndHeight;

	kvarco::SetDrawArea(area);
}

//グラフィック関係
int	LoadGraph(xtal::String GrName,xtal::String RelaPath)
{
	std::string name=GrName.c_str();
	fsys::path path=fsys::absolute(RelaPath.c_str(),kvarco::ExePath);

	GrInfo grinfo;
	grinfo.GrHandle=Warning::Retry_LoadGraph(path.string());

	//""(NULL)の時はGrHadleを返すだけ
	if(name!="")
	{
		//サイズ獲得
		int w,h;
		GetGraphSize(grinfo.GrHandle,&w,&h);
#ifdef USE_SIZE_STRCT
		grinfo.Size.width	=w;
		grinfo.Size.height	=h;
#else
		grinfo.Size.left	=0;	grinfo.Size.top		=0;
		grinfo.Size.right	=w;	grinfo.Size.bottom	=h;
#endif
		kvarco::ImageNameList.insert(std::make_pair(GrName.c_str(),grinfo));
	}

	return grinfo.GrHandle;
}

GrInfo* GetGrInfo_p(xtal::String GrName)
{
	std::string name=GrName.c_str();

	boost::unordered_map<std::string,GrInfo>::iterator i=kvarco::ImageNameList.find(name);
	if(i!=kvarco::ImageNameList.end())
		return &((*i).second);
	return NULL;
}

GrInfo	GetGrInfo(xtal::String GrName)
{
	GrInfo* gr=GetGrInfo_p(GrName);
	if(gr!=NULL) return (*gr);

	GrInfo gr_null;	gr_null.GrHandle=NULL;
	return gr_null;
}

int GetGrHandle(xtal::String GrName)
{
	GrInfo* gr=GetGrInfo_p(GrName);
	if(gr!=NULL) return gr->GrHandle;
	return -1;
}

#ifdef USE_SIZE_STRCT
lSizePtrX GetGrSize(xtal::String GrName)
{
	GrInfo* gr=GetGrInfo_p(GrName);
	if(gr!=NULL) return xtal::xnew<lSize>(gr->Size);
	return lSizePtrX();
}
#else
lRectPtrX GetGrSize(xtal::String GrName)
{
	GrInfo* gr=GetGrInfo_p(GrName);
	if(gr!=NULL) return xtal::xnew<lRect>(gr->Size);
	return lRectPtrX();
}
#endif

//グラフィックハンドル指定型
int	LoadCutGraph_H(xtal::String NewName,int GrHandle,long x,long y,long w,long h)
{
	GrInfo grnew;
	grnew.GrHandle=DxLib::DerivationGraph(x,y,w,h,GrHandle);
	
	std::string name=NewName.c_str();
	if(name!="")
	{
#ifdef USE_SIZE_STRCT
		grnew.Size.width	=w;
		grnew.Size.height	=h;
#else
		grnew.Size.left	=0;	grnew.Size.top		=0;
		grnew.Size.right=w;	grnew.Size.bottom	=h;
#endif

		kvarco::ImageNameList.insert(make_pair(name,grnew));
	}

	return grnew.GrHandle;
}

void DeleteGraph_H(int GrHandle)
{
	if(GrHandle!=-1)	DxLib::DeleteGraph(GrHandle);
}

//グラフィックネーム指定型
int	LoadCutGraph_N(xtal::String NewName,xtal::String GrName,long x,long y,long w,long h)
{
	int GrHandle=GetGrHandle(GrName);

	if(GrHandle!=-1)
		return LoadCutGraph_H(NewName,GrHandle,x,y,w,h);
	return -1;
}

void DeleteGraph_N(xtal::String GrName)
{
	int GrHandle=GetGrHandle(GrName);
	if(GrHandle!=-1) kvarco::DeleteGraph_H(GrHandle);
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
		static_cast<int>(Rect.top),
		static_cast<int>(Rect.left),
		static_cast<int>(Rect.bottom),
		static_cast<int>(Rect.right),
		static_cast<int>(fill),color
		);
}

void DrawString( long x, long y, xtal::String str, int Color, int EdgeColor)
{
	DxLib::DrawString(x,y,str.c_str(),Color,EdgeColor);
}

//Tools
typedef std::string::size_type str_index;
xtal::StringPtr SplitOption(xtal::String src_x,xtal::String opt_x)
{
	std::string src_=src_x.c_str();
	std::string option=opt_x.c_str();

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

xtal::StringPtr SplitWords(xtal::String src_x)
{
	std::string src_=src_x.c_str();

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

	Xdef_fun(GetGrSize);

	Xdef_fun(LoadGraph);
	Xdef_fun(GetGrHandle);
	Xdef_fun(LoadCutGraph_H);
	Xdef_fun(DeleteGraph_H);
	Xdef_fun(LoadCutGraph_N);
	Xdef_fun(DeleteGraph_N);

	//描画関数
	//グラフィックハンドル指定型
	#define Xdef_param_(x) xtal::lib()->def(Xid(x),xtal::fun(&x)
	Xdef_param_(DrawGraph_H)			->param(4,Xid(trans),true)->param(5,Xid(call_alpha),false));
	Xdef_param_(DrawRotaGraph_H)		->param(5,Xid(trans),true)->param(6,Xid(call_alpha),false));
	Xdef_param_(DrawRotaGraph2_H)		->param(7,Xid(trans),true)->param(8,Xid(call_alpha),false));
	Xdef_param_(DrawGraphAlpha_H)		->param(5,Xid(trans),true));
	Xdef_param_(DrawRotaGraphAlpha_H)	->param(6,Xid(trans),true));
	Xdef_param_(DrawRotaGraphAlpha2_H)	->param(8,Xid(trans),true));
	Xdef_param_(DrawZoom_H)				->param(6,Xid(trans),true)->param(7,Xid(call_alpha),false));
	Xdef_param_(DrawZoomAlpha_H)		->param(7,Xid(trans),true));
	Xdef_param_(DrawRotaZoom_H)			->param(7,Xid(trans),true)->param(8,Xid(call_alpha),false));
	Xdef_param_(DrawRotaZoomAlpha_H)	->param(8,Xid(trans),true));

	//グラフィックネーム指定型
	Xdef_param_(DrawGraph_N)			->param(4,Xid(trans),true));
	Xdef_param_(DrawRotaGraph_N)		->param(5,Xid(trans),true));
	Xdef_param_(DrawRotaGraph2_N)		->param(7,Xid(trans),true));
	Xdef_param_(DrawGraphAlpha_N)		->param(5,Xid(trans),true));
	Xdef_param_(DrawRotaGraphAlpha_N)	->param(6,Xid(trans),true));
	Xdef_param_(DrawRotaGraphAlpha2_N)	->param(8,Xid(trans),true));
	Xdef_param_(DrawZoom_N)				->param(6,Xid(trans),true));
	Xdef_param_(DrawZoomAlpha_N)		->param(7,Xid(trans),true));
	Xdef_param_(DrawRotaZoom_N)			->param(7,Xid(trans),true));
	Xdef_param_(DrawRotaZoomAlpha_N)	->param(8,Xid(trans),true));
	#undef Xdef_param_

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
