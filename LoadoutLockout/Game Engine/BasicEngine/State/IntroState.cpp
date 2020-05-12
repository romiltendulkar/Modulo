#include "stdafx.h"
#include "IntroState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "UtilityState.h"
#include "GameClearState.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Managers\MemoryManager.h"
#include "..\Graphics\Camera.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: IntroState.h
Purpose: This is the intro state. This allows the player to start the game.
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

IntroState IntroState::mIntroState;

IntroState::IntroState() : GameState(INTRO)
{

}


IntroState::~IntroState()
{
}

void IntroState::Init(GameEngine * _owner, std::string level)
{
	pRM = new Resource_Manager();
	pEM = new EventManager();
	pCM = new CollisionManager();
	
	
	pPhysics = new Physics_Manager();
	script = 0;
	if (_level == "") _level = level;
	if (_level == "") _level = "intro.csv";
	std::string nar;
	pGOM = FileManager::LoadLevel(_level, &w, &h, &background, &back_music, &cam_type, &next_level, &nar);
	final_background = background;
	background = "DigiPenLogo.png";
	pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
	mpOwner = _owner;
	pGfx->SetDrawSpace(0, 0, true);
	pGfx->SetFollow(false);
	pGfx->FollowPlayer(0, 0);
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	change_state = false;
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	if (pAM == nullptr)
		pAM = new Audio_Manager(back_music);
//	pAM->SetAudioVolume(mpOwner->music*mpOwner->master);
	pEM = new EventManager();
	change_state = false;
	next_level = "";
	time = 2.f;
	displayedDigiLogo = false;
	team_logo = false;
	pGOM->Pause();
	GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
	cur->active = true;
	cur->unpausePriority = 0;
	pEM->BroadcastEvent(new PauseEvent());
}

void IntroState::CleanUp()
{
	change_state = false;
	next_level = "";
	GameObjectFactory::Reset();
	delete pEM;
	delete pRM;
	delete pGOM;
	delete pPhysics;
	delete pCM;
	delete pAM;
	pAM = nullptr;
	time = 2.f;
}

void IntroState::Pause()
{
	pEM->BroadcastEvent(new PauseEvent());
	if (pAM != nullptr) {
		pAM->Pause();
	}
	next_level = "";
	change_state = false;
}

void IntroState::Resume()
{
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetDrawSpace(0, 0, true);
	next_level = "";
	if (pAM != nullptr) {
		pAM->Resume();
	}
	pEM->BroadcastEvent(new PauseEvent());
}

void IntroState::ButtonCallback(unsigned int id, std::string str)
{
	if (str == "yes") { 
		change_state = false;
		mpOwner->Quit(); 
		return;
	}
	if (str == "no") {
		GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
		
		cur->active = false;
		cur->unpausePriority = 1;
		pGOM->ClearActiveObjects();
		pGOM->UnPause();
		change_state = false;
		Resume();
		next_level = "";
		return;
	}
	change_state = true;
	next_level = str;
}

void IntroState::HandleInput()
{
	pFRC->StartFrame();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			next_level = "quit";
			change_state = true;
			cout << next_level << endl;
			
		}
		if (e.type == SDL_WINDOWEVENT ){
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
		cout << next_level << endl;
		
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
}

void IntroState::Update()
{

	pPhysics->Update(pFRC->GetDT());
	pGOM->Update();
	pEM->Update(pFRC->GetDT());
	Cursor* c = static_cast<Cursor*>(pGOM->GetObjectByType(TYPE_CURSOR)->GetComponent(_CURSOR));
	if (!c->mpOwner->active || c->mpOwner->unpausePriority > 0) {
		cout << "ERROR CURSOR IS NOT ACTIVE";
	}
	if ((pIF->IsButtonTriggered(JUMP_BUTTON) || c->IsClick) && !(fmod_logo&&displayedDigiLogo && team_logo && done)) {
		mpOwner->skip_intro = true;
		time = 0;
		displayedDigiLogo = true;
		team_logo = true;
		done = true;
		fmod_logo = true;
		background = final_background;
		pGOM->UnPause();
		pEM->BroadcastEvent(new PauseEvent());
	}
	if (change_state && (team_logo && displayedDigiLogo)) {
		if (next_level == "quit") {
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			pGOM->Pause();
			cur->active = true;
			cur->unpausePriority = 0;
			FileManager::LoadPopupMenu("quit-menu.json", pGOM);
			pGOM->PushGameObject(cur);

			change_state = false;
			next_level = "";
		}
		else if (next_level == "controls.csv")
		{
			
			mpOwner->PushState(PauseState::GetInstance());
			return;
		}
		else if (next_level == "credits.csv") {
			mpOwner->PushState(GameClearState::GetInstance(), "credits.csv");
			change_state = false;
		}
		else if (next_level == "settings.csv") {//all game levels MUST start with the letter "l"
			mpOwner->PushState(UtilityState::GetInstance(), next_level);
		}
		else {
			mpOwner->ChangeState(PlayState::GetInstance(), next_level);
			return;
		}
	}
	else {
		change_state = false;
		next_level = "";
	}
	change_state = false;
	pMM->Update();
}

void IntroState::Reset(std::string _level)
{
	CleanUp();
	Init(mpOwner, _level);
}

void IntroState::Draw()
{
	if (displayedDigiLogo && team_logo&& fmod_logo && done) {
		pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context);
	} else if(!displayedDigiLogo) {
		pGfx->Update(mpOwner->pWindow, nullptr, mpOwner->opengl_context);
		time -= pFRC->GetDT();
		displayedDigiLogo = (time <= 0);
		if (displayedDigiLogo) {
			background = "FMOD.png";
			time = 1.75;
		}
	}
	else if (!fmod_logo) {
		pGfx->Update(mpOwner->pWindow, nullptr, mpOwner->opengl_context);
		time -= pFRC->GetDT();
		fmod_logo = (time <= 0);
		if (fmod_logo) {
			background = "Teamintro.png";
			time = 1.75;
		}
	}
	else if(!team_logo){
		pGfx->Update(mpOwner->pWindow, nullptr, mpOwner->opengl_context);
		time -= pFRC->GetDT();
		team_logo = (time <= 0);
		if (team_logo) {
			background = "BestUseController.png";
			time = 1.75;
		}
	}
	else {
		pGfx->Update(mpOwner->pWindow, nullptr, mpOwner->opengl_context);
		time -= pFRC->GetDT();
		done = (time <= 0);
		if (done) {
			background = final_background;
			pGOM->UnPause();
			pEM->BroadcastEvent(new PauseEvent());
		}
	}
	pFRC->EndFrame();
}

