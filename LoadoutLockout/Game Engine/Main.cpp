#include "stdafx.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>
#include "stdio.h"
#include "BasicEngine/GameEngine.h"
#include "BasicEngine/State/IntroState.h"

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc, char* args[])
{
	
	//INIT
	GameEngine* game = new GameEngine();
	game->Init();
	game->ChangeState(IntroState::GetInstance());

	// game loop
	while (game->IsRunning())
	{
		game->HandleInputs();
		game->Update();
		game->Draw();
	}

	// cleanup 
	game->CleanUp();

	return 0;
}