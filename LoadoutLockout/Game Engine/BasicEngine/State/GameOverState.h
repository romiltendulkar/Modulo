#pragma once
#include "stdafx.h"
#include "GameState.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameOverState.h
Purpose: This is the game over state definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/
class GameOverState :
	public GameState
{
public:
	GameOverState();
	~GameOverState();
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void Pause();
	virtual void Resume();
	virtual void Draw() {
		if (nullptr != pGfx && nullptr != pGOM) {
			pGfx->DrawBackground(background);
			pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context);
		}
		pFRC->EndFrame();
	};
	virtual void ButtonCallback(unsigned int id, std::string str);

	virtual void HandleInput();
	virtual void Update();
	virtual void Reset(std::string _level = "game-over.csv");
	static GameOverState* GetInstance() {
		return &mGameOverState;
	}
private:
	static GameOverState mGameOverState;
};


