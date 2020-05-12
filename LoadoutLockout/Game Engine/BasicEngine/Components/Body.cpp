#include "stdafx.h"
#include "Body.h"
#include "definitions.h"
#include "Transform.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\MoveEvent.h"
#include "..\Events\PauseEvent.h"
#include "..\Events\ResetPlayerEvent.h"
#include "..\Events\Event.h"
#include "..\Events\DestroyEvent.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Managers\EventManager.h"
#include "..\State\GameState.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.cpp
Purpose: Implementation of the Body component. Body is used for most 
physics and collisions.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/


extern GraphicsManager * pGfx;
extern GameState* current_state;
extern FrameRate_Controller *pFRC;

using namespace nlohmann;

Body::~Body()
{
}

void Body::Update()
{
	Controller* pC = static_cast<Controller*>(mpOwner->GetComponent(CONTROLLER));
	if (nullptr != pC) {
		pGfx->FollowPlayer(pos.x, pos.y);
	}
}

void Body::Serialize(std::string obj)
{
	json j_body = json::parse(obj);
	mass = SafeGet<float>(j_body, "mass", mass);
	if (mass <= 0) {
		inverse_mass = 1000000;
	}
	else {
		inverse_mass = 1.0f / mass;
	}
	shape_name = SafeGet<std::string>(j_body, "shape", shape_name);
	if ("circle" == shape_name) {
		
		Circle* circle = new Circle();
		circle->radius = SafeGet<float>(j_body, "radius", circle->radius);
		circle->pOwnerBody = this;
		shape = circle;
	}
	 else if("rect" == shape_name) {

		Rect* rect = new Rect();
		rect->width = SafeGet<float>(j_body, "width", rect->width);
		rect->height = SafeGet<float>(j_body, "height", rect->height);
		rect->pOwnerBody = this;
		shape = rect;
	}
	 else {
		shape = nullptr;
	}
	ridged = SafeGet<bool>(j_body, "ridged", ridged);
	paused = false;
	subscribe = SafeGet<bool>(j_body, "subscribe", subscribe);
	physics_obj = SafeGet<bool>(j_body, "physics", physics_obj);
	movable = SafeGet<bool>(j_body, "movable", movable);
	match_transform = SafeGet<bool>(j_body, "match", match_transform);
	elasticity = SafeGet<float>(j_body, "elasticity", elasticity);
	mu = SafeGet<float>(j_body, "mu", mu);
}

void Body::Integrate(float grav, float dt)
{
	if (!paused && movable&& mpOwner->visable) {

		prev_pos = pos;
				
		if (physics_obj) {
			acc.y = acc.y - (grav  * dt * inverse_mass);
			vel.y = vel.y + forces.y + acc.y;
			
		}
		else { 
			acc.y = acc.y + (forces.y / mass);
			vel.y = vel.y + (dt * acc.y);
		}
		acc.x = dt * acc.x *inverse_mass;
		vel.x = vel.x + acc.x  +forces.x ;
		
		vel.x = vel.x * 0.8;
		pos.x = pos.x + vel.x;
		pos.y = pos.y + vel.y ;
		Transform *pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
				
		if (nullptr != pT) {
			pT->SetX(pos.x);//this ties transform pos to body
			pT->SetY(pos.y);
			float min_y = -current_state->h/2.f;
			float max_y = current_state->h;
			float min_x = -current_state->w / 2.f;
			float max_x = current_state->w / 2.f;
			if (pos.x < min_x || pos.x > max_x ||
				pos.y < min_y || pos.y > max_y) {
				current_state->pEM->SendEvent(new DestroyEvent(), mpOwner);
			}
		}
		forces.x = 0;
		forces.y = 0;
	}
}

void Body::Init()
{
	Transform *pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	if (nullptr != pT) {
		pos.x = pT->GetX();
		pos.y = pT->GetY();
		if (match_transform) {
			if (shape_name == "circle") {
				Circle* circle = static_cast<Circle*>(shape);
				circle->radius = pT->scaleXY.x / 2.f;
			}
			else {
				Rect* rect = static_cast<Rect*>(shape);
				rect->height = pT->scaleXY.y;
				rect->width = pT->scaleXY.x;
			}
		}
	}
	if (subscribe) {
		current_state->pEM->Subscribe(MOVE, mpOwner);
	}
	vel = vec4(0.0);
}

void Body::HandleEvent(Event * pevent)
{
	EVENT_TYPE type = pevent->mType;
	Transform *pT;
	Player* pP;
	bool move = true;
	MoveEvent* mE = nullptr;
	CollisionEvent* cE = nullptr;
	Rect* rec;
	Circle* cir;
	switch (type) {
	case MOVE: 
		if (!mpOwner->visable&& physics_obj) break;
		mE = static_cast<MoveEvent*>(pevent);
		pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		if (nullptr != pT && move) 
		{
			forces.x += mE->xPos;
			forces.y += mE->yPos;
			pT->SetAngle(mE->angle);
			
		}
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case COLLIDE:
		break;
	}
}
