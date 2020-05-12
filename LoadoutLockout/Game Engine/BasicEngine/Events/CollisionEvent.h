#pragma once
#include "stdafx.h"
#include "Event.h"
#include "..\Components\Body.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionEvent.h
Purpose: Collision Event definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/06/2018
- End Header --------------------------------------------------------*/
class CollisionEvent : public Event
{
public:
	CollisionEvent() : Event(COLLIDE) {};
	~CollisionEvent();
	Body* b1;
	Body* b2;
	vec4 n;
	float depth;
	float x;
	float y;
};

