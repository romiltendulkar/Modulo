#include "stdafx.h"
#pragma once
#include "Component.h"
#include "../Abilities/Ability.h"
#include "..\Abilities\AbilityFactory.h"
class GameObject;
class AbilityNode :
	public Component
{
public:
	AbilityNode();
	~AbilityNode();
	virtual AbilityNode* clone() {
		AbilityNode* abnode = new AbilityNode();
		abnode->ab = ab;
		abnode->ability = AbilityFactory::CreateAbility(ability->ab_type);//TODO fix this to use factory
		return abnode;
	}
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	std::string ab;
	GameObject* player;
	Ability* ability;
};

