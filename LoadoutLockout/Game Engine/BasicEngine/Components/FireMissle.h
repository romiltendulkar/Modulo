#pragma once
#include "stdafx.h"
#include "Component.h"
#include "..\GameObject.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FireMissle.h
Purpose: handles the act of fireing a missle, determining speed and missle
type on creation.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class FireMissle : public Component
{
public:
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual FireMissle* clone() {
		FireMissle* fm = new FireMissle();
		fm->init_delay;
		fm->base_reload_time = base_reload_time;
		fm->fired = fired;
		fm->count = count;
		fm->reload_time = reload_time;
		fm->missle_type = missle_type;
		fm->m_name = m_name;
		fm->speed = speed;
		fm->random = random;
		return fm;
	}
	FireMissle();
	~FireMissle();
	int init_delay;
	Uint32 reload_time;
	Uint32 base_reload_time;
	int jump_size;
	Uint32 count;
	bool fired;
	std::string m_name;
	OBJECT_TYPE missle_type;
	float speed;
	bool random;
};

