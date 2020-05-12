#include "stdafx.h"
#include "Transform.h"
#include "Body.h"
#include "..\GameObject.h"
#include "..\Managers\CollisionManager.h"

#include "..\Managers\FrameRate_Controller.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: handles the transform of a object so it can be rendered. This
also handles making matrices.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

using namespace nlohmann;
extern FrameRate_Controller* pFRC;

Transform::Transform() : Component(TRANSFORM)
{
	pos = vec4(vec3(0.0f), 1.0f);
	scaleXY = vec2(0.f);
	rotate = false;
}


Transform::~Transform()
{

}

void Transform::Update()
{
	if (!paused && rotate) {
		angle += 0.25f*pFRC->GetDT();
		if (angle > 180) angle -= 360;
	}
}

void Transform::Serialize(std::string obj)
{
	json j_trans = json::parse(obj);
	
	scaleXY.x = SafeGet<float>(j_trans, "scaleX", scaleXY.x);
	scaleXY.y = SafeGet<float>(j_trans, "scaleY", scaleXY.y);
	pos.x = SafeGet<float>(j_trans, "x", pos.x);
	pos.y = SafeGet<float>(j_trans, "y", pos.y);
	rotate = SafeGet<float>(j_trans, "rotate", rotate);
}

void Transform::HandleEvent(Event * pevent)
{
	switch (pevent->mType) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break; 
	
	}
}

void Transform::SetX(float x)
{
	pos.x = x;
}

void Transform::SetY(float y)
{
	pos.y = y;
}

float Transform::GetX()
{
	return pos.x;
}

float Transform::GetY()
{
	return pos.y;
}

void Transform::SetAngle(float a)
{
	angle = a;
}

mat4 Transform::MakeTransformMatrix()
{
	prevAngle = angle;
	prevPos = pos;
	mat4 trans = glm::translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f));
	mat4 rot = glm::rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
	mat4 scale = glm::scale(mat4(1.0f), vec3(scaleXY.x, scaleXY.y, 1.0f));
	transform = trans * rot * scale;
	return transform;
}

mat4 Transform::MakeTransformFromBody(Body* pB) {
	mat4 t;
	mat4 trans = glm::translate(mat4(1.0f), vec3(pB->pos.x, pB->pos.y, 0.0f));
	mat4 rot = glm::rotate(mat4(1.0f), 0.0f, vec3(0.0f, 0.0f, 1.0f));
	mat4 scale;
	if(pB->shape_name == "circle"){
		Circle* cir = static_cast<Circle*>(pB->shape);
		scale = glm::scale(mat4(1.0f), vec3(2 * cir->radius, 2*cir->radius, 1.0f));
	}
	else {
		Rect* rec = static_cast<Rect*>(pB->shape);
		scale = glm::scale(mat4(1.0f), vec3(rec->width , rec->height, 1.0f));
	}
	
	t = trans * rot * scale;
	return t;
}

mat4 Transform::MakeScaledTransformMatrix(float scale_value)
{
	prevAngle = angle;
	prevPos = pos;
	mat4 trans = glm::translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f));
	mat4 rot = glm::rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));
	mat4 scale = glm::scale(mat4(1.0f), vec3(scaleXY.x *scale_value, scaleXY.y *scale_value, 1.0f));
	transform = trans * rot * scale;
	return transform;
}
