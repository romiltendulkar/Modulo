#ifndef __COLLISION_MANAGER__
#define __COLLISION_MANAGER__
#include "stdafx.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionManager.h
Purpose: This defines the CollisonManager and the Shape class 
and its sub classes. The collision manager will utilize the shapes 
to ensure that the correct checks are done.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

class Body;
class Shape
{

public:
	enum SHAPE_TYPE {
		CIRCLE,
		RECT,
		NUM
	};
	Shape(SHAPE_TYPE type);
	virtual ~Shape() {};
	virtual Shape* clone() = 0;
	virtual bool TestPoint(float pX, float pY) {
		return false;
	}
	virtual void SetBody(Body* body) {
		pOwnerBody = body;
	}
	Body* pOwnerBody;
	SHAPE_TYPE type;
};

class Circle : public Shape
{
public:
	Circle();
	~Circle() {};
	bool TestPoint(float pX, float pY);
	virtual Circle* clone() {
		Circle* cir = new Circle();
		cir->radius = radius;
		return cir;
	}
	virtual void SetBody(Body* body) {
		pOwnerBody = body;
	}
	float radius;
};

class Rect : public Shape
{
public:
	Rect();
	~Rect() {};
	bool TestPoint(float pX, float pY);
	virtual Rect* clone() {
		Rect* rect = new Rect();
		rect->height = height;
		rect->width = width;
		return rect;
	}
	virtual void SetBody(Body* body) {
		pOwnerBody = body;
	}
	float height,width;
};

class Contact {
public:
	Shape* pS1;
	Shape* pS2;
	glm::vec4 n;
	float depth;
	float x;
	float y;
};


class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();
	std::set<Contact*> mContacts;
	void Clear();
	void CheckCollision(Shape* pS1, float x1, float y1, Shape* pS2, float x2, float y2);
	void TestPotentionalCollision(Body* b1, Body* b2);
private:
	//2darray of shape collision test functions
	void (*checks[Shape::NUM][Shape::NUM])(Shape* pS1, float x1, float y1, 
				Shape* pS2, float x2, float y2,
				std::set<Contact*>& contacts);
	void SendEvent(Body* a, Body* b);
	


};
#endif
