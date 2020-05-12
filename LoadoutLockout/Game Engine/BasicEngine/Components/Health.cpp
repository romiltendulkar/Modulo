#include "stdafx.h"
#include "Health.h"

#include "Damage.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\GameObject.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Health.cpp
Purpose: Handles the health of whatever object has the the component.
When health hits 0 the object is destroyed.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

extern GameState* current_state;
extern EventManager* pEM;

using namespace nlohmann;

void Health::Update()
{
}

void Health::Serialize(std::string obj)
{
	json j_health = json::parse(obj);
	base_health = SafeGet<int>(j_health, "health", base_health);
	subscribe = SafeGet<bool>(j_health, "subscribe", subscribe);
	curr_health = base_health;
}

void Health::HandleEvent(Event * pevent)
{
	CollisionEvent* cE;
	switch (pevent->mType) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		GameObject* other = cE->b1->mpOwner;
		if (other == mpOwner) {
			other = cE->b2->mpOwner;
		}
		Damage* dmg = static_cast<Damage*>(other->GetComponent(DAMAGE));
		if (nullptr != dmg) {
			curr_health -= dmg->GetDamage();
		}
		if (curr_health <= 0) {
			current_state->pEM->SendEvent(new DestroyEvent(), mpOwner);
		}
		break;
	}
}

Health::Health() : Component(HEALTH)
{
	curr_health = base_health = 0;
	subscribe = false;
}


Health::~Health()
{
}
