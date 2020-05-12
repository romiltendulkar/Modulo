#pragma once
#include "stdafx.h"
#include "Component.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Damage.h
Purpose: The Damage component , used to track the base and current damage 
of whatever object has this component.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class Damage : public Component
{
public:
	unsigned int base_damage;
	unsigned int tmp_damage;
	virtual Damage* clone() {
		Damage* dmg = new Damage();
		dmg->base_damage;
		dmg->tmp_damage;
		return dmg;
	}
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	unsigned int GetDamage();
	void BoostDamage(unsigned int mul);
	void Reset();
	Damage();
	~Damage();
};

