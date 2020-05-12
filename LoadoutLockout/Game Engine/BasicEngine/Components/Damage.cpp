#include "stdafx.h"
#include "Damage.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Damage.cpp
Purpose: The damage component, used to track damage.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

using namespace nlohmann;
void Damage::Update()
{
}

void Damage::Serialize(std::string obj)
{
	json j_dmg = json::parse(obj);
	subscribe = SafeGet<bool>(j_dmg, "subscribe", subscribe);
	base_damage = SafeGet<unsigned int>(j_dmg, "damage", base_damage);
	tmp_damage = base_damage;
}

void Damage::HandleEvent(Event * pevent)
{
}

unsigned int Damage::GetDamage()
{
	if (tmp_damage > base_damage)
		return tmp_damage;
	return base_damage;
}

void Damage::BoostDamage(unsigned int mul)
{
	tmp_damage = base_damage * mul;
}

void Damage::Reset()
{
	tmp_damage = base_damage;
}

Damage::Damage() : Component(DAMAGE)
{
	base_damage = tmp_damage = 0;
	subscribe = false;
}

Damage::~Damage()
{
}
