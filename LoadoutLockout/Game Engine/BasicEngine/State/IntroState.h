#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "..\Managers\NarrativeManager.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: IntroState.h
Purpose: This is the intro state definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/

class IntroState :public GameState
{
public:
	IntroState();
	~IntroState();
	
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void Pause();
	virtual void Resume();
	virtual void ButtonCallback(unsigned int id, std::string str);
	virtual void HandleInput();
	virtual void Update();
	virtual void Reset(std::string _level = "");
	static IntroState* GetInstance() {
		return &mIntroState;
	}
	virtual void Draw();
private:
	static IntroState mIntroState;
	int script;
	std::string final_background;
	bool displayedDigiLogo =false;
	bool team_logo = false;
	bool fmod_logo = false;
	bool done = false;
	float time = 2.3f;
	float reset_time =.5f;
};

