#include "stdafx.h"
#include "FireMissle.h"

#include "..\Events\FireMissleEvent.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Managers\EventManager.h"
#include "..\State\GameState.h"
#include "..\GameObjectFactory.h"
#include "..\State\GameState.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FireMissle.cpp
Purpose: handles the act of fireing a missle, determining speed and missle
type on creation.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/


extern FrameRate_Controller* pFRC;
extern GameState* current_state;

using namespace nlohmann;

void FireMissle::Update()
{
	if (paused || !mpOwner->visable)return;
	if (!fired) {
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		FireMissleEvent* pF = new FireMissleEvent();
		pF->x = pT->GetX();
		if (speed > 0) {
			pF->y = pT->GetY() + (pT->scaleXY.y / 1.75f) +10;
		}
		else {
			pF->y = pT->GetY() - (pT->scaleXY.y / 1.75f) -10;
		}
		current_state->pEM->SendEvent(pF, mpOwner);
		reload_time = base_reload_time;
	} else if(fired && !paused){
		Uint32 frame_time = pFRC->GetFrameTime();
		count += frame_time;
		if (init_delay > 0) {
			init_delay -= frame_time;
			count = 0;
			return;
		} 
		if (count >= reload_time) {
			fired = false;
			count = 0;
		}
	}
}

void FireMissle::Serialize(std::string obj)
{
	
	json j_fire = json::parse(obj);
	speed = SafeGet<float>(j_fire, "speed", speed);
	base_reload_time = SafeGet<Uint32>(j_fire, "reload-time", base_reload_time); 
	reload_time = base_reload_time;
	
	
	m_name = SafeGet<std::string>(j_fire, "missle-name", m_name); 
	subscribe = SafeGet<bool>(j_fire, "subscribe", subscribe);
	random = SafeGet<bool>(j_fire, "random", random);
	init_delay = SafeGet<int>(j_fire, "init_delay", init_delay);
	fired = false;
	missle_type = SafeGet(j_fire, "m_type", missle_type);
	paused = false;
}

void FireMissle::HandleEvent(Event * pevent)
{
	FireMissleEvent* pFME;
	switch (pevent->mType) {
	case FIRE:
		
		if (!fired) {
			pFME = static_cast<FireMissleEvent*>(pevent);
			fired = true;

			GameObject* missle = current_state->pGOM->GetUsedObject(missle_type);
			if (nullptr == missle) {
				missle = GameObjectFactory::LoadObject(m_name.c_str());
				missle->active = true;
				missle->visable = true;
				current_state->pGOM->PushGameObject(missle);
			}
			else {
				int i = 0;
				missle->active = true;
				missle->visable = true;
			}
			Transform* pmTrans = static_cast<Transform*>(missle->GetComponent(TRANSFORM));
			pmTrans->SetX(pFME->x);
			Body* pmBody = static_cast<Body*>(missle->GetComponent(BODY));
			
			if (speed > 0) {
				pmTrans->SetY(pFME->y + pmTrans->scaleXY.y/1.75 );
				
			}
			else {
				pmTrans->SetY(pFME->y - pmTrans->scaleXY.y/1.75 );
				
			}
			pmBody->Init();
			pmBody->acc.y = speed*.1;
			pmBody->vel.y = speed;
		
		}
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}

FireMissle::FireMissle() : Component(FIRE_MISSLE)
{
	base_reload_time = 0;
	init_delay = 0;
	random = false;
	m_name = "";
	subscribe = false;
	speed = 0.f;
	fired = false;
	missle_type = OBJECT_TYPE(2);
	paused = false;
}


FireMissle::~FireMissle()
{
}
 
