#include "stdafx.h"
#include "GameObject_Manager.h"
#include "MemoryManager.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Graphics\Camera.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject_Manager.cpp
Purpose: This handles making sure all of the game objects are updated
and deleted correctly.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/
extern MemoryManager* pMM;
extern GraphicsManager *pGfx;

GameObject_Manager::GameObject_Manager()
{
	mObjects = std::vector<GameObject*>();
	mInactiveObjects = std::vector<GameObject*>();
}


GameObject_Manager::~GameObject_Manager()
{
	for (std::vector<GameObject*>::iterator it = mObjects.begin(); it < mObjects.end(); ++it) {
		delete *it;
		*it = nullptr;
	}
	for (std::vector<GameObject*>::iterator it = mInactiveObjects.begin(); it < mInactiveObjects.end(); ++it) {
		delete *it;
		*it = nullptr;
	}
}


void GameObject_Manager::Update()
{
	Camera* cam = pGfx->GetCamera();
	for (std::vector<GameObject*>::iterator it = mObjects.begin(); it < mObjects.end(); ) {
		if ((*it) == nullptr) break;
		(*it)->visable = cam->IsInCamera(*it);
		
		if (!(*it)->active && (*it)->unpausePriority <= 0) {
			(*it)->visable = true;
			mInactiveObjects.push_back(*it);
			it = mObjects.erase(it);
		}
		else {
			if ((*it)->type == TYPE_PLAYER) (*it)->visable = true;
			(*it)->Update();
			++it;
		}
			
	}
}


std::vector<GameObject*> GameObject_Manager::GetObjects()
{
	return mObjects;
}
std::vector<GameObject*> GameObject_Manager::GetNotvisableObjects()
{
	return mObjects;
}

void GameObject_Manager::PushGameObject(GameObject* go) {
	for (std::vector<GameObject*>::iterator it = mObjects.begin(); it < mObjects.end(); ) {

		if ((*it) == go) {
			it = mObjects.erase(it); break;
		}
		else 
			++it;
	}
	mObjects.push_back(go);
}

int GameObject_Manager::CountObjectsByType(OBJECT_TYPE type)
{
	int count = 0;
	for (auto object : mObjects) {
		if (object->type == type) {
			++count;
		}
	}
	return count;
}

GameObject * GameObject_Manager::GetObjectByType(OBJECT_TYPE type)
{
	for (auto object : mObjects) {
		if (object->type == type) {
			return object;
		}
	}
	return nullptr;
}

GameObject * GameObject_Manager::GetUsedObject(OBJECT_TYPE type)
{
	GameObject* go = nullptr;
	for (std::vector<GameObject*>::iterator it = mInactiveObjects.begin(); it < mInactiveObjects.end(); ++it) {
		if ((*it)->type == type) {
			go = *it;
			go->active = true;
			go->visable = true;
			mObjects.push_back(go);
			mInactiveObjects.erase(it);
			return go;
		}
	}
	return nullptr;
}

void GameObject_Manager::ClearActiveObjects()
{
	for (std::vector<GameObject*>::iterator it = mObjects.begin(); it < mObjects.end(); ) {
		if ((*it)->active) {
			pMM->AddObjectForDelete(*it);
			it = mObjects.erase(it);
		}
		else {
			++it;
		}

	}
}

void GameObject_Manager::Pause()
{
	for (auto go : mObjects) {
		go->unpausePriority += 1;
		go->active = !(go->unpausePriority > 0);
	}
}

void GameObject_Manager::UnPause()
{
	for (auto go : mObjects) {
		go->unpausePriority -= 1;
		if (go->unpausePriority < 0) go->unpausePriority = 0;
		go->active = (go->unpausePriority <= 0);
	}
}

int GameObject_Manager::MaxUnpausePriority()
{
	int max_unpause = -1;
	for (auto go : mObjects) {		
		if (max_unpause < go->unpausePriority) {
			max_unpause = go->unpausePriority;
		}
	}
	return max_unpause;
}


