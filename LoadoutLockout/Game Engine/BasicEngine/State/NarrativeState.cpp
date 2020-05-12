#include "stdafx.h"
#include "NarrativeState.h"

#include "..\Graphics\GraphicsManager.h"
#include "..\Managers\InputFacade.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\GameObject_Manager.h"
#include "..\GameObjectFactory.h"
#include "..\GameEngine.h"
#include "../Audio/Audio_Manager.h"
#include "..\Graphics\Camera.h"
#include "..\Managers\MemoryManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Managers\EventManager.h"
extern GameObjectFactory* pGOF;
extern InputFacade* pIF;
extern GraphicsManager* pGfx;
extern Audio_Manager* pAM;
extern MemoryManager* pMM;

NarrativeState NarrativeState::mNarrativeState;

NarrativeState::NarrativeState()
{
}


NarrativeState::~NarrativeState()
{
}

void NarrativeState::Init(GameEngine * _owner, std::string level)
{
	mpOwner = _owner;
	pRM = new Resource_Manager();
	if (level != "") _level = level;
	if (_level == "") _level = "act-0.csv";
	std::string nar;
	pGOM = FileManager::LoadLevel(_level, &w, &h, &background, &back_music, &cam_type, &next_level,&nar);
	pGfx->SetDrawSpace(0, 0, true);
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	pNM = new NarrativeManager(next_level);
	if (pAM == nullptr)
		pAM = new Audio_Manager(back_music);
	else
		pAM->load_audio(back_music);

	pCM = new CollisionManager();
	pPhysics = new Physics_Manager();
	pEM = new EventManager();
	
}

void NarrativeState::CleanUp()
{
	pGfx->DrawBackground("Loading.png");
	delete pNM;
	delete pRM;
	delete pGOM;
	delete pPhysics;
	delete pCM;
	delete pEM;
	GameObjectFactory::Reset();
	pRM = nullptr;
	pGOM = nullptr;
	script = 0;
}

void NarrativeState::Update()
{
	if (change_state) {
		if (next_level == "quit") {
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			pGOM->Pause();
			FileManager::LoadPopupMenu("quit-menu.json", pGOM);
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
			next_level = "";
		}
		if (next_level == "skip") {
			mpOwner->PopState();
		}
	}
	pEM->Update(pFRC->GetDT());
	pPhysics->Update(pFRC->GetDT());
	pGOM->Update();
	pMM->Update();
	if (script != -1 && !change_state)
	{
		if (script == 0)
		{
			pGfx->renderString = { 0 };
			pGfx->useText = false;
			script = pNM->ParseScript();
		}
		if (script == -1) { 
			mpOwner->PopState(); 
			return;
		}
		if (pIF->IsButtonTriggered(JUMP_BUTTON) ) {
			script = 0;
		}
	}
	
}

void NarrativeState::ButtonCallback(unsigned int id, std::string str)
{
	if (str == "yes") {
		change_state = false;
		mpOwner->Quit();
		return;
	}
	if (str == "no") {
		GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
		pGOM->ClearActiveObjects();
		pGOM->UnPause();
		change_state = false;
		return;
	}
	if (str == "next" && pGOM->MaxUnpausePriority() < 0) {
		change_state = false;
		script = 0;
		return;
	}
	change_state = true;
	next_level = str;
}

void NarrativeState::Draw()
{
	pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context);
	pFRC->EndFrame();
}

void NarrativeState::Resume()
{
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetDrawSpace(0, 0, true);
	if (pAM != nullptr) {
		pAM->Resume();
	}
}

void NarrativeState::Pause()
{
	if (pAM != nullptr) {
		pAM->Pause();
	}
}
void NarrativeState::HandleInput()
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
	if (pIF->IsButtonTriggered(CHEAT_SKIP)) {
		pGfx->useText = false;
		mpOwner->PopState();
	}
}

void NarrativeState::Reset(std::string _level)
{

}