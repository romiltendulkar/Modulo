#ifndef _COMPONENT
#define _COMPONENT
 // !COMPONENT
#include "stdafx.h"
#include "definitions.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.h
Purpose: The base component class definition.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/


class GameObject;
class Component
{
	public:
		Component(const C_TYPE & _type) : mType(_type) {
			mpOwner = nullptr;
			paused = false;
		};
		virtual Component* clone() = 0;
		~Component() {};
		virtual void Update() {};
		virtual void Serialize(std::string obj) {};
		virtual C_TYPE GetType() { return mType; };
		virtual void HandleEvent(Event * pevent) {};
		virtual void SetOwner(GameObject* _owner) {
			mpOwner = _owner;
		}

		C_TYPE mType;
		GameObject* mpOwner;
		bool paused =false;
		bool subscribe;
};
#endif