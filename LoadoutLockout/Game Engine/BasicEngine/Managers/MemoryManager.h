#pragma once
#include "stdafx.h"
class GameObject;
class Event;
/*
Used for garbage cleanup, other areas create Events and 
Game objects but after they are destroyed/used
they are allocated here for safe delete
*/
class MemoryManager
{
public:
	void Update();
	void AddObjectForDelete(GameObject* go);
	void AddUsedEvent(Event *pevent);
	MemoryManager();
	~MemoryManager();
	std::set<GameObject*> obj_to_clean;
	std::set<Event*> event_to_clean;
};

