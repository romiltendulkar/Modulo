#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Managers\InputFacade.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Managers\GameObject_Manager.h"
#include "..\Managers\EventManager.h"
#include "..\GameObjectFactory.h"
#include "IntroState.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameClearState.h
Purpose: This is the game clear state definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/
class GameClearState :
	public GameState
{
public:
	GameClearState();
	~GameClearState();
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void Pause();
	virtual void Resume();
	virtual void ButtonCallback(unsigned int id, std::string str);

	virtual void HandleInput();
	virtual void Update();
	virtual void Reset(std::string _level = "game-clear.csv");
	virtual void Draw() {
		if (nullptr != pGfx && nullptr != pGOM) {
			pGfx->DrawBackground(background);
			pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context);
			if (change_state) {
				if (mpOwner->StateSize() > 1) {
					mpOwner->PopState();
				}
				else {
					mpOwner->ChangeState(IntroState::GetInstance());
				}
			}
		}
		pFRC->EndFrame();
	};
	static GameClearState* GetInstance() {
		return &mGameClearState;
	}
private:
	int index = -1;
	static GameClearState mGameClearState;
	std::vector<std::string> credits = 
	{"GameCredits_01.png","GameCredits_02.png", "GameCredits_03.png", 
		"GameCredits_04.png","GameCredits_05.png" ,"GameCredits_06.png",
	"GameCredits_07.png","GameCredits_08.png" };
};

