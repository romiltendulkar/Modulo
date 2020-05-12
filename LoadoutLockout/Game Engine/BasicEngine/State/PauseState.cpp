#include "stdafx.h"
#include "PauseState.h"

#include "..\Graphics\GraphicsManager.h"
#include "..\Managers\InputFacade.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\GameObject_Manager.h"
#include "..\Managers\EventManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Managers\MemoryManager.h"
#include "..\GameObjectFactory.h"
#include "..\GameEngine.h"
#include "..\Graphics\Camera.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PauseState.cpp
Purpose: This is the pause state. this state is put on top of the play
state and also shows the controls to play the game.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/

extern GameObjectFactory* pGOF;
extern InputFacade* pIF;
extern GraphicsManager* pGfx;
extern Audio_Manager* pAM;
extern MemoryManager* pMM;

PauseState PauseState::mPauseState;

PauseState::PauseState() : GameState(PAUSE_STATE)
{
	
}


PauseState::~PauseState()
{
}
//;
void PauseState::Init(GameEngine * _owner, std::string level)
{
	mpOwner = _owner;
	this->pCM = new CollisionManager();
	pEM = new EventManager();
	pRM = new Resource_Manager();
	pPhysics = new Physics_Manager();
	if (_level == "") _level = level;
	if (_level == "") _level = "controls.csv";
	std::string nar;
	pGOM = FileManager::LoadLevel(_level, &w, &h, &background, &back_music, &cam_type, &next_level,&nar);
	pGfx->SetDrawSpace(0, 0, true);
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	//FileManager::LoadPopupMenu("back-menu.json", pGOM, vec4(-400,	300,0,0));
	GameObject* cur = GameObjectFactory::LoadObject("cursor.json");
	cur->visable = true;
	pGOM->PushGameObject(cur);
}

void PauseState::CleanUp()
{
	delete pRM;
	delete pGOM;
	delete pEM;
	delete pPhysics;
	GameObjectFactory::Reset();
	pRM = nullptr;
	pGOM = nullptr;
}

void PauseState::Pause()
{
}

void PauseState::Resume()
{
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetDrawSpace(0, 0, true);
}

void PauseState::ButtonCallback(unsigned int id, std::string str)
{
	if (str == "yes") {
		change_state = false;
		mpOwner->Quit();
		return;
	}
	if (str == "no") {
		GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
		cur->active = false;
		pGOM->ClearActiveObjects();
		pGOM->UnPause();
		change_state = false;
		Resume();
		return;
	}
	if (str == "back") {
		next_level = str;
		change_state = false;
		return;
	}
	change_state = true;
	next_level = str;
}

void PauseState::HandleInput()
{
	pFRC->StartFrame();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		SDL_JoystickUpdate();
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			next_level = "quit";
			change_state = true;
		}
		if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event) {
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				Pause();
				break;
			case SDL_WINDOWEVENT_RESIZED:
				pGfx->HandleResize(mpOwner->pWindow);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_RESTORED:
				Resume();
				break;
			default:
				break;
			}
		}
	}
	SDL_GameControllerUpdate();
	pIF->Update();
	if (pIF->IsButtonTriggered(QUIT)) {
		next_level = "quit";
		change_state = true;
		return;
	}
	if (pIF->IsButtonTriggered(RESIZE)) {
		mpOwner->full_screen = !mpOwner->full_screen;
		if (mpOwner->full_screen) {
			SDL_SetWindowFullscreen(mpOwner->pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else {
			SDL_SetWindowFullscreen(mpOwner->pWindow, 0);
		}

	}
	if (pIF->IsButtonTriggered(HIDE_1) && pIF->IsButtonTriggered(HIDE_2)) {
		SDL_MinimizeWindow(mpOwner->pWindow);
	}
	if (pIF->IsButtonTriggered(PAUSE_BUTTON))
	{
		mpOwner->PopState();
	}
}

void PauseState::Update()
{
	if (nullptr == pGOM) return;
	pPhysics->Update(pFRC->GetDT());
	pEM->Update(pFRC->GetDT());
	pGOM->Update();
	if (change_state) { 
		if (next_level == "quit") {
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			if (nullptr == cur) {
				pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
				GameObjectFactory::InitGameObject(cur);
			}
			else {
				cur->active = true;
				cur->unpausePriority = 0;
				pGOM->PushGameObject(cur);
			}
			pGOM->Pause();
			cur->active = true;
			cur->unpausePriority = 0;
			Camera* c = pGfx->GetCamera();
			FileManager::LoadPopupMenu("quit-menu.json", pGOM,c->camera_space->pos);
			pGOM->PushGameObject(cur);

			change_state = false;
			next_level = "";
		}
	}
	pCM->Clear();
	pMM->Update();
	if (next_level == "back") {
		mpOwner->PopState();
		return;
	}
}

void PauseState::Draw()
{
	pGfx->DrawBackground(background);
	pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context);
	pFRC->EndFrame();
}

void PauseState::Reset(std::string _level)
{
	CleanUp();
	Init(mpOwner, _level);
}
