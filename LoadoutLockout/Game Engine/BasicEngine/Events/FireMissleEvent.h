#pragma once
#include "stdafx.h"
#include "Event.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FireMissleEvent.h
Purpose: Tirggered a missle being fired definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/11/2018
- End Header --------------------------------------------------------*/
class FireMissleEvent :public Event
{
public:
	FireMissleEvent() : Event(FIRE) {};
	~FireMissleEvent();
	float x;
	float y;
};

