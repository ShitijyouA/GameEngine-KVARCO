#include "pch.h"
#include "Game.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	Game::Boot("setting.ini");
	Game::GetInst()->Main();

	return 0 ;		// ソフトの終了
}
