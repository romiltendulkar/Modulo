#ifndef __JUMP_DOWNN__
#define __JUMP_DOWNN__
#include "stdafx.h"
#include "Component.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Jumpdown.h
Purpose: Handles the when a object should suddenly jumpdown.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/


class JumpDown : public Component
{
public:
	JumpDown();
	~JumpDown();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Update();
	Uint32 time;
	int jump_size;
	Uint32 count;
	virtual JumpDown* clone() {
		JumpDown* jd = new JumpDown();
		jd->time = time;
		jd->jump_size = jump_size;
		jd->count = count;
		return jd;
	}
	
};


#endif