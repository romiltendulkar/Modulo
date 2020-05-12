#pragma once
#include "stdafx.h"
#include "Component.h"
class Platform : public Component
{
public:
	Platform();
	~Platform();
	A_TYPE key;
	virtual Platform* clone() {
		Platform* p = new Platform();
		p->key = key;
		p->player = nullptr;
		return p;
	};
	std::string s_key;
	void Update();
	void Init();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	GameObject* player = nullptr;
	bool collide;
};

