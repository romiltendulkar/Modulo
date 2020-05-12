#pragma once
#include "stdafx.h"
#include "Event.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ChangeSpriteEvent.h
Purpose: Change Sprite Event definition.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/13/2018
- End Header --------------------------------------------------------*/
class ChangeSpriteEvent :
	public Event
{
public:
	ChangeSpriteEvent();
	~ChangeSpriteEvent();
	std::string new_name;
};

