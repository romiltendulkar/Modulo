#ifndef _TRANSFORM
#define _TRANSFORM
#include "stdafx.h"
#include "Component.h"
#include "../Events/Event.h"
#include <glm/glm.hpp>


/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.h
Purpose: handles the transform of a object so it can be rendered. This
also handles making matrices.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/
using namespace glm;
class Body;
class Transform : public Component
{
public:
	Transform();
	~Transform();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void SetX(float x);
	void SetY(float y);
	float GetX();
	float GetY();
	void SetAngle(float a);
	mat4 MakeTransformMatrix();
	mat4 MakeScaledTransformMatrix(float scale_value);
	vec4 pos;
	mat4 transform;
	float angle;
	vec2 scaleXY;
	vec4 prevPos;
	float prevAngle;
	bool rotate;
	virtual Transform* clone() {
		Transform* pT = new Transform();
		pT->angle = angle;
		pT->pos = pos;
		pT->rotate = rotate;
		pT->transform = transform;
		pT->prevPos = prevPos;
		pT->prevAngle = prevAngle;
		pT->scaleXY = scaleXY;
		return pT;
	}

	mat4 static MakeTransformFromBody(Body* pB);
};
#endif // !_TRANSFORM

