#pragma once
#include "stdafx.h"
#include "Component.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LevelState.h
Purpose: used to help track the current level state and ensure it is being
tracked properly.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class LevelState :public Component
{
public:
	unsigned int lives;
	unsigned int level_score;
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	LevelState();
	~LevelState();
	virtual LevelState* clone() {
		LevelState* ls = new LevelState();
		ls->subscribe;
		return ls;
	}
};

