#include "stdafx.h"
#include "JumpDown.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Events\MoveEvent.h"
#include "..\Managers\EventManager.h"
#include "..\State\GameState.h"


/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Jumpdown.cpp
Purpose: Handles the when a object should suddenly jumpdown.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

extern FrameRate_Controller* pFRC;
extern GameState* current_state;

JumpDown::JumpDown() : Component(JUMP_DOWN)
{
	jump_size = 0;
	time = 10000000;
	subscribe = false;
}


JumpDown::~JumpDown()
{
}


void JumpDown::Serialize(std::string obj)
{
	json j_jump = json::parse(obj);

	jump_size = SafeGet<int>(j_jump, "jump_size", jump_size);
	time = SafeGet<Uint32>(j_jump, "time", time);
	paused = false;
	subscribe = SafeGet<bool>(j_jump, "subscribe", subscribe);
}

void JumpDown::HandleEvent(Event * pevent)
{
	EVENT_TYPE type = pevent->mType;
	//Transform *pT; //unreferenced value
	MoveEvent* mE = nullptr;
	switch (type) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}

void JumpDown::Update()
{
	if (!paused) {
		Uint32 frame_time = pFRC->GetFrameTime();
		count += frame_time;
		if (count >= time ) {
			current_state->pEM->SendEvent(new MoveEvent(0, -jump_size, 0), mpOwner);
			count = 0;
		}
	}
}
