#include "stdafx.h"
#include "Bullet.h"
#include "Transform.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\GameObject.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\Events\DamageEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Managers\EventManager.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Bullet.cpp
Purpose: Implementation of the Bullet component. Mainly used to help 
track and clean up objects that go off the screen and handle the
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

using namespace nlohmann;
extern FrameRate_Controller* pFRC;
extern GraphicsManager* pGfx;
extern GameState* current_state;

Bullet::Bullet() : Component(BULLET)
{
	rotate = false;
	subscribe = false;
}


Bullet::~Bullet()
{
}

void Bullet::Update()
{
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	
	
	if (pT->GetY() > pGfx->GetWindowHeight() / 2.f) {
		mpOwner->active = false;
	}else if (pT->GetY() < -(pGfx->GetWindowHeight() / 2.f) ){

		mpOwner->active = false;
		
	}
	if (!paused && rotate && mpOwner->active) {
		float dt = pFRC->GetDT();
		pT->angle += 0.25f*dt;
		if (pT->angle > 180) pT -= 360;
	}
}

void Bullet::HandleEvent(Event * pevent)
{
	CollisionEvent* cE;
	Body* other;
	switch (pevent->mType) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1;
		if (other->mpOwner == mpOwner) {
			other = cE->b2;
		}
		if (!other->ridged) break;
		if (other->mpOwner->GetComponent(ENEMY) == nullptr) {		
			current_state->pEM->SendEvent(new DestroyEvent(), mpOwner);
			current_state->pEM->SendEvent(new DestroyEvent(), other->mpOwner);
		}
		break;
	case DESTROY:
		mpOwner->active = false;
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}

void Bullet::Serialize(std::string obj)
{
	json j_bullet = json::parse(obj);
	rotate = SafeGet<bool>(j_bullet, "rotate", rotate);
	subscribe = SafeGet<bool>(j_bullet, "subscribe", subscribe);
	paused = false;
}
