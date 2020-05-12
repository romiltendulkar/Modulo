#pragma once
#include "stdafx.h"
#include "Event.h"
#include "..\GameObject.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MoveEvent.h
Purpose: Move Event definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/
class MoveEvent : public Event
{
public:
	MoveEvent(float x, float y, float a) : Event(MOVE), xPos(x),yPos(y), angle(a) {};
	~MoveEvent();
	float xPos;
	float yPos;
	float angle;
};

