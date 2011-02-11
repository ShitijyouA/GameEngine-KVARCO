#include "pch.h"
#include "Input.h"

void CInput::Update()
{	
	int JoypadNum=DxLib::GetJoypadNum();
	
	char buf[256];
	GetHitKeyStateAll(buf);

	int get_input=DX_INPUT_KEY_PAD1;
	for(int i=0; i<JoypadNum || i<MAX_PLYER_NUM-1 ;i++)
	{	
		//方向キーの入力獲得
		int JoypadInput=GetJoypadInputState(get_input);

		if(buf[KEY_INPUT_LEFT]	|| (JoypadInput & PAD_INPUT_LEFT))	States[i].Left	++;
		else														States[i].Left	=0;
		if(buf[KEY_INPUT_UP]	|| (JoypadInput & PAD_INPUT_UP))	States[i].Up	++;
		else														States[i].Up	=0;
		if(buf[KEY_INPUT_RIGHT]	|| (JoypadInput & PAD_INPUT_RIGHT))	States[i].Right	++;
		else														States[i].Right	=0;
		if(buf[KEY_INPUT_DOWN]	|| (JoypadInput & PAD_INPUT_DOWN))	States[i].Down	++;
		else														States[i].Down	=0;

		#define INCLIMENT_OR_ZERO(flag_,index_)\
			if((flag_)!=0)	States[i].Buttons[(index_)]++;\
			else			States[i].Buttons[(index_)]=0;

		#define DX_KEY_INCLI_OR_ZERO(key_code_,index_)\
			INCLIMENT_OR_ZERO(JoypadInput & (key_code_),index_)

		//コマンドキーの入力獲得
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_A	,0);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_B	,1);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_C	,2);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_X	,3);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_Y	,4);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_Z	,5);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_L	,6);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_R	,7);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_START,8);
		DX_KEY_INCLI_OR_ZERO(PAD_INPUT_M	,9);
		INCLIMENT_OR_ZERO(buf[KEY_INPUT_LSHIFT] || buf[KEY_INPUT_RSHIFT],10);

		for(int ii=0; ii<16; ii++)
		{
			INCLIMENT_OR_ZERO(GetKeyState(ii+VK_F1)&0x80,ii+11)
		}

		INCLIMENT_OR_ZERO(GetKeyState(VK_DELETE)&0x80,31)
		INCLIMENT_OR_ZERO(GetKeyState(VK_INSERT)&0x80,32)
		
#undef INCLIMENT_OR_ZERO
#undef DX_KEY_INCLI_OR_ZERO

		//アナログレバーの数値獲得

		int bufX=States[i].AnalogX;
		int bufY=States[i].AnalogY;

		GetJoypadAnalogInput(&bufX,&bufY,i);
		
		//ゆとり
		if(abs(bufX)<BORDER)	bufX=0;
		if(abs(bufY)<BORDER)	bufY=0;

		get_input=i+1;
	}
}

//InputPtr CInput::Inst=NULL;
InputPtr CInput::GetInst()
{
	//if(Inst==NULL) Inst=new CInput;
	static CInput Inst;
	return &Inst;
}

CInputState::CInputState()
{
	Left=Up=Right=Down=false;
	AnalogX=AnalogY=0;
	for(int i=0; i>MAX_BUTTON_NUM; i++)		Buttons[i]=false;
}