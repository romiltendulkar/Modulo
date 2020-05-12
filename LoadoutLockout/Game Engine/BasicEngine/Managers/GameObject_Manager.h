#ifndef __GAME_OBJECT_MANAGER__
#define __GAME_OBJECT_MANAGER__
#include "stdafx.h"
#include "../GameObject.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject_Manager.h
Purpose: This file has the defines the variables and methods for the Game
Object Manager.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/
class GameObject_Manager
{
public:
	GameObject_Manager();
	~GameObject_Manager();
	void Update();
	std::vector<GameObject*> GetObjects();
	std::vector<GameObject*> GetNotvisableObjects();
	void PushGameObject(GameObject* go);
	int CountObjectsByType(OBJECT_TYPE type);
	GameObject* GetObjectByType(OBJECT_TYPE type);
	GameObject* GetUsedObject(OBJECT_TYPE type);
	void ClearActiveObjects();
	void Pause();
	void UnPause();
	int MaxUnpausePriority();
private:
	std::vector<GameObject*> mObjects;
	std::vector<GameObject*> mInactiveObjects;
	bool isPaused;
};
#endif
