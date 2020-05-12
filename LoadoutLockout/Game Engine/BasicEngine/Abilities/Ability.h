#pragma once
#include "stdafx.h"
#include "../Components/definitions.h"
#include "../Managers/InputFacade.h"

class Player;

class Ability
{
public:
	//Ability functions
	Ability(A_TYPE type, int cool, bool IsPassive)
	{
		ab_type = type;
		cooldown = cool;
		cooldowntimer = 0;
		abOwner = nullptr;
		is_Passive = IsPassive;
		sprite_file = "";
		use = "";
	}
	~Ability() 
	{
	
	}
	virtual void Update(float DT)
	{
		
	}
	virtual void DestroyAbility() {};
	virtual void ExecuteAbility(BUTTON AbilityDir, PRESS_TYPE type = _TRIGGER_) {};
	//variable to store ability type
	A_TYPE ab_type;
	int cooldown;
	float cooldowntimer;
	Player* abOwner;
	bool is_Passive;
	std::string sprite_file;
	std::string use;
};