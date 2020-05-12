#include "stdafx.h"
#include "UtilityState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "UtilityState.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Managers\Physics_Manager.h"
#include "..\Graphics\Camera.h"
#include "..\Events\ChangeSpriteEvent.h"

extern GameObjectFactory* pGOF;
extern InputFacade* pIF;
extern GraphicsManager* pGfx;
extern Audio_Manager* pAM;

UtilityState UtilityState::mUtilityState;

UtilityState::UtilityState()
{
}


UtilityState::~UtilityState()
{
}

void UtilityState::Init(GameEngine * _owner, std::string level)
{
	mpOwner = _owner;
	pRM = new Resource_Manager();
	pGOM = new GameObject_Manager();
	pCM = new CollisionManager();
	pPhysics = new Physics_Manager();
	if (_level == "") _level = level;
	std::string nar;
	pGOM = FileManager::LoadLevel(_level, &w, &h, &this->background, &back_music, &cam_type, &next_level, &nar);
	pGfx->SetDrawSpace(0, 0, true);
	Camera* c = pGfx->GetCamera();
	c->SetCameraType(cam_type);
	pGfx->SetView(pGfx->GetWindowWidth() / 2.f, pGfx->GetWindowHeight() / 2.f);
	//pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
	pAM->Resume();
	pEM = new EventManager();
	change_state = false;
	next_level = "";

	GameObject* cur = GameObjectFactory::LoadObject("cursor.json");
	pGOM->PushGameObject(cur);
	cur->active = true;
	cur->unpausePriority = 0;
	pGOM->PushGameObject(cur);

	std::vector<GameObject *>mObj = pGOM->GetObjects();
	Button *PButton;
	Sprite *pSprite;
	if (mpOwner->mute_master)
	{
		PButton = static_cast<Button *>(mObj[10]->GetComponent(_BUTTON));
		PButton->selected = "Masterselected_hovered.png";
		PButton->n_selected = "Masterselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[10]->GetComponent(SPRITE));
		pSprite->file = "Masterselected_nothovered.png";
		
	}
	else if (!mpOwner->mute_master)
	{
		PButton = static_cast<Button *>(mObj[10]->GetComponent(_BUTTON));
		PButton->selected = "Masternotselected_hovered.png";
		PButton->n_selected = "Masternotselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[10]->GetComponent(SPRITE));
		pSprite->file = "Masternotselected_nothovered.png";
	}
	if (mpOwner->mute_music)
	{
		PButton = static_cast<Button *>(mObj[11]->GetComponent(_BUTTON));
		PButton->selected = "Musicselected_hovered.png";
		PButton->n_selected = "Musicselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[11]->GetComponent(SPRITE));
		pSprite->file = "Musicselected_nothovered.png";
	}
	else if(!mpOwner->mute_music)
	{
		PButton = static_cast<Button *>(mObj[11]->GetComponent(_BUTTON));
		PButton->selected = "MusicNotselected_hovered.png";
		PButton->n_selected = "MusicNotselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[11]->GetComponent(SPRITE));
		pSprite->file = "MusicNotselected_nothovered.png";
	}
	if (mpOwner->mute_sfx)
	{
		PButton = static_cast<Button *>(mObj[12]->GetComponent(_BUTTON));
		PButton->selected = "SFXselected_hovered.png";
		PButton->n_selected = "SFXselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[12]->GetComponent(SPRITE));
		pSprite->file = "SFXselected_nothovered.png";
	}
	else if(!mpOwner->mute_sfx)
	{
		PButton = static_cast<Button *>(mObj[12]->GetComponent(_BUTTON));
		PButton->selected = "SFXnotselected_hovered.png";
		PButton->n_selected = "SFXnotselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[12]->GetComponent(SPRITE));
		pSprite->file = "SFXnotselected_nothovered.png";
	}
	if (mpOwner->full_screen)
	{
		PButton = static_cast<Button *>(mObj[13]->GetComponent(_BUTTON));
		PButton->selected = "fullscreenselected_hovered.png";
		PButton->n_selected = "fullscreenselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[13]->GetComponent(SPRITE));
		pSprite->file = "fullscreenselected_nothovered.png";
	}
	else if (!mpOwner->full_screen)
	{
		PButton = static_cast<Button *>(mObj[13]->GetComponent(_BUTTON));
		PButton->selected = "fullscreennotselected_hovered.png";
		PButton->n_selected = "fullscreennotselected_nothovered.png";
		pSprite = static_cast<Sprite *>(mObj[13]->GetComponent(SPRITE));
		pSprite->file = "fullscreennotselected_nothovered.png";
	}

}

