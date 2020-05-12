#include "stdafx.h"
#include "PlayState.h"
#include "..\Events\PauseEvent.h"
#include "PauseState.h"
#include "UtilityState.h"
#include "GameClearState.h"
#include "NarrativeState.h"
#include "GameOverState.h"
#include "IntroState.h"
#include "..\GameObject.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\EventManager.h"
#include "..\Managers\GameObject_Manager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Managers\MemoryManager.h"
#include "..\Managers\ParticleManager.h"
#include "../Audio/Audio_Manager.h"
#include "..\Graphics\Camera.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PlayState.cpp
Purpose: This is the play state definition, this allows for basic debugging
and cheats to win and lose conditions in debug mode.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/

extern InputFacade* pIF;
extern GraphicsManager* pGfx;
extern GameObjectFactory* pGOF;
extern 	FrameRate_Controller * pFRC;
extern MemoryManager* pMM;
extern Audio_Manager* pAM;


PlayState PlayState::mPlayState;

PlayState::PlayState() : GameState(PLAY)
{
	
}


PlayState::~PlayState()
{
}

void PlayState::Init(GameEngine * _owner, std::string level)
{
	pEM = new EventManager();
	pRM = new Resource_Manager();
	pGfx->DrawBackground("Loading.png");
	mpOwner = _owner;
	if (prev_lives < 3) prev_lives = 3;
	if (prev_lives > 9) prev_lives = 9;
	if(level != "") _level = level;
	if (_level == "") _level = "level-0-0.csv";
	std::string nar = "";
	pGOM = FileManager::LoadLevel(_level, &this->w, &this->h, &this->background, 
		&this->back_music, &this->cam_type, &this->next_level,&nar);
	pPhysics = new Physics_Manager();
	pCM = new CollisionManager();
	pPM = new ParticleManager();
	if (pAM == nullptr)
		pAM = new Audio_Manager(back_music);
	else
		pAM->Resume();

	pEM = new EventManager();
	lState = luaL_newstate();
	luaL_openlibs(lState);
	win = false;
	move_to_next = false;
	pGfx->SetFollow(true);
	pGfx->SetDrawSpace(w, h, false);
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	Body* b = static_cast<Body*>(pGOM->GetObjectByType(TYPE_GOAL)->GetComponent(BODY));
	b = static_cast<Body*>(pGOM->GetObjectByType(TYPE_PLAYER)->GetComponent(BODY));
	static_cast<Player*>(pGOM->GetObjectByType(TYPE_PLAYER)->GetComponent(PLAYER))->lives = prev_lives;
	c->camera_space->pos = b->pos;
	pGfx->FollowPlayer(b->pos.x, b->pos.y);
	if (!nar.empty()) {
		mpOwner->PushState(NarrativeState::GetInstance(), nar);//this nar is the narative 'level' file
	}
	paused = false;
	current_trans_time = transition_time;
	move_to_next = false;
	win = false;
	complete = false;
	GameObject* player = pGOM->GetObjectByType(TYPE_PLAYER);
	pGOM->PushGameObject(player);
}

void PlayState::CleanUp()
{
	pGfx->ResetCamera();
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);

	delete pGOM,
	delete pPhysics;
	delete pCM;
	delete pEM;
	delete pPM;
	delete pAM;
	pAM = nullptr;
	delete pRM;
	
	GameObjectFactory::Reset();
	lua_close(lState);
}

void PlayState::ButtonCallback(unsigned int id, std::string str)
{
	if (str == "yes") {
		change_state = false;
		mpOwner->Quit();
		return;
	}
	if (str == "resume") {
		pGOM->ClearActiveObjects();
		change_state = false;
		Resume();
		return;
	}
	if (str == "no" ) {
		GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
		if(pGOM->MaxUnpausePriority() > 1)
			cur->active = false;
		pGOM->ClearActiveObjects();
		if (pGOM->MaxUnpausePriority() > 1)
			cur->active = true;
		change_state = false;
		Resume();
		return;
	}
	if (str == "back") {
		mpOwner->PopState();
		return;
	}
	if (str == "next") {
		button = next_level;
		change_state = true;
		move_to_next = true;
		return;
	}
	//add other functions like controls and muting
	change_state = true;
	button = str;
}

void PlayState::Pause()
{
	paused = true;
	
	GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
	pGOM->Pause();
	pEM->BroadcastEvent(new PauseEvent());
	if (pAM != nullptr) {
		pAM->Pause();
	}
	prev_pos = pGfx->GetCamera()->camera_space->pos;
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	pGfx->ResetCamera();
	button = "";
	change_state = false;
	
}

