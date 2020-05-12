#include "stdafx.h"
#include "StompAbility.h"


void StompAbility::ExecuteAbility(BUTTON AbilityDir, PRESS_TYPE type)
{
	sprite_file = "StompAbility.png";
}

StompAbility::StompAbility() : Ability(STOMP, 0, false)
{
}


StompAbility::~StompAbility()
{
}
