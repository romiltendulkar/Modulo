#pragma once
#include "stdafx.h"
#include "Ability.h"


class DashAbility : public Ability
{
public:
	DashAbility();
	DashAbility(float Speed);
	~DashAbility();
	virtual void Update(float DT);
	void ExecuteAbility(BUTTON AbilityDir, PRESS_TYPE type = _TRIGGER_);

private: 
	bool shown = false;
	float DashSpeed;
	float max_speed, min_speed;
	float increment,current_speed;

};
