#ifndef __GAME_OBJECT_FACTORY__
#define __GAME_OBJECT_FACTORY__
#include "stdafx.h"
#include "GameObject.h"
#include "Managers/GameObject_Manager.h"
#include "GameObject.h"
#include "ComponentCreator.h"


/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectFactory.h
Purpose: The Game Object Factory definition.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

class GameObjectFactory
{
private:
	static std::map<std::string,ComponentCreator*> creators;
	GameObjectFactory();
	~GameObjectFactory();
	static GameObjectFactory* factory;
	std::map<std::string, GameObject*> prev_objects;
public:
	static void Init();
	static void Destory();
	static C_TYPE String_To_C_TYPE(std::string);
	static void Reset();
	static GameObject * LoadObject(const char * pfilename);
	static void InitGameObject(GameObject * pNew);
	static Component* CreateComponent(std::string name);
	
};
#endif // !GAMEOBJECTFACTORY
