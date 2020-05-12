#pragma once
#include "stdafx.h"
#include "Event.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:PauseEvent.h
Purpose: Pause Event definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/
class PauseEvent :
	public Event
{
public:
	PauseEvent(bool p = true ) : pause(p), Event(PAUSE) {};
	~PauseEvent();
	bool pause;
};

