#include "stdafx.h"
#include "Physics_Manager.h"
#include "../Components/Body.h"
#include "../GameObject.h"
#include "EventManager.h"
#include "../Events/CollisionEvent.h"
#include "CollisionManager.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\State\GameState.h"


/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Physics_Manager.h
Purpose: The Physics Manager handles the movement of the objects with a
Body component, checks for collisions, and will do reflections.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

extern GameState* current_state;

Physics_Manager::Physics_Manager()
{
}


Physics_Manager::~Physics_Manager()
{
}

void Physics_Manager::Update(float dt)
{
	//Physics math here
	//do reflections here
	current_state->pCM->Clear();

	std::vector<GameObject*> objects = current_state->pGOM->GetObjects();
	for (auto go = objects.begin(); go != objects.end(); ++go) {
		Body * pB = dynamic_cast<Body*>((*go)->GetComponent(BODY));
		if (nullptr != pB) {
			if (pB->movable) {
				pB->mpOwner = *go;
				pB->Integrate(10, dt);
			}
		}
	}
	
	
	//collision stuff here
	for (auto go1 = objects.begin(); go1 != objects.end(); ++go1) {
		Body* b1 = static_cast<Body*>((*go1)->GetComponent(BODY));
		for (auto go2 = go1 + 1; go2 != objects.end(); ++go2) {
			
			Body* b2 = static_cast<Body*>((*go2)->GetComponent(BODY));	
			if (!(*go2)->active || !(*go1)->active) continue;
			if (b1 != nullptr && b2 != nullptr) {
				if ( (b1->movable || b2->movable) && ( (*go1)->visable || (*go2)->visable ) ) {
					current_state->pCM->CheckCollision(b1->shape, b1->pos.x, b1->pos.y, b2->shape, b2->pos.x, b2->pos.y);
				}
			}
		}
	}
	for (auto contact : current_state->pCM->mContacts) {

		
		ResolveCollision(contact);
		CorrectPositions(contact);

		CollisionEvent* cevent = new CollisionEvent();
		cevent->b1 = contact->pS1->pOwnerBody;
		cevent->b2 = contact->pS2->pOwnerBody;
		cevent->x = contact->x;
		cevent->y = contact->y;
		cevent->n = contact->n;
		cevent->depth = contact->depth;
		current_state->pEM->SendEvent(cevent, contact->pS1->pOwnerBody->mpOwner);
		current_state->pEM->SendEvent(cevent, contact->pS2->pOwnerBody->mpOwner);

	}
	current_state->pCM->Clear();
}


void Physics_Manager::ResolveCollision(Contact * contact)
{
	
	if (!contact->pS1->pOwnerBody->ridged || !contact->pS2->pOwnerBody->ridged) return;
	vec4 rel_vel = contact->pS1->pOwnerBody->vel - contact->pS2->pOwnerBody->vel;
	float vel_along_normal = dot(rel_vel, contact->n);

	
	if (vel_along_normal > 0)
		return;

	float e = fminf(contact->pS1->pOwnerBody->elasticity, contact->pS2->pOwnerBody->elasticity);

	float j = -(1 + e) * vel_along_normal;
	j /= (contact->pS1->pOwnerBody->inverse_mass + contact->pS2->pOwnerBody->inverse_mass);

	vec4 impulse = j * contact->n;
	if (contact->pS1->pOwnerBody->physics_obj && contact->pS1->pOwnerBody->movable){
		contact->pS1->pOwnerBody->vel += contact->pS1->pOwnerBody->inverse_mass * impulse;
		contact->pS1->pOwnerBody->acc -= (contact->pS1->pOwnerBody->acc * contact->n);
	}

	if (contact->pS2->pOwnerBody->physics_obj && contact->pS2->pOwnerBody->movable) {
		contact->pS2->pOwnerBody->vel -= contact->pS2->pOwnerBody->inverse_mass * impulse;
		contact->pS2->pOwnerBody->acc += (contact->pS2->pOwnerBody->acc * contact->n);
	}

}

void Physics_Manager::CorrectPositions(Contact * contact)
{
	
	if (!contact->pS1->pOwnerBody->ridged || !contact->pS2->pOwnerBody->ridged) return;

	float percent = 1.8f; // 80%
	float slop = 0.1;

	float total_inverse = contact->pS1->pOwnerBody->inverse_mass + contact->pS2->pOwnerBody->inverse_mass;
	vec4 correction = (contact->depth  / total_inverse)* percent * contact->n;
	if (contact->pS1->pOwnerBody->physics_obj && contact->pS1->pOwnerBody->movable) {
		vec4 cor = contact->pS1->pOwnerBody->inverse_mass*correction;
		
		contact->pS1->pOwnerBody->pos += cor;
	}

	if (contact->pS2->pOwnerBody->physics_obj && contact->pS2->pOwnerBody->movable) {
		vec4 cor = contact->pS2->pOwnerBody->inverse_mass*correction;
		contact->pS2->pOwnerBody->pos -= cor;
	}
}

float Physics_Manager::FrictionFunction(float mu1, float mu2)
{
	float sq_mu = mu1 * mu1 + mu2 * mu2;
	return sqrtf(sq_mu);
}

void Physics_Manager::TestPotentialMiss(Body * b1, Body * b2)
{

}