void UtilityState::CleanUp()
{
}

void UtilityState::Pause()
{
	if (pAM != nullptr) {
		pAM->Pause();
	}
}

void UtilityState::Resume()
{
	if (pAM != nullptr) {
		pAM->Resume();
	}
}

void UtilityState::ButtonCallback(unsigned int id, std::string str)
{
	std::vector<GameObject *>mObj = pGOM->GetObjects();
	Button *PButton;
	Sprite *pSprite;

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
		return;
	}
	
	//volumn change
	{
		if (str == "Volume-Up-SFX") {
			mpOwner->current_sfx += .1;
			if (mpOwner->current_sfx > 1.0) mpOwner->current_sfx = 1.0;
			if (!mpOwner->mute_sfx) {
				mpOwner->sfx = mpOwner->current_sfx;
			}
			else {
				mpOwner->sfx = 0.0;
			}
			pAM->SetSFXVolume(mpOwner->sfx);
			change_state = false;
			return;
		}
		if (str == "Volume-Up-Music") {
			mpOwner->current_music += .1;
			if (mpOwner->current_music > 1.0) mpOwner->current_music = 1.0;
			if (!mpOwner->mute_music) {
				mpOwner->music = mpOwner->current_music;
			}
			else {
				mpOwner->music = 0.0;
			}
			pAM->SetMusicVolume(mpOwner->music);
			change_state = false;
			return;
		}
		if (str == "Volume-Up-Master") {
			mpOwner->current_master += .1;
			if (mpOwner->current_master > 1.0) mpOwner->current_master = 1.0;
			if (!mpOwner->mute_master) {
				mpOwner->master = mpOwner->current_master;
			}
			else {
				mpOwner->master = 0.0;
			}
			pAM->SetMasterVolume(mpOwner->master);
			change_state = false;
			return;
		}
		///
		if (str == "Volume-Down-SFX") {
			mpOwner->current_sfx -= .1;
			if (mpOwner->current_sfx < 0.0) mpOwner->current_sfx = 0.0;
			if (!mpOwner->mute_sfx) {
				mpOwner->sfx = mpOwner->current_sfx;
			}
			else {
				mpOwner->sfx = 0.0;
			}
			pAM->SetSFXVolume(mpOwner->sfx);
			change_state = false;
			return;
		}
		if (str == "Volume-Down-Music") {
			mpOwner->current_music -= .1;
			if (mpOwner->current_music < 0.0) mpOwner->current_music = 0.0;
			if (!mpOwner->mute_music) {
				mpOwner->music = mpOwner->current_music;
			}
			else {
				mpOwner->music = 0.0;
			}
			pAM->SetMusicVolume(mpOwner->music);
			change_state = false;
			return;
		}
		if (str == "Volume-Down-Masters") {
			mpOwner->current_master -= .1;
			if (mpOwner->current_master < 0.0) mpOwner->current_master = 0.0;
			if (!mpOwner->mute_master) {
				mpOwner->master = mpOwner->current_master;
			}
			else {
				mpOwner->master = 0.0;
			}
			pAM->SetMasterVolume(mpOwner->master);
			change_state = false;
			return;
		}
	}
	//muting
	{
		if (str == "Mute-SFX") {
			mpOwner->mute_sfx = !mpOwner->mute_sfx;
			if (!mpOwner->mute_sfx) {
				mpOwner->sfx = mpOwner->current_sfx;
			}
			else {
				mpOwner->sfx = 0.0;
			}
			pAM->SetSFXVolume(mpOwner->sfx);


			if (mpOwner->mute_sfx)
			{
				PButton = static_cast<Button *>(mObj[12]->GetComponent(_BUTTON));
				PButton->selected = "SFXselected_hovered.png";
				PButton->n_selected = "SFXselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[12]->GetComponent(SPRITE));
				pSprite->file = "SFXselected_hovered.png";
			}
			else if (!mpOwner->mute_sfx)
			{
				PButton = static_cast<Button *>(mObj[12]->GetComponent(_BUTTON));
				PButton->selected = "SFXnotselected_hovered.png";
				PButton->n_selected = "SFXnotselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[12]->GetComponent(SPRITE));
				pSprite->file = "SFXnotselected_hovered.png";
			}

			change_state = false;
			return;
		}
		if (str == "Mute-Music") {
			mpOwner->mute_music = !mpOwner->mute_music;
			if (!mpOwner->mute_music) {
				mpOwner->music = mpOwner->current_music;
			}
			else {
				mpOwner->music = 0.0;
			}
			pAM->SetMusicVolume(mpOwner->music);

			if (mpOwner->mute_music)
			{
				PButton = static_cast<Button *>(mObj[11]->GetComponent(_BUTTON));
				PButton->selected = "Musicselected_hovered.png";
				PButton->n_selected = "Musicselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[11]->GetComponent(SPRITE));
				pSprite->file = "Musicselected_hovered.png";
			}
			else if(!mpOwner->mute_music)
			{
				PButton = static_cast<Button *>(mObj[11]->GetComponent(_BUTTON));
				PButton->selected = "MusicNotselected_hovered.png";
				PButton->n_selected = "MusicNotselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[11]->GetComponent(SPRITE));
				pSprite->file = "MusicNotselected_hovered.png";
			}
			change_state = false;
			return;
		}
		if (str == "Mute-Master") {
			mpOwner->mute_master = !mpOwner->mute_master;
			if (!mpOwner->mute_master) {
				mpOwner->master = mpOwner->current_master;
			}
			else {
				mpOwner->master = 0.0;
			}
			pAM->SetMasterVolume(mpOwner->master);


			if (mpOwner->mute_master)
			{
				PButton = static_cast<Button *>(mObj[10]->GetComponent(_BUTTON));
				PButton->selected = "Masterselected_hovered.png";
				PButton->n_selected = "Masterselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[10]->GetComponent(SPRITE));
				pSprite->file = "Masterselected_hovered.png";

			}
			else if (!mpOwner->mute_master)
			{
				PButton = static_cast<Button *>(mObj[10]->GetComponent(_BUTTON));
				PButton->selected = "Masternotselected_hovered.png";
				PButton->n_selected = "Masternotselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[10]->GetComponent(SPRITE));
				pSprite->file = "Masternotselected_hovered.png";
			}
			change_state = false;
			return;
		}
	}
	//resizing
	{
		if (str == "full-screen") {
			mpOwner->full_screen = !mpOwner->full_screen;
			if (mpOwner->full_screen) {
				SDL_SetWindowFullscreen(mpOwner->pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			else {
				SDL_SetWindowFullscreen(mpOwner->pWindow, 0);
			}
			if (mpOwner->full_screen)
			{
				PButton = static_cast<Button *>(mObj[13]->GetComponent(_BUTTON));
				PButton->selected = "fullscreenselected_hovered.png";
				PButton->n_selected = "fullscreenselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[13]->GetComponent(SPRITE));
				pSprite->file = "fullscreenselected_hovered.png";
			}
			else if (!mpOwner->full_screen)
			{
				PButton = static_cast<Button *>(mObj[13]->GetComponent(_BUTTON));
				PButton->selected = "fullscreennotselected_hovered.png";
				PButton->n_selected = "fullscreennotselected_nothovered.png";
				pSprite = static_cast<Sprite *>(mObj[13]->GetComponent(SPRITE));
				pSprite->file = "fullscreennotselected_hovered.png";
			}
		}

		
	}
	if (str == "back") {
		mpOwner->PopState();
		return;
	}
	change_state = true;
	next_level = str;
}

void UtilityState::HandleInput()
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
	if (pIF->IsButtonTriggered(PAUSE_BUTTON))
	{
		mpOwner->PopState();
	}
}

