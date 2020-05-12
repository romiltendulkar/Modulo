#ifndef _SIDETOSIDE
#define _SIDETOSIDE
#include "stdafx.h"
#include "Component.h"
#include "Body.h"
#include "../Events/Event.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.h
Purpose: handles the up down enemy movement.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
class FallDown : public Component
{

public:
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	FallDown();
	~FallDown() { }
	void Init();
	Uint32 time;
	float speed;
	Uint32 count;
	Uint32 down_time;
	glm::vec4 land_point;
	glm::vec4 home_point;
	float trigger_distance;
	bool go_up = false;
	bool rest = false;
	Body* body;
	virtual FallDown* clone() {
		FallDown* fd = new FallDown();
		fd->count = count;
		fd->time = time;
		fd->speed = speed;
		fd->go_up = go_up;
		return fd;
	}
	virtual void SetOwner(GameObject* go) {
		mpOwner = go;
		body = static_cast<Body*>(mpOwner->GetComponent(BODY));
		paused = false;
	}
};
#endif
