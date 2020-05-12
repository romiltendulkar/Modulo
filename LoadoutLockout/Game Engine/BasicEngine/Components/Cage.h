#pragma once
#include "Component.h"

class Cage :
	public Component
{
public:
	Cage();
	~Cage();
	A_TYPE key;
	virtual Cage* clone() {
		Cage* c = new Cage();
		c->key = key;
		c->player = nullptr;
		return c;
	};
	std::string s_key;
	void Update();
	void Init();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	GameObject* player = nullptr;
	bool collide;
};

