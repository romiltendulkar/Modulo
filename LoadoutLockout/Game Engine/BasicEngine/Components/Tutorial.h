#pragma once
#include "Component.h"
class Tutorial :
	public Component
{
public:
	Tutorial();
	~Tutorial();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual Tutorial* clone() {
		Tutorial* tut = new Tutorial();
		tut->controller_image = controller_image;
		tut->key_image = key_image;
		tut->prev_controller = prev_controller;
		return tut;
	};
	bool prev_controller;
	std::string controller_image;
	std::string key_image;
};

