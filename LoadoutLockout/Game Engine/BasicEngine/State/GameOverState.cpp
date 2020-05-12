#include "stdafx.h"
#include "GameOverState.h"

#include "..\Graphics\GraphicsManager.h"
#include "..\Managers\InputFacade.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\GameObject_Manager.h"
#include "PlayState.h"
#include "IntroState.h"
#include "..\GameObjectFactory.h"
#include "..\Graphics\Camera.h"


extern GameObjectFactory* pGOF;
extern InputFacade* pIF;
extern GraphicsManager* pGfx;
extern FrameRate_Controller* pFRC;
extern Audio_Manager* pAM;

GameOverState GameOverState::mGameOverState;

GameOverState::GameOverState() : GameState(GAMEOVER)
{
}


GameOverState::~GameOverState()
{
}

void GameOverState::Init(GameEngine * _owner, std::string level)
{
	mpOwner = _owner;
	pRM = new Resource_Manager();
	pGOM = new GameObject_Manager();
	if (_level == "") _level = level;
	std::string nar;
	pGOM = FileManager::LoadLevel("game-over.csv", &w, &h, &this->background, &back_music, &cam_type, &next_level,&nar);
	pGfx->SetDrawSpace(0, 0, true);
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	//add music 
	if (pAM == nullptr)
		pAM = new Audio_Manager(back_music);
//	pAM->SetAudioVolume(mpOwner->music*mpOwner->master);
}

void GameOverState::CleanUp()
{
	GameObjectFactory::Reset();
	delete pRM;
	delete pGOM;
}

void GameOverState::Pause()
{
	if (pAM != nullptr) {
		pAM->Pause();
	}
}

void GameOverState::Resume()
{
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetDrawSpace(0, 0, true);
	if (pAM != nullptr) {
		pAM->Resume();
	}
}

void GameOverState::ButtonCallback(unsigned int id, std::string str)
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
	change_state = true;
	next_level = str;
}

void GameOverState::HandleInput()
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

void GameOverState::Update()
{
	pGOM->Update();
}

void GameOverState::Reset(std::string _level)
{
	CleanUp();
	Init(mpOwner, _level);
}

