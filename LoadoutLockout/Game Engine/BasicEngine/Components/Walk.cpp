#include "stdafx.h"
#include "Walk.h"
#include "Transform.h"
#include "../GameObject.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Events\MoveEvent.h"
#include "..\Managers\EventManager.h"
#include "..\State\GameState.h"
#include "..\Events\CollisionEvent.h"
#include "Body.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SideToSide.cpp
Purpose: handles the character moving side to side.
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

Walk::Walk() : Component(WALK)
{
	speed = 0.f;
	subscribe =false;
}


Walk::~Walk()
{
}

void Walk::Update()
{
	if (!paused) {
		have_turn = false;
		Uint32 frame_time = pFRC->GetFrameTime();
		float dt = pFRC->GetDT();
		current_state->pEM->SendEvent(new MoveEvent(speed, 0, 0), mpOwner);
		
	}
}

void Walk::Serialize(std::string obj)
{
	json j_ss;
	j_ss = json::parse(obj);
	subscribe = SafeGet<bool>(j_ss, "subscribe", subscribe);
	speed = SafeGet<float>(j_ss, "speed", speed);
}

void Walk::HandleEvent(Event * pevent)
{
	EVENT_TYPE type = pevent->mType;
	CollisionEvent* cE;
	Body* other;		//unreferenced variables
	Rect* rect;
	Circle* cir;
	float bottom;
	switch (type) {
	case COLLIDE:
		if (have_turn) break;
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1;
		if (other == body)
			other = cE->b2;
		if (!other->ridged) break;
		rect = static_cast<Rect*>(body->shape);
		if (rect == nullptr) {
			cir = static_cast<Circle*>(body->shape);
			if ((cE->x > body->pos.x || cE->x < body->pos.x) 
				&& !(body->prev_pos.y - cir->radius/2.f > cE->y) ){
				speed = -speed;
				have_turn = true;
				break;
			}
			break;
		}
		bottom = body->pos.y - rect->height / 2.f;
		if (cE->n.y != 0) 
			break;
		if (cE->n.x != 0 && bottom > other->pos.y )
			break;
		speed = -speed;
		have_turn = true;
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}

void Walk::Init()
{
	body = static_cast<Body*>(mpOwner->GetComponent(BODY));
	body->vel.x = speed;
	paused = false;
}
