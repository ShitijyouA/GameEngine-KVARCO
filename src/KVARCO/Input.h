#pragma once
#include "Utillity.h"

const int MAX_BUTTON_NUM =40;
const int MAX_PLYER_NUM =16;

const int BORDER = 20;

enum
{
	INPUT_Z=0,
	INPUT_X,
	INPUT_C,
	INPUT_A,
	INPUT_S,
	INPUT_D,
	INPUT_Q,
	INPUT_W,
	INPUT_ESC,
	INPUT_SPEACE,
	INPUT_SHIFT,
};

class Input;
class InputState;

typedef Input* InputPtr;
typedef InputState* InputStatePtr;

class InputState {
public:
	DWORD Left,Up,Right,Down,Buttons[MAX_BUTTON_NUM];
	int AnalogX,AnalogY;

	InputState();
};

class Input
{
private:
	InputState States[MAX_PLYER_NUM];

public:
	void ClearState();

	void Update();
	const InputStatePtr GetState(int player)
	{
		return &States[player];
	}

	SINGLETON_PATTERN(Input,InputPtr)
};