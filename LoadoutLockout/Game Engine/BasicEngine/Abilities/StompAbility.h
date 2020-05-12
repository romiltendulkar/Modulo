#pragma once
#include "stdafx.h"
#include "Ability.h"
class StompAbility :
	public Ability
{
public:
	virtual void ExecuteAbility(BUTTON AbilityDir, PRESS_TYPE type = _PRESS_);
	StompAbility();
	~StompAbility();
	
};

