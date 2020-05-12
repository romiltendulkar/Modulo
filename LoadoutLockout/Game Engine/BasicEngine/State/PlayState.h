#pragma once
#include "stdafx.h"
#include "GameState.h"

#include "..\Managers\EventManager.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Components\definitions.h"


/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: PlayState.h
Purpose: This is the play state definition, the play state tracks the 
win and lose conditions as well as basic state functionality.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/

class PlayState :
	public GameState
{
public:
	PlayState();
	~PlayState();
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void ButtonCallback(unsigned int id, std::string str);

	virtual void Pause();
	virtual void Resume();

	virtual void HandleInput();
	virtual void Update();
	virtual void Draw();
	virtual void Reset(std::string _level = "");
	static PlayState* GetInstance() {
		return &mPlayState;
	};
	virtual void LoseLife() { --lives; };
	virtual int GetLives() { return lives; };
private:
	void DisplayVictory();
	static PlayState mPlayState;
	float dt, level_width,level_height;
	int current_level_index;
	std::vector<std::string> levels;
	int lives;
	bool win, move_to_next;
	int total_friends = 12;
	int saved_friends = 0;
	int prev_lives = 3;
	glm::vec4 prev_pos;
};

