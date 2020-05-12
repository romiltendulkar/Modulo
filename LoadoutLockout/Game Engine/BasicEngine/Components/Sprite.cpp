#include "stdafx.h"
#include "Sprite.h"
#include "Component.h"
#include "..\State\GameState.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Events\ChangeSpriteEvent.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Holds the texture of the given object.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

extern GameState* current_state;
extern FrameRate_Controller* pFRC;
using namespace nlohmann;

Sprite::Sprite() : Component(SPRITE)
{
	file = "";
	animate = false;
	x_off = y_off =0.f;
	x_mul = y_mul = 1.f;
	speed = 1;
	cur_time = 0;
}


Sprite::~Sprite()
{
}

void Sprite::Update()
{
	if (animate && !paused) {
		if (cur_time >speed) {
			cur_time = 0.0f;
			x_off += x_mul;
			if (x_off == 1.0f) {
				x_off = 0.0f;
				y_off -= y_mul;
				if (y_off < 0) {
					y_off = 1.0f - y_mul;
				}
			}
		}
		else {
			cur_time += pFRC->GetDT();
		}
	}
}

void Sprite::Serialize(std::string obj)
{
	json s_json;
	s_json = json::parse(obj);
	file = SafeGet<std::string>(s_json, "texture", file);
	animate = SafeGet<bool>(s_json, "animated", animate);
	x_off = SafeGet<float>(s_json, "x_off", x_off);
	y_off = SafeGet<float>(s_json, "y_off", y_off);
	x_mul = SafeGet<float>(s_json, "x_mul", x_mul);
	y_mul = SafeGet<float>(s_json, "y_mul", y_mul);
	speed = SafeGet<float>(s_json, "speed", speed);
}

void Sprite::HandleEvent(Event * pevent)
{
	ChangeSpriteEvent* pCSE;
	switch (pevent->mType) {
	case CHANGE_SPRITE:
		pCSE = static_cast<ChangeSpriteEvent*>(pevent);
		this->file = pCSE->new_name;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}

