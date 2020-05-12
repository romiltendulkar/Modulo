#include "stdafx.h"
#include "GameObject.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Purpose: The Game Object implementation. Main methods are the setting and
getting methods for the components. Also each Game Object has an update.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/


GameObject::GameObject()
{
	mComponents = std::unordered_map<C_TYPE,Component*>();
	active = true;
	visable = true;
	unpausePriority = 0;
}

GameObject::~GameObject()
{
	for (auto component : mComponents) {
		delete component.second;
	}
}

void GameObject::Update()
{
	if (unpausePriority > 0) return;
	for (auto component : mComponents) {
		component.second->Update();
	}
}

Component * GameObject::AddComponent(Component* com)
{
	mComponents[com->GetType()] = com;
	com->mpOwner = this;
	return com;
}

Component * GameObject::GetComponent(C_TYPE _type)
{
	auto it = mComponents.find(_type);
	if (it == mComponents.end()) return nullptr;
	return mComponents[_type];
}

GameObject * GameObject::clone()
{
	GameObject* go = new GameObject();
	for (auto kv : mComponents) {
		Component* com = kv.second->clone();
		go->mComponents[kv.first] = com;
		com->SetOwner(go);
	}
	go->type = type;
	go->active = true;
	go->visable = visable;
	go->unpausePriority = 0;
	return go;
}

void GameObject::HandleEvent(Event * pevent)
{
	if (unpausePriority > 1) return;
	for (auto com : mComponents) {
		com.second->HandleEvent(pevent);
	}
	
}