void UtilityState::Update()
{
	pPhysics->Update(pFRC->GetDT());
	pEM->Update(pFRC->GetDT());
	pGOM->Update();
	if (change_state) {//button press handling for quit and back
		if (next_level == "quit") {
			GameObject* cur = pGOM->GetObjectByType(TYPE_CURSOR);
			pGOM->Pause();
			if (nullptr == cur) {
				pGOM->PushGameObject(GameObjectFactory::LoadObject("cursor.json"));
				GameObjectFactory::InitGameObject(cur);
			}
			else {
				cur->active = true;
				cur->unpausePriority = 0;
				pGOM->PushGameObject(cur);
			}
			
			cur->active = true;
			FileManager::LoadPopupMenu("quit-menu.json", pGOM);
			pGOM->PushGameObject(cur);

			change_state = false;
			next_level = "";
		}
	}

	std::vector<GameObject *>mObj = pGOM->GetObjects();
	Sprite *pSprite;
	pSprite = static_cast<Sprite *>(mObj[2]->GetComponent(SPRITE));
	if (mpOwner->current_master <= 1.0f)
	{
		pSprite->file = "MasterTexture10.png";
	}
	if (mpOwner->current_master <= 0.9f)
	{
		pSprite->file = "MasterTexture9.png";
	}
	if (mpOwner->current_master <= 0.8f)
	{
		pSprite->file = "MasterTexture8.png";
	}
	if (mpOwner->current_master <= 0.7f)
	{
		pSprite->file = "MasterTexture7.png";
	}
	if (mpOwner->current_master <= 0.6f)
	{
		pSprite->file = "MasterTexture6.png";
	}
	if (mpOwner->current_master <= 0.5f)
	{
		pSprite->file = "MasterTexture5.png";
	}
	if (mpOwner->current_master <= 0.4f)
	{
		pSprite->file = "MasterTexture4.png";
	}
	if (mpOwner->current_master <= 0.3f)
	{
		pSprite->file = "MasterTexture3.png";
	}
	if (mpOwner->current_master <= 0.2f)
	{
		pSprite->file = "MasterTexture2.png";
	}
	if (mpOwner->current_master <= 0.1f)
	{
		pSprite->file = "MasterTexture1.png";
	}
	if (mpOwner->current_master <= 0.0f)
	{
		pSprite->file = "MasterTexture0.png";
	}
	
	
	

	pSprite = static_cast<Sprite *>(mObj[5]->GetComponent(SPRITE));
	if (mpOwner->current_music <= 1.0f)
	{
		pSprite->file = "MusicTexture10.png";
	}
	if (mpOwner->current_music <= 0.9f)
	{
		pSprite->file = "MusicTexture9.png";
	}
	if (mpOwner->current_music <= 0.8f)
	{
		pSprite->file = "MusicTexture8.png";
	}
	if (mpOwner->current_music <= 0.7f)
	{
		pSprite->file = "MusicTexture7.png";
	}
	if (mpOwner->current_music <= 0.6f)
	{
		pSprite->file = "MusicTexture6.png";
	}
	if (mpOwner->current_music <= 0.5f)
	{
		pSprite->file = "MusicTexture5.png";
	}
	if (mpOwner->current_music <= 0.4f)
	{
		pSprite->file = "MusicTexture4.png";
	}
	if (mpOwner->current_music <= 0.3f)
	{
		pSprite->file = "MusicTexture3.png";
	}
	if (mpOwner->current_music <= 0.2f)
	{
		pSprite->file = "MusicTexture2.png";
	}
	if (mpOwner->current_music <= 0.1f)
	{
		pSprite->file = "MusicTexture1.png";
	}
	if (mpOwner->current_music <= 0.0f)
	{
		pSprite->file = "MusicTexture0.png";
	}

	pSprite = static_cast<Sprite *>(mObj[8]->GetComponent(SPRITE));
	if (mpOwner->current_sfx <= 1.0f)
	{
		pSprite->file = "SFXTexture10.png";
	}
	if (mpOwner->current_sfx <= 0.9f)
	{
		pSprite->file = "SFXTexture9.png";
	}
	if (mpOwner->current_sfx <= 0.8f)
	{
		pSprite->file = "SFXTexture8.png";
	}
	if (mpOwner->current_sfx <= 0.7f)
	{
		pSprite->file = "SFXTexture7.png";
	}
	if (mpOwner->current_sfx <= 0.6f)
	{
		pSprite->file = "SFXTexture6.png";
	}
	if (mpOwner->current_sfx <= 0.5f)
	{
		pSprite->file = "SFXTexture5.png";
	}
	if (mpOwner->current_sfx <= 0.4f)
	{
		pSprite->file = "SFXTexture4.png";
	}
	if (mpOwner->current_sfx <= 0.3f)
	{
		pSprite->file = "SFXTexture3.png";
	}
	if (mpOwner->current_sfx <= 0.2f)
	{
		pSprite->file = "SFXTexture2.png";
	}
	if (mpOwner->current_sfx <= 0.1f)
	{
		pSprite->file = "SFXTexture1.png";
	}
	if (mpOwner->current_sfx <= 0.0f)
	{
		pSprite->file = "SFXTexture0.png";
	}
	
}

void UtilityState::Reset(std::string _level)
{
}
