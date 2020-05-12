#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Body.h"
#include "..\GameObject.h"

class Enemy :
	public Component
{
public:
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Init();
	Body* self;
	Enemy();
	~Enemy();
	virtual Enemy* clone() {
		Enemy* e = new Enemy();
		return e;
	};
	virtual void SetOwner(GameObject* go) {
		mpOwner = go;
		self = static_cast<Body*>(mpOwner->GetComponent(BODY));
		paused = false;
	}

	//sprite scripting constraints
	enemy_type SpriteType;
	player_direction SpriteDirection;
	player_direction LastSpriteDirection;

	//animation constraints
	player_animations enem_animations;
	bool E_isAnimated;
	bool E_animationFrames;
};

