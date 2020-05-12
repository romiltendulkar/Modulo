#include "stdafx.h"
#include "DoppleJump.h"

#include "..\Components\Controller.h"
#include "..\Components\Player.h"
DoppleJump :: DoppleJump(int JumpInc) : Ability(DOPPLE, 2, true)
{
	jump_increment = JumpInc;
	is_Passive = true;
	sprite_file = "DoubleJump.png";
}

void DoppleJump::DestroyAbility() {
	abOwner->DecrementCounter(jump_increment);
}

DoppleJump :: ~DoppleJump()
{
	abOwner = nullptr;
}

void DoppleJump::ExecuteAbility(BUTTON AbilityDir, PRESS_TYPE type)
{
	//Call this when the ability is created
	//Writing the code below into the constructor returns null
	if (!incremented) {
		if (nullptr != abOwner){
			abOwner->IncrementCounter(jump_increment);
			incremented = true;
		}
	}

}