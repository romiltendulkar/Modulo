#pragma once
#include "stdafx.h"
#include "Component.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Bullet.h
Purpose: Definition of the Bullet component. 
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class Bullet :
	public Component
{
public:
	Bullet();
	~Bullet();
	virtual Bullet* clone() {
		Bullet* bullet = new Bullet();
		bullet->rotate = rotate;
		return bullet;
	}
	void Update();
	void HandleEvent(Event * pevent);
	virtual void Serialize(std::string obj);
	bool rotate;
};

