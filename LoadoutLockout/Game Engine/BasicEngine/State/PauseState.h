#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "..\GameEngine.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PauseState.h
Purpose: This is the pause state definintion.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/
class PauseState :
	public GameState
{
public:
	PauseState();
	~PauseState();
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void Pause();
	virtual void Resume();
	virtual void ButtonCallback(unsigned int id, std::string str);

	virtual void HandleInput();
	virtual void Update();
	virtual void Draw();
	virtual void Reset(std::string _level = "pause.csv");
	static PauseState* GetInstance() {
		return &mPauseState;
	}
private:
	static PauseState mPauseState;
	glm::vec4 prev_pos;
};

