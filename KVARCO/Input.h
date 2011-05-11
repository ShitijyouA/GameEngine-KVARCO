#pragma once
#include "KVARCO.h"

const int MAX_BUTTON_NUM =40;
const int MAX_PLYER_NUM =16;

const int BORDER = 20;

#define INPUT_Z			0
#define INPUT_X			1
#define INPUT_C			2
#define INPUT_A			3
#define INPUT_S			4
#define INPUT_D			5
#define INPUT_Q			6
#define INPUT_W			7
#define INPUT_ESC		8
#define INPUT_SPEACE	9
#define INPUT_SHIFT		10

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