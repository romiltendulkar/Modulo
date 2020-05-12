#pragma once
#include "stdafx.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Event.h
Purpose: Base Event definition.
Platform: <Visual Studio 2017, Win32>
Project: trevor.harron, Milestone3
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/
enum EVENT_TYPE{
	COLLIDE,
	PAUSE,
	MOVE,
	FIRE,
	DESTROY,
	RESET,
	RELEASE,
	DAMAGE_EVENT,
	CHANGE_SPRITE,
	ABILITY,
	GOAL,
	CLICK,
	TEXT_EVENT,
	PARTICLE_EVENT,
	NO_DEF
};
class Event
{
public:
	Event(const EVENT_TYPE & _type) : mType(_type), mTime(0.0f) {};
	~Event();
	EVENT_TYPE mType;
	float mTime;
};

