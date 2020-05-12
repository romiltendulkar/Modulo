#include "stdafx.h"
#include "LevelState.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\GoalEvent.h"
#include "Audio.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LevelState.cpp
Purpose: used to help track the current level state and ensure it is being
tracked properly.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

extern GameState* current_state;
using namespace nlohmann;

void LevelState::Update()
{
}

void LevelState::Serialize(std::string obj)
{
	json j_jump = json::parse(obj);
	subscribe = SafeGet<bool>(j_jump, "subscribe", false);
}

void LevelState::HandleEvent(Event * pevent)
{
	std::string life_name = "";
	CollisionEvent* pCE = nullptr;
	Controller* pC;
	Audio* pAud;
	GameObject* other;
	switch (pevent->mType) {
	case COLLIDE:
		pCE = static_cast<CollisionEvent*>(pevent);
		other = pCE->b1->mpOwner;
		if (other == mpOwner) {
			other = pCE->b2->mpOwner;
		}
		pC = static_cast<Controller*>(other->GetComponent(CONTROLLER));
		if (nullptr != pC && pCE->depth > 15.f) {
			pAud = static_cast<Audio *>(mpOwner->GetComponent(AUDIO));
			if (nullptr != pAud)
				pAud->ExecuteAudio();
			//animateions for end of level should be triggered here go here
			current_state->complete = true;
		}
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
	
}

LevelState::LevelState() : Component(LEVELSTATE)
{
	subscribe = false;
}


LevelState::~LevelState()
{
}
