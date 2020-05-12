#include "stdafx.h"
#include "GameClearState.h"
#include "IntroState.h"
#include "..\Graphics\Camera.h"
#include "..\Managers\MemoryManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Managers\EventManager.h"
#include "NarrativeState.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameClearState.cpp
Purpose: This is the game clear state that allows the user to exit the game 
and shows them a nice message.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/

extern GameObjectFactory* pGOF;
extern InputFacade* pIF;
extern GraphicsManager* pGfx;
extern FrameRate_Controller* pFRC;
extern Audio_Manager* pAM;
extern MemoryManager* pMM;
GameClearState GameClearState::mGameClearState;

GameClearState::GameClearState() : GameState(GAMECLEAR)
{
	
}


GameClearState::~GameClearState()
{
}

void GameClearState::Init(GameEngine * _owner, std::string level)
{
	mpOwner = _owner;
	pRM = new Resource_Manager();
	if (level != "") _level = level;
	if (_level == "") _level = "game-clear.csv";
	std::string nar;
	pGOM = FileManager::LoadLevel(_level, &w, &h, &background, &back_music, &cam_type, &next_level, &nar);
	pGfx->SetDrawSpace(0, 0, true);
	pGfx->GetCamera();
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	if (_level == "credits.csv") {
		index = 0;
		background = credits[index];
	}
	//add music
	if (pAM == nullptr)
		pAM = new Audio_Manager(back_music);

	if (!nar.empty()) {
		mpOwner->PushState(NarrativeState::GetInstance(), nar);
	}
	pPhysics = new Physics_Manager();
	pCM = new CollisionManager();
	pEM = new EventManager();
}

void GameClearState::CleanUp()
{
	GameObjectFactory::Reset();
	index = -1;
	delete pRM;
	delete pGOM;
	delete pPhysics;
	delete pCM;
	delete pEM;
	change_state = false;
}

void GameClearState::Pause()
{
	if (pAM != nullptr) {
		pAM->Pause();
	}
}

void GameClearState::Resume()
{
	
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetDrawSpace(0, 0, true);
	if (pAM != nullptr) {
		pAM->Resume();
	}
}

void GameClearState::ButtonCallback(unsigned int id, std::string str)
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
	//volumn change
	{}
	//muting
	{}
	change_state = true;
	next_level = str;
}

void GameClearState::HandleInput()
{
	pFRC->StartFrame();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			mpOwner->Quit();
			return;
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
	if (pIF->IsButtonTriggered(JUMP_BUTTON)) {
		index += 1;
		if (index < credits.size())
 			background = credits[index];
		else {
			change_state = true;
		}
	}
}

void GameClearState::Update()
{

	pPhysics->Update(pFRC->GetDT());
	pEM->Update(pFRC->GetDT());
	pGOM->Update();
	if (change_state) {//button press handling for quit and back
		if (next_level == "quit") {
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			pGOM->Pause();
			FileManager::LoadPopupMenu("quit-menu.json", pGOM);
			if (nullptr == cur) {
				cur = GameObjectFactory::LoadObject("cursor.json");
				pGOM->PushGameObject(cur);
				GameObjectFactory::InitGameObject(cur);
			}
			else {
				cur->active = true;
				cur->unpausePriority = 0;
				pGOM->PushGameObject(cur);
			}
			change_state = false;
			next_level = "";
		}
	}
	pMM->Update();
}

void GameClearState::Reset(std::string _level)
{
	CleanUp();
	Init(mpOwner, _level);
}

