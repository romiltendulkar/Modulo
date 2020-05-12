#include "stdafx.h"
#include "Enemy.h"
#include "../GameObject.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Events\MoveEvent.h"
#include "..\Managers\EventManager.h"
#include "..\State\GameState.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\Events\ResetPlayerEvent.h"
#include "Player.h"
#include "Body.h"

extern GameState* current_state;

Enemy::Enemy() : Component(ENEMY)
{
	SpriteType = NORMALENEMY;
	SpriteDirection = FACELEFT;
	LastSpriteDirection = FACERIGHT;
	enem_animations = NO_ANIMATION;
	E_isAnimated = false;
	E_animationFrames = 0;
}

void Enemy::Update()
{
	if (paused) return;
	if(!E_isAnimated)
	{
		if (self->vel.x > 0.0f)
		{
			SpriteDirection = FACERIGHT;
		}
		if (self->vel.x < 0.0f)
		{
			SpriteDirection = FACELEFT;
		}

		if (SpriteDirection != LastSpriteDirection)
		{
			//lua code to change sprite
			string spritestring;
			luaL_dofile(current_state->lState, ".\\Scripts\\EnemySpriteScript.lua");
			{
				lua_getglobal(current_state->lState, "Espritechange");

				if (lua_isfunction(current_state->lState, -1))
				{
					lua_pushnumber(current_state->lState, SpriteType);
					lua_pushnumber(current_state->lState, SpriteDirection);
					lua_pcall(current_state->lState, 2, 1, 0);
					spritestring = lua_tostring(current_state->lState, -1);
				}
			}
			Sprite *psprite = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
			psprite->file = spritestring;
			LastSpriteDirection = SpriteDirection;
		}
	}
	else
	{
		int return_frames = 0;
		string spritestring;
		luaL_dofile(current_state->lState, ".\\Scripts\\EnemyAnimationScript.lua");
		{
			lua_getglobal(current_state->lState, "EAnimScript");

			if (lua_isfunction(current_state->lState, -1))
			{
				lua_pushnumber(current_state->lState, enem_animations);
				lua_pushnumber(current_state->lState, E_animationFrames);
				lua_pcall(current_state->lState, 2, 2, 0);
				E_animationFrames = lua_tonumber(current_state->lState, -1);
				spritestring = lua_tostring(current_state->lState, -2);
			}
		}
		Sprite *psprite = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
		psprite->file = spritestring;
		if (return_frames == 0)
		{
			E_isAnimated = false;
		}
	}
}

void Enemy::Init() {
	Body *pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
	self = pB;
}

void Enemy::Serialize(std::string obj) {

}

void Enemy::HandleEvent(Event * pevent) {
	EVENT_TYPE type = pevent->mType;
	CollisionEvent* cE;
	//Transform *pT;	//unreferenced variables
	Body* other;		//unreferenced variables
	Rect* rect;
	Rect* other_rect;
	Circle* cir;
	float bottom,self_top, top;
	Player* pP;
	switch (type) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1;
		if (other == self ) {
			other = cE->b2;
		}
		pP = static_cast<Player*>(other->mpOwner->GetComponent(PLAYER));
		if (nullptr == pP) break;
		rect = static_cast<Rect*>(self->shape);
		if (rect == nullptr) {
			cir = static_cast<Circle*>(self->shape);
			if (cE->y > self->pos.y && pP->IsStomping())
				current_state->pEM->SendEvent(new DestroyEvent(), mpOwner);
			else
				if(cE->depth > 5.f)
					current_state->pEM->SendEvent(new DestroyEvent(), other->mpOwner);
			break;
		}
		other_rect = static_cast<Rect*>(other->shape);
		bottom = other->prev_pos.y - other_rect->height / 2.f;
		top = other->prev_pos.x + other_rect->width / 2.f;
		if(bottom > self->pos.y&&  pP->stomp)
			current_state->pEM->SendEvent(new DestroyEvent(), mpOwner);
		else
			current_state->pEM->SendEvent(new DestroyEvent(), other->mpOwner);
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case DESTROY:
		mpOwner->active = false;
		break;
	}
}

Enemy::~Enemy()
{
}
