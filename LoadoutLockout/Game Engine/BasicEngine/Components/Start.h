#include "stdafx.h"
#pragma once
#include "Component.h"
class Start : public  Component
{
public:
	Start();
	~Start();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Init();
	virtual Start* clone() {
		return new Start();
	}
};

