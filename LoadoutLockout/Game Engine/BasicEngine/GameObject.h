#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__
#include "stdafx.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Controller.h"
#include "Components/Component.h"
#include "Components/definitions.h"
#include "Events/Event.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Purpose: The Game Object definition. The Game Object has a map of components
that defines the behavior.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/

enum OBJECT_TYPE {
	TYPE_PLAYER,
	TYPE_BALL,
	TYPE_E1,
	TYPE_E2,
	TYPE_P_MISSLE,
	TYPE_E_MISSLE,
	LEVEL_STATE,
	WALL_TYPE,
	TYPE_SPIKE,
	TYPE_GOAL,
	TYPE_COLLECT,
	TYPE_START,
	TYPE_CHECKPOINT,
	TYPE_BUTTON,
	TYPE_OTHER,
	TYPE_CAGE,
	TYPE_FRIEND,
	TYPE_CURSOR,
	TYPE_VICTORY,
	NUM_OBJ_TYPES
};

class GameObject
{

public:
	GameObject();
	~GameObject();
	OBJECT_TYPE type;
	void Update();
	Component * AddComponent(Component* com);
	Component * GetComponent(C_TYPE _type);
	GameObject* clone();
	//make private and make accessors 

	std::unordered_map<C_TYPE,Component*> mComponents;//maybe unordered map?
	bool active;
	bool visable;
	int unpausePriority = 0;
	void HandleEvent(Event *pevent);
};
#endif // !1

