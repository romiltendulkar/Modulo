#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Body.h"
#include "..\GameObject.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SideToSide.h
Purpose: handles the character moving side to side.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class Body;
class Walk : public Component
{
public:
	Walk();
	~Walk();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Init();
	Body* body;
	float speed;//negative for left postive for right
	float prev_speed;
	bool have_turn = false;
	virtual Walk* clone() {
		Walk* wlk = new Walk();
		wlk->speed = speed;
		wlk->prev_speed = prev_speed;
		return wlk;
	}
	virtual void SetOwner(GameObject* go) {
		mpOwner = go;
		body = static_cast<Body*>(mpOwner->GetComponent(BODY));
		paused = false;
	}
};

