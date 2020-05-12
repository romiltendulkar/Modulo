#pragma once
#include "stdafx.h"
#include "Event.h"
#include "..\GameObject.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ReleaseEvent.cpp
Purpose: Release Event definition, mainly blank.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/06/2018
- End Header --------------------------------------------------------*/
class ReleaseEvent :
	public Event
{
public:
	ReleaseEvent(GameObject* pGO);
	~ReleaseEvent();
	GameObject* pPlayerObject;
};

