#include "stdafx.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "..\Components\Body.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CollisionManager.h
Purpose: This is the implementation of the CollisonManager and some of the
Shape implementaions for constructors. The main feature of this is the 
jump table to ensure the correct shape to shape checks are done. 
to ensure that the correct checks are done.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

void CircleToCircle(Shape* pS1, float x1, float y1, Shape* pS2, float x2, float y2, std::set<Contact*> &contacts);
void CircleToRect(Shape* pS1, float x1, float y1, Shape* pS2, float x2, float y2, std::set<Contact*> &contacts);
void RectToCircle(Shape* pS1, float x1, float y1, Shape* pS2, float x2, float y2, std::set<Contact*> &contacts);
void RectToRect(Shape* pS1, float x1, float y1, Shape* pS2, float x2, float y2, std::set<Contact*> &contacts);


Shape::Shape(SHAPE_TYPE t) : type(t)
{
}

Circle::Circle() : Shape(Shape::CIRCLE)
{
}

bool Circle::TestPoint(float pX, float pY)
{
	return false;
}

Rect::Rect() : Shape(Shape::RECT)
{
}

bool Rect::TestPoint(float pX, float pY)
{
	bool right = pX < pOwnerBody->pos.x + width / 2.f;
	bool left = pX > pOwnerBody->pos.x - width / 2.f;
	bool top = pY < pOwnerBody->pos.y + height / 2.f;
	bool bottom = pY > pOwnerBody->pos.y - height / 2.f;
	return (right && left && top && bottom);
}




CollisionManager::CollisionManager()
{
	checks[Shape::SHAPE_TYPE::CIRCLE][Shape::SHAPE_TYPE::CIRCLE] = CircleToCircle;
	checks[Shape::SHAPE_TYPE::CIRCLE][Shape::SHAPE_TYPE::RECT] = CircleToRect;
	checks[Shape::SHAPE_TYPE::RECT][Shape::SHAPE_TYPE::CIRCLE] = RectToCircle;
	checks[Shape::SHAPE_TYPE::RECT][Shape::SHAPE_TYPE::RECT] = RectToRect;
}

CollisionManager::~CollisionManager()
{
	Clear();
}

void CollisionManager::Clear()
{
	for (auto it = mContacts.begin(); it != mContacts.end();) {
		delete *it;
		it = mContacts.erase(it);
	}
}
	



void CollisionManager::SendEvent(Body * a, Body * b)
{
}


void CircleToRect(Shape * pS1, float x1, float y1, Shape * pS2, float x2, float y2, std::set<Contact*>& contacts)
{
	
	Circle* c1 = static_cast<Circle*>(pS1);
	Rect* rect2 = static_cast<Rect*>(pS2);

	
	float halfW = rect2->width / 2.0f;
	float halfH = rect2->height / 2.0f;

	vec4 half_extents = vec4(halfW, halfH, 0, 0);
	vec4 delta = pS1->pOwnerBody->pos - pS2->pOwnerBody->pos;
	
	vec4 closest = glm::clamp(delta, -half_extents, half_extents);
	
	bool inside = rect2->TestPoint(c1->pOwnerBody->pos.x, c1->pOwnerBody->pos.y);
	
	vec4 normal = closest- delta;
	float d = glm::length(normal);
	
	if(c1->radius < d && !inside) return;

	if (rect2->height < 600) {
		int i = 0;
	}
	Contact* contact = new Contact();

	contact->pS1 = pS1;
	contact->pS2 = pS2;

	contact->x = closest.x+ rect2->pOwnerBody->pos.x;
	contact->y = closest.y + rect2->pOwnerBody->pos.y;

	if (delta == closest) contact->n = -(c1->pOwnerBody->pos - closest);
	else contact->n = (c1->pOwnerBody->pos - closest);

	contact->depth = fabs(c1->radius - d);

	contacts.insert(contact);

}

void RectToCircle(Shape * pS1, float x1, float y1, Shape * pS2, float x2, float y2, std::set<Contact*>& contacts)
{
	CircleToRect(pS2, x2, y2, pS1, x1, y1, contacts);
}


void RectToRect(Shape * pS1, float x1, float y1, Shape * pS2, float x2, float y2, std::set<Contact*>& contacts)
{
	
	Rect* rect1 = static_cast<Rect*>(pS1);
	Rect* rect2 = static_cast<Rect*>(pS2);
	
	float min_x1 = x1 - rect1->width/2.f;
	float max_x1 = x1 + rect1->width/2.f;
	float min_y1 = y1 - rect1->height/2.f;
	float max_y1 = y1 + rect1->height / 2.f;

	float min_x2 = x2 - rect2->width / 2.f;
	float max_x2 = x2 + rect2->width/2.f ;
	float min_y2 = y2 - rect2->height/2.f;
	float max_y2 = y2 + rect2->height / 2.f;

	if (min_x2 > max_x1 || min_x1 > max_x2
		|| min_y2 > max_y1 || min_y1 > max_y2)
	{
		return; //no collision
	}
	vec4 delta = pS1->pOwnerBody->pos - pS2->pOwnerBody->pos;

	float x_overlap = rect2->width / 2.f + rect1->width / 2.f - fabs(delta.x);
	float y_overlap = rect2->height / 2.f + rect1->height / 2.f - fabs(delta.y);


	Contact* contact = new Contact();
	contact->pS1 = pS1;
	contact->pS2 = pS2;
	contact->x = x1 + delta.x/2.f;
	contact->y = y1 + delta.y/2.f;
	if (pS1->pOwnerBody->mpOwner != nullptr &&pS2->pOwnerBody->mpOwner != nullptr) {

		if (x_overlap < y_overlap) {
			if (delta.x < 0) contact->n = vec4(-1, 0, 0, 0);
			else contact->n = vec4(1, 0, 0, 0);
			contact->depth = x_overlap;
		}
		else {
			if (delta.y < 0)
				contact->n = vec4(0, -1, 0, 0);
			else
				contact->n = vec4(0, 1, 0, 0);
			contact->depth = y_overlap;
		}
	}
	contacts.insert(contact);
}

void CircleToCircle(Shape * pS1, float x1, float y1, Shape * pS2, float x2, float y2, std::set<Contact*>& contacts)
{
	
	Circle* c1 = static_cast<Circle*>(pS1);
	Circle* c2 = static_cast<Circle*>(pS2);
	float bothRad = c1->radius + c2->radius;
	float bothSq = bothRad * bothRad;
	float squareDis = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
	if (squareDis > bothSq) {
		return;
	}
	vec4 delta = pS2->pOwnerBody->pos - pS1->pOwnerBody->pos;
	Contact* contact = new Contact();
	contact->pS1 = pS1;
	contact->pS2 = pS2;

	contact->x = x1 + delta.x / 2.f;
	contact->y = y1 + delta.y / 2.f;

	float dis = sqrtf(squareDis);
	contact->depth = bothRad - dis;
	contact->n = delta / dis;

	contacts.insert(contact);
}


void CollisionManager::CheckCollision(Shape * pS1, float x1, float y1, Shape * pS2, float x2, float y2)
{
	checks[pS1->type][pS2->type](pS1, x1, y1, pS2, x2, y2, mContacts);
}

void CollisionManager::TestPotentionalCollision(Body * b1, Body * b2)
{
}