void PlayState::Resume()
{
	paused = false;
	pGOM->UnPause();
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	
	pGfx->SetFollow(true);
	pGfx->SetDrawSpace(w, h, false);
	Body* b= static_cast<Body*>(pGOM->GetObjectByType(TYPE_PLAYER)->GetComponent(BODY));
	pGfx->ResetCamera(b->pos.x, b->pos.y);
	pEM->BroadcastEvent(new PauseEvent());
	if (nullptr != pAM && 0 <= pGOM->MaxUnpausePriority())
		pAM->Resume();
	else
		pAM = new Audio_Manager(back_music);
}

void PlayState::HandleInput()
{
	pFRC->StartFrame();
	dt = pFRC->GetDT();

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			button = "quit";
			change_state = true;
			return;
		}
		Camera* c;
		GameObject* cur;
		int i;
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
	//UPDATE
	//input
	pIF->Update();
	if (pIF->IsButtonTriggered(PAUSE_BUTTON)) {
		//boardcast pause event
		
		if (pGOM->MaxUnpausePriority() == 0) {
			if (pGfx->GetDebug()) pGfx->ToggleDebug();
			//display pause menu
			button = "pause";
			change_state = true;
		}
		else if (complete && pGOM->MaxUnpausePriority() > 0) {
			button = "pause";
			change_state = true;
		}
		else {
			button = "resume";
			change_state = true;
		}
		return;
	}
	
	if (pIF->IsButtonTriggered(QUIT) ) {
		button = "quit";
		change_state = true;
		return;
	}
	if (change_state) {
		if (button == "quit") {
			pGOM->Pause();
			pEM->BroadcastEvent(new PauseEvent());
			if (pAM != nullptr) {
				pAM->Pause();
			}
			prev_pos = pGfx->GetCamera()->camera_space->pos;
			Camera* c = pGfx->GetCamera();
			FileManager::LoadPopupMenu("quit-menu.json", pGOM,c->camera_space->pos);
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			if (nullptr == cur) {
				
				pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
				GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
				Transform* pT = static_cast<Transform*>(cur->GetComponent(TRANSFORM));
				pT->scaleXY.x = 20;
				pT->scaleXY.y = 20;
				GameObjectFactory::InitGameObject(cur);
			}
			else {
				cur->active = true;
				cur->unpausePriority = 0;
				pGOM->PushGameObject(cur);
			}

			change_state = false;
			button = "";
			return;
		}
		if (button == "pause") {
			
			pGOM->Pause();
			int i = pEM->BroadcastEvent(new PauseEvent());
			if (i < 0) {
				mpOwner->Quit();
			}
			if (pAM != nullptr) {
				//delete pAM;
				pAM->Pause();
			}
			prev_pos = pGfx->GetCamera()->camera_space->pos;
			Camera* c = pGfx->GetCamera();
			FileManager::LoadPopupMenu("pause-menu.json", pGOM, c->camera_space->pos);
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			if (nullptr == cur) {
				pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
				cur = pGOM->GetObjectByType(TYPE_CURSOR);
				Transform* pT = static_cast<Transform*>(cur->GetComponent(TRANSFORM));
				pT->scaleXY.x = 20;
				pT->scaleXY.y = 20;
				GameObjectFactory::InitGameObject(cur);
			} else {
				cur->active = true;
				cur->unpausePriority = 0;
				pGOM->PushGameObject(cur);
			}
			change_state = false;
			button = "";
			return;
		}
		if (button == "resume") {

			pGOM->ClearActiveObjects();
			change_state = false;
			Resume();
			button = "";
		}
	}
	if (pIF->IsButtonTriggered(JUMP_BUTTON) && complete) {
		move_to_next = true;
	}
	GameObject* player = nullptr;
	Player* pP = nullptr;
	if (pIF->IsButtonTriggered(CHEAT_LIFE)) {
		player = pGOM->GetObjectByType(TYPE_PLAYER);
		pP = static_cast<Player*>(player->GetComponent(PLAYER));
		pP->lives += 1;
		if (pP->lives > 10) pP->lives = 10;
	}
	if (pIF->IsButtonTriggered(CHEAT_NO_FRIENDS)) {
		saved_friends = 0;
	}
	if (pIF->IsButtonTriggered(CHEAT_MAX_FRIENDS)) {
		saved_friends = 999;
	}
	if (pIF->IsButtonTriggered(CHEAT_SKIP)) {
		complete = true;
	}
	
	if (pIF->IsButtonTriggered(RESIZE)) {
		mpOwner->full_screen = !mpOwner->full_screen;
		if (mpOwner->full_screen) {
			SDL_SetWindowFullscreen(mpOwner->pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			//SDL_SetRelativeMouseMode(SDL_TRUE);
		}else {
			SDL_SetWindowFullscreen(mpOwner->pWindow, 0);
			//SDL_SetRelativeMouseMode(SDL_FALSE);
		}

	}
	if (pIF->IsButtonTriggered(HIDE_1) && pIF->IsButtonTriggered(HIDE_2)) {
		SDL_MinimizeWindow(mpOwner->pWindow);
	}
	//add 
#ifdef _DEBUG
	if (pIF->IsButtonTriggered(BACK)) {//maybe don't need this
		mpOwner->ChangeState(IntroState::GetInstance());
		return;
	}
	if (pIF->IsButtonTriggered(DEBUG)) {
		//Debug mode
		pGfx->ToggleDebug();
	}
	if (pIF->IsButtonTriggered(GUI))
	{
		pGfx->ToggleGUI();
	}
#endif
	
}

