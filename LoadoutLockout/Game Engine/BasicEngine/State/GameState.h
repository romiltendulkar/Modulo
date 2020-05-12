#ifndef ___GAME_STATE___
#define ___GAME_STATE___
#include "stdafx.h"
#include "..\GameEngine.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Managers\InputFacade.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\GameObjectFactory.h"
#include "..\BasicEngine\Managers\Resource_Manager.h"
#include "../Audio/Audio_Manager.h"
#include "lua.hpp"
#include "..\Events\PauseEvent.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameState.h
Purpose: This is the base class for all fo the different kinds of game states
and details the expected functions.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/10/2018
- End Header --------------------------------------------------------*/

class Physics_Manager;
class CollisionManager;
class EventManager;
class Resource_Manager;
class GameObject_Manager;
class Physics_Manager;
class CollsionManager;
class ParticleManager;
class Audio_Manager;

enum STATE_TYPE {
	INTRO,
	PLAY,
	PAUSE_STATE,
	GAMEOVER,
	GAMECLEAR,
	QUIT_STATE,
	UTILITY_STATE,
	NARATIVE_STATE,
	NUM_STATES
};
extern GraphicsManager* pGfx;
extern FrameRate_Controller* pFRC;
extern InputFacade* pIF;
extern GameObjectFactory * pGOF;


class GameState
{
public:
	Resource_Manager * pRM;
	GameObject_Manager * pGOM;
	Physics_Manager * pPhysics;
	CollisionManager * pCM;
	EventManager * pEM;
	lua_State *lState;
	ParticleManager* pPM;
public:

	GameState() : mType(NUM_STATES), mpOwner(NULL) {};
	GameState(STATE_TYPE _type) : mType(_type), complete(false) {};
	~GameState() {};

	STATE_TYPE mType;
	GameEngine* mpOwner;
	STATE_TYPE GetStateType() { return mType; };
	std::string background;
	std::string back_music;
	std::string cam_type;

	virtual void Init(GameEngine* _owner, std::string level = "") { };
	virtual void CleanUp() {};

	virtual void Pause() {};
	virtual void Resume() {};

	virtual void HandleInput() {};
	virtual void Update() {};
	virtual void Draw() = 0;
	virtual void LoseLife() {};
	virtual void AddLife() {};
	virtual void ButtonCallback(unsigned int id, std::string str) {};
	virtual int GetLives() { return 0; };
	virtual void Reset(std::string _level = "") {};
	bool complete,paused;
	float w, h;
	bool change_state, start_trans, end_trans;
	int score;
	std::string _level, next_level,button;
	float transition_time = 0.75;
	float current_trans_time;
};

#endif
