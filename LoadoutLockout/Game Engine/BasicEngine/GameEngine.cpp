#include "stdafx.h"
#include "GameEngine.h"
#include "Managers/FileManager.h"
#include "State/GameState.h"
#include "Managers/FrameRate_Controller.h"
#include "Managers/MemoryManager.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameEngine.cpp
Purpose: The Game Engine implementation at the highest level. This uses
The various managers and tracks the global variables required including 
the current game state. 
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/09/2018
- End Header --------------------------------------------------------*/
Audio_Manager* pAM = nullptr;
InputFacade* pIF;
GraphicsManager * pGfx = nullptr;
FrameRate_Controller* pFRC;
GameObjectFactory *pGOF;
MemoryManager* pMM;

GameState* current_state;

void GameEngine::ChangeState(GameState * state, std::string str)
{
	current_state = state;
	if (!states.empty()) {
		states.top()->CleanUp();
		states.pop();
	}

	// store and init the new state
	states.push(state);
	states.top()->Init(this, str);
	
}

void GameEngine::PushState(GameState * state, std::string str)
{
	// pause current state
	if (!states.empty()) {
		states.top()->Pause();
	}

	// store and init the new state
	states.push(state);
	current_state = states.top();
	states.top()->Init(this,str); 
	
}

void GameEngine::PopState()
{
	// cleanup the current state
	if (!states.empty()) {
		states.top()->CleanUp();
		states.pop();
	}

	// resume previous state
	if (states.size() > 0) {
		current_state = states.top();
		states.top()->Resume();
		
	}
}

void GameEngine::Reset()
{
	PopState();
	current_state->Init(this);
}

void GameEngine::Init()
{
	skip_intro = false;
	int error = 0;
	sfx = current_sfx = 1.f;
	master = current_master = 1.f;
	music = current_music = 1.f;
	mute_master = mute_sfx = mute_music = false;
#ifdef _DEBUG
		if (AllocConsole()) {
			FILE* file;

			freopen_s(&file, "CONOUT$", "wt", stdout);
			freopen_s(&file, "CONOUT$", "wt", stderr);
			freopen_s(&file, "CONOUT$", "wt", stdin);

			SetConsoleTitle(L"Debug: Lockout Loadout Console");
		}
#endif
		std::cout << "SETTING UP SDL" << std::endl;
	// Initialize SDL
	if ((error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER| SDL_INIT_JOYSTICK)) < 0)// for the controller
	{
		std::cerr << "ENCOUNTERED ERROR SETTING UP SDL" << std::endl;
		throw;
	}
	//SDL_JoystickEventState(SDL_ENABLE);
	std::cout << "SET UP SDL" << std::endl;
	SDL_GL_LoadLibrary(NULL);
	std::cout << "Loading OPENGL" << std::endl;
	{
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	}

	std::string config_str = FileManager::GetJSONString("config.json", CONFIG_FILE);
	json config = json::parse(config_str);
	std::cout << "Read config file" << std::endl;
	std::string name = config["name"];
	pWindow = SDL_CreateWindow(name.c_str(),	// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		config["width"],								// width, in pixels
		config["height"],								// height, in pixels
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	full_screen = config["full-screen"];
	if (full_screen) {
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		//SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	int w, h;
	SDL_GetWindowSize(pWindow, &w, &h);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouseInWindow(pWindow, w / 2, h / 2);
	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		std::cerr << "ENCOUNTERED ERROR SETTING UP SDL WINDOW" << std::endl;
		// In the event that the window could not be made...
		throw;
	}
	//*
	opengl_context = SDL_GL_CreateContext(pWindow);
	if (opengl_context == NULL)
		throw;
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		throw;
	SDL_GL_SetSwapInterval(1);

	pWindowSurface = SDL_GetWindowSurface(pWindow);
	if (NULL == pWindowSurface) {
		throw;
	}
	pIF = new InputFacade();

	pGfx = new GraphicsManager(pWindow, opengl_context, true);

	pFRC = new FrameRate_Controller(config["fps"]);
	pMM = new MemoryManager();
	GameObjectFactory::Init();
	run_app = true;
}

void GameEngine::CleanUp()
{
	while (!states.empty()) {
		states.top()->CleanUp();
		states.pop();
	}
	current_state = nullptr;
	GameObjectFactory::Destory();
	delete pIF ;
	delete pGfx;
	delete pFRC;
	if(pAM != nullptr)
		delete pAM;
	delete pMM;

	SDL_DestroyWindow(pWindow);
	SDL_GL_DeleteContext(opengl_context);

	SDL_LogDebug(SDL_LOG_PRIORITY_DEBUG, "Finished cleanup.\n");

	// Quit SDL subsystems
	SDL_Quit();
	SDL_LogDebug(SDL_LOG_PRIORITY_DEBUG, "Finished quit.\n");
}

void GameEngine::HandleInputs()
{
	if (!states.empty()) {
		states.top()->HandleInput();
	}
}

void GameEngine::Update()
{
	if (!states.empty()) {
		states.top()->Update();
	}
}

void GameEngine::Draw()
{
	if (!states.empty()) {
		states.top()->Draw();
	}
}
