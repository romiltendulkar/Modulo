#ifndef _SPRITE
#define _SPRITE
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.h
Purpose: Holds the texture of the given object.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "Component.h"
#include "../Events/Event.h"
#include "..\Graphics\Texture.h"

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();
	Texture *Texture;
	int i_width;
	int i_height;
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);

	std::string file;
	bool animate;
	float x_off, y_off;
	float x_mul, y_mul;
	float speed;
	float cur_time;
	virtual Sprite* clone() {
		Sprite* s = new Sprite();
		s->animate = animate;
		s->file = file;
		s->cur_time = cur_time;
		s->speed = speed;
		s->x_mul = x_mul;
		s->x_off = x_off;
		s->y_mul = y_mul;
		s->y_off = y_off;
		return s;
	}
};
#endif // !_SPRITE

