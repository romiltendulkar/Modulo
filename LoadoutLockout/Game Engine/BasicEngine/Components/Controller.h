#ifndef _CONTROLLER
#define _CONTROLLER
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.h
Purpose: definition of the Controller component. This for all intentive purposes
is the player.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "Component.h"
#include "definitions.h"
#include "../Events/Event.h"
#include <nlohmann/json.hpp>

using namespace nlohmann;
class Player;

class Controller : public Component
{
	
public:
	Controller();
	~Controller();
	void Init();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual Controller* clone() {
		return new Controller();
	}
	
};
#endif
