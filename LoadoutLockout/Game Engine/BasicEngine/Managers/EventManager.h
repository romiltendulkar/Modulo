#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__
#include "stdafx.h"
#include "../Events/Event.h"
#include "../GameObject.h"


/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: EventManager.h
Purpose: The definition of the event manager.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

class EventManager
{
public:
	EventManager();
	~EventManager();
	int BroadcastEvent( Event * pevent);
	void AddTimedEvent(Event *pevent);
	void Update(float dt);
	void Subscribe(EVENT_TYPE e_type, GameObject* go);
	void BroadCastToSubs(Event *pevent);
	void SendEvent(Event* pevent, GameObject* target);
	std::list<Event*>mTimedEvents;
	std::unordered_map<EVENT_TYPE, std::list<GameObject*> > mSubs;
};

#endif
