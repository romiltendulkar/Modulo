#ifndef __GAME_ENGINE__
#define __GAME_ENGINE__
#include <stdafx.h>
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameEngin.h
Purpose: The Game Engine. This defines the functions that the main
function will use. This will provide more of an abstraction so this
can be utilized in more games.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/25/2018
- End Header --------------------------------------------------------*/

class GameState;
class GameEngine {
public:
	void Init();
	void CleanUp();

	bool IsRunning() { return run_app; }
	void Quit() { run_app = false; }

	void ChangeState(GameState* state, std::string str = "");
	void PushState(GameState* state, std::string str ="");
	void PopState();
	void Reset();
	int StateSize() { return states.size(); }

	void HandleInputs();
	void Update();
	void Draw();
	SDL_Window* pWindow;
	SDL_Surface* pWindowSurface;
	SDL_GLContext opengl_context;
	float current_sfx, current_master, current_music;
	float sfx, master, music;
	bool full_screen;
	bool skip_intro;
	bool mute_sfx, mute_master, mute_music;

private:
	std::stack<GameState*> states;
	bool run_app;
	
};


#endif