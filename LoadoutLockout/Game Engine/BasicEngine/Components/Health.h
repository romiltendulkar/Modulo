#pragma once
#include "stdafx.h"
#include "Component.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Health.h
Purpose: Handles the health of whatever object has the the component.
When health hits 0 the object is destroyed.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class Health :
	public Component
{
public:
	int base_health;
	int curr_health;
	virtual Health* clone() {
		Health* h = new Health();
		h->base_health = base_health;
		h->curr_health = curr_health;
		return h;
	}
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	Health();
	~Health();
};

