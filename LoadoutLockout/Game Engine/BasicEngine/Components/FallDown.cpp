#include "stdafx.h"
#include "FallDown.h"
#include "Component.h"
#include "Transform.h"
#include "../GameObject.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Events\MoveEvent.h"
#include "..\Managers\EventManager.h"
#include "..\State\GameState.h"
#include "..\Events\CollisionEvent.h"
//TODO FIX

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.cpp
Purpose: handles the up down enemy movement.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

extern FrameRate_Controller* pFRC;
extern GraphicsManager* pGfx;
extern GameState* current_state;

using namespace nlohmann;

FallDown::FallDown() : Component(FALLDOWN)
{
	time  = count = 0;
	speed = 0.f;
	go_up = false;
}

void FallDown::Init()
{
	body  = static_cast<Body*>(mpOwner->GetComponent(BODY));
	body->vel.y = -speed;
	paused = false;
}

void FallDown::Update()
{
	if (!paused) {
		if (go_up || rest) {
		Uint32 frame_time = pFRC->GetFrameTime();
		float dt = pFRC->GetDT();
		count += frame_time;

			if (count >= time && !rest) {
				go_up = false;
				count = 0;
			}
			else if (count >= down_time && rest) {
				rest = false;
				count = 0;
			}
		}
		if (nullptr != body) {
			if (go_up) {
				if (current_state->pEM != nullptr) {
					float dt = pFRC->GetDT();
					float dy = glm::distance(home_point, body->pos) /(time  * dt);
					current_state->pEM->SendEvent(new MoveEvent(0, dy, 0), mpOwner);
				}
			}
			else if(!rest){
					
			}
			
		}
	} 
	
}

void FallDown::Serialize(std::string obj)
{
	json j_ud;
	j_ud = json::parse(obj);
	speed = SafeGet<float>(j_ud, "speed", speed);
	time = SafeGet<Uint32>(j_ud, "time", time);

}

void FallDown::HandleEvent(Event * pevent)
{
	EVENT_TYPE type = pevent->mType;
	CollisionEvent* cE;
	Body* other;
	switch (type) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1;
		if (other == body) {
			other = cE->b2;
		}
		rest = true;
		go_up = true;
		break;
	case PAUSE:
		paused = !paused;
		break;
	}
}
