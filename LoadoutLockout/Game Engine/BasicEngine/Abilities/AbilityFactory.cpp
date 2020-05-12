#include "stdafx.h"
#include "AbilityFactory.h"
#include "DashAbility.h"
#include "DoppleJump.h"
#include "StompAbility.h"
#include "ProtectAbility.h"


Ability * AbilityFactory::CreateAbility(A_TYPE type) {
	switch (type)
	{
	case DASH:
		return new DashAbility();
	case DOPPLE:
		return new DoppleJump();
	case STOMP:
		return new StompAbility();
	default:
		return nullptr;
	}
}

Ability * AbilityFactory::CreateAbility(std::string type)
{
	if (type == "dash") {
		return new DashAbility();
	}
	else if (type == "spike") {
		return new ProtectAbility();
	}
	else if (type == "double") {
		DoppleJump *NewDJ = new DoppleJump;
		NewDJ->ExecuteAbility(ABILITY_TOP);
		return NewDJ;
	}
	else if (type == "stomp") {
		return new StompAbility();
	}
	return nullptr;
}
A_TYPE AbilityFactory::GetType(std::string t) {
	if (t == "dash") {
		return DASH;
	}
	else if (t == "spike") {
		return SPIKER;
	}
	else if (t == "double") {
		return DOPPLE;
	}
	else if (t == "stomp") {
		return STOMP;
	}


	return NUM_A_TYPE;
}

void AbilityFactory::Init()
{
}

AbilityFactory::AbilityFactory()
{
}


AbilityFactory::~AbilityFactory()
{
}
