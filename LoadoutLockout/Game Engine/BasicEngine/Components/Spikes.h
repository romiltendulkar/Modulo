#pragma once
#include "stdafx.h"
#include "Component.h"
class Spikes : public Component
{
public:
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	Spikes();
	~Spikes();
	virtual Spikes* clone() {
		return new Spikes();
	}
};

