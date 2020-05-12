#include "stdafx.h"
#include "EventManager.h"
#include "GameObject_Manager.h"
#include "MemoryManager.h"
#include "..\State\GameState.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: EventManager.cpp
Purpose: This handles the broadcasting of events and handles the subscriptions
of the Game Objects to specific events. THe Types of Events that are broadcasted
are to a specific object, globally, timed events, and to a list of subscriptions. 
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

extern GameState * current_state;
extern MemoryManager * pMM;

EventManager::EventManager()
{
	mTimedEvents = std::list<Event*>();
}


EventManager::~EventManager()
{
	for (auto e : mTimedEvents) {
		delete e;
	}
}

int EventManager::BroadcastEvent(Event * pevent)
{ 
	int i = 0;
	for (auto go : current_state->pGOM->GetObjects()) {
		if (nullptr != go) {
			go->HandleEvent(pevent);
			++i;
		}
	}
	pMM->AddUsedEvent(pevent);
	return i;
}

void EventManager::AddTimedEvent(Event * pevent)
{
	mTimedEvents.push_back(pevent);
}

void EventManager::Update(float dt)
{
	//loop through events and decrease their timers
	std::list<Event*>::iterator it, itTmp;
	it = mTimedEvents.begin();
	while(it != mTimedEvents.end()){
		Event* pE = *it;
		pE->mTime -= dt;
		if (pE->mTime <= 0.0f) {
			BroadcastEvent(pE);
			it = mTimedEvents.erase(it);
		}
		else {
			++it;
		}
	}
}

void EventManager::Subscribe(EVENT_TYPE e_type, GameObject * go)
{
	std::unordered_map<EVENT_TYPE, std::list<GameObject*>>::iterator found = mSubs.find(e_type);
	if (found == mSubs.end()) {
		std::list<GameObject*> l;
		l.push_back(go);
		mSubs[e_type] = l;
	}
	else {
		std::list<GameObject*> l = mSubs[e_type];
		l.push_back(go);
		mSubs[e_type] = l;
	}
}

void EventManager::BroadCastToSubs(Event * pevent)
{
	auto subList = mSubs[pevent->mType];
	for (auto go : subList) {
		if(nullptr != go)
			go->HandleEvent(pevent);
	}
	pMM->AddUsedEvent(pevent);
	
}

void EventManager::SendEvent(Event * pevent, GameObject * target)
{
	if (nullptr != target || NULL != target) {
		target->HandleEvent(pevent);
	}
	pMM->AddUsedEvent(pevent);
	
}


