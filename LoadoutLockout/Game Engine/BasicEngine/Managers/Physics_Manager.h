#ifndef __PHYSICS_MANAGER__
#define __PHYSICS_MANAGER__
#include "stdafx.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Physics_Manager.h
Purpose: This file has the defines the variables and methods for the
Physics Manager.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/
#include "..\GameObject.h"
class Contact;
class Body;
class Physics_Manager
{
public:
	Physics_Manager();
	~Physics_Manager();
	void Update(float dt);
private:
	void ResolveCollision(Contact* contact);
	void CorrectPositions(Contact* contact);
	float FrictionFunction(float mu1, float mu2);
	void TestPotentialMiss(Body* b1, Body* b2);
};
#endif
