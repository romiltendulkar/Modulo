#include "stdafx.h"
#include "../Components/definitions.h"
#include "Ability.h"
#include "..\Components\definitions.h"
#pragma once
class AbilityFactory
{
public:
	static Ability* CreateAbility(std::string type);
	static Ability* CreateAbility(A_TYPE type);
	static A_TYPE GetType(std::string t);
	static void Init();
private:
	AbilityFactory();
	~AbilityFactory();

};

