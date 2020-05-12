#pragma once
#include "stdafx.h"
#include "Ability.h"

class DoppleJump : public Ability
{
public:
	DoppleJump(int JumpInc = 1);
	~DoppleJump();
	virtual void DestroyAbility();
	virtual void ExecuteAbility(BUTTON AilityDir, PRESS_TYPE type= _TRIGGER_);
	int jump_increment;
	bool incremented = false;
};