void PlayState::Update()
{
	if (current_trans_time > 0.f && !move_to_next) {
		current_trans_time -= dt;
	}
	else if(!move_to_next){
		current_trans_time = 0;
	}
	//physics
	pPhysics->Update(dt);
	
	//events
	pEM->Update(dt);
	//game objects
	pPM->Update(dt);
  	pGOM->Update();

	pMM->Update();
	GameObject* player = pGOM->GetObjectByType(TYPE_PLAYER); 
	if (nullptr ==  player) {
		Reset(_level);
	} 
	if (complete && !win) {//fix when adding victory
		if (pGfx->GetDebug()) pGfx->ToggleDebug();
		DisplayVictory();
	}
	//will need to switch levels here
	if (button == "controls.csv")
	{
		mpOwner->PushState(PauseState::GetInstance());
		return;
	}
	if (button == "credits.csv")
	{
		mpOwner->PushState(GameClearState::GetInstance(),"credits.csv");
		return;
	}
	
}

void PlayState::Draw()
{
	//render
	vec4 color = vec4(1);
	if (current_trans_time > 0) {
		float c = (current_trans_time / transition_time);
		color = vec4(1.f -c, 1.f - c, 1.f - c,1.f);
	}
	pGfx->DrawBackground(background,color);
	pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context,false,color);
	//end
	pFRC->EndFrame();
	GameObject* player = pGOM->GetObjectByType(TYPE_PLAYER);
	if (move_to_next) {
		if (current_trans_time <= transition_time) {
			current_trans_time += dt;
		}
		else {
			current_trans_time = transition_time;
			if (next_level.empty()) {
				move_to_next = false;
				if (saved_friends >= total_friends) {//change with other endings added
					saved_friends = 0;
					mpOwner->ChangeState(GameClearState::GetInstance(), "good-end.csv");//canon good ending
				}
				else if (saved_friends < total_friends && saved_friends != 0) {
					saved_friends = 0;
					mpOwner->ChangeState(GameClearState::GetInstance(), "bad-end.csv");
				}
				else {
					saved_friends = 0;
					mpOwner->ChangeState(GameClearState::GetInstance(), "worst-end.csv");
				}
			}
			else {
				button = next_level;
				saved_friends += static_cast<Player*>(player->GetComponent(PLAYER))->saved_count;
				prev_lives = static_cast<Player*>(player->GetComponent(PLAYER))->lives;

				Reset(button);
				return;
			}
		}

	}
	if (pIF->IsButtonTriggered(CHEAT_END)) {
		Reset("level-final.csv");
	}

	if (button == "settings.csv")
	{
		mpOwner->PushState(UtilityState::GetInstance(), button);
	}
}

void PlayState::Reset(std::string _level)
{
	current_trans_time = transition_time;
	CleanUp();
	Init(mpOwner,_level);
}

void PlayState::DisplayVictory()
{
	win = true;
	pGOM->Pause();
	int i = pEM->BroadcastEvent(new PauseEvent());
	Camera* c = pGfx->GetCamera();
	if(next_level != "")
		FileManager::LoadPopupMenu("victory-menu.json", pGOM, c->camera_space->pos);
	else
		FileManager::LoadPopupMenu("game-clear-menu.json", pGOM, c->camera_space->pos);
	GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
	if (nullptr == cur) {
		pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
		GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
		Transform* pT = static_cast<Transform*>(cur->GetComponent(TRANSFORM));
		pT->scaleXY.x = 20;
		pT->scaleXY.y = 20;
		GameObjectFactory::InitGameObject(cur);
	}
	else {
		cur->active = true;
		cur->unpausePriority = 0;
		pGOM->PushGameObject(cur);
	}
}
