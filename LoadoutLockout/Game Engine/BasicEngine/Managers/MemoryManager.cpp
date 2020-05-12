#include "stdafx.h"
#include "MemoryManager.h"
#include "..\GameObject.h"


void MemoryManager::Update()
{
	obj_to_clean.erase(obj_to_clean.begin(), obj_to_clean.end());
	for (auto it = event_to_clean.begin(); it != event_to_clean.end(); ) {
		delete *it;
		it = event_to_clean.erase(it);
	}
	
}

void MemoryManager::AddObjectForDelete(GameObject * go)
{
	obj_to_clean.insert(go);
}

void MemoryManager::AddUsedEvent(Event * pevent)
{
	
	event_to_clean.insert(pevent);
}

MemoryManager::MemoryManager()
{
	event_to_clean = std::set<Event*>();
	obj_to_clean = std::set<GameObject*>();
}


MemoryManager::~MemoryManager()
{
	obj_to_clean.erase(obj_to_clean.begin(), obj_to_clean.end());
	for (auto it = event_to_clean.begin(); it != event_to_clean.end(); ) {
		delete *it;
		it = event_to_clean.erase(it);
	}
}

