#include "stdafx.h"
#include "Component.h"
#pragma once
class Doom :
	public Component
{
public:
	Doom();
	~Doom();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual Doom* clone() {
		return new Doom();
	}
};

