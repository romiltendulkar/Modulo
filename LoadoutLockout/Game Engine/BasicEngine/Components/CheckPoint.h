#pragma once
#include "stdafx.h"
#include "Component.h"
class CheckPoint : public  Component
{
public:
	bool triggered = false;
	bool shown = false;
public:
	CheckPoint();
	~CheckPoint();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual CheckPoint* clone() {
		return new CheckPoint();
	}
};

