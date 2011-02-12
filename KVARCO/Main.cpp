#include "pch.h"
#include "Game.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	KVARCO::Boot("setting.ini");

	Game->Main();

	return 0 ;		// ソフトの終了
}
