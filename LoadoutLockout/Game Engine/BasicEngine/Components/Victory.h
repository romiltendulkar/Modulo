#include "stdafx.h"
#pragma once
#include "Component.h"
class Victory :
	public Component
{
public:
	Victory();
	~Victory();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual Victory* clone() {
		return new Victory();
	}
	float time, current_time;
};

