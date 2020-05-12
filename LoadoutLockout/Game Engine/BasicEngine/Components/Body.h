#ifndef _BODY
#define _BODY
#include "stdafx.h"
#include "Component.h"
#include "../GameObject.h"
#include "..\Managers\CollisionManager.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.h
Purpose: Definition of the Body component. Body is used for most
physics and collisions.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class GameObject;

using namespace glm;
class Body : public  Component
{
public:
	Body(float m = 0.f) : mass(m), Component(BODY) {
		pos.x = pos.y = prev_pos.x = prev_pos.y = 0;
		vel.x = vel.y = 0;
		if (mass <= 0) {
			inverse_mass = 1000000;
		}
		else {
			inverse_mass = 1.0f / mass;
		}
		match_transform = true;
		paused = false;
		ridged = true;
		mpOwner = nullptr;
		shape_name = "rect";
		mu = 1.f;
		physics_obj = true;
		movable = true;
		elasticity = 1.f;
	};

	bool ridged;
	vec4 pos;
	vec4 prev_pos;
	vec4 vel;
	vec4 acc;
	vec4 total;
	vec4 forces;
	float mass, inverse_mass;
	Shape* shape;
	std::string shape_name;
	float mu;
	float elasticity;
	bool physics_obj;
	bool movable;
	bool match_transform;
	virtual Body* clone() {
		Body* body = new Body();
		body->shape = shape->clone();
		body->shape->SetBody(body);
		body->mass = mass;
		body->inverse_mass = inverse_mass;
		body->shape_name = shape_name;
		body->mu = mu;
		body->pos = pos;
		body->physics_obj = physics_obj;
		body->paused = paused;
		body->ridged = ridged;
		body->movable = movable;
		body->elasticity = elasticity;
		body->match_transform= match_transform;
		return body;
	};
	~Body();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Init();
	void Integrate(float grav, float dt);
	
};

#endif