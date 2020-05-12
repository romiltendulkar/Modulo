#include "stdafx.h"
#include "Controller.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\GameObject.h"
#include "..\Managers\Input_Manager.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Events\PauseEvent.h"
#include "..\Events\MoveEvent.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\ReleaseEvent.h"
#include "..\Events\ResetPlayerEvent.h"
#include "..\Events\AbilityEvent.h"
#include "..\Components\Audio.h"
#include "definitions.h"
#include "..\Managers\EventManager.h"
#include "..\Managers\ParticleManager.h"
#include "..\Audio\Audio_Manager.h"
#include "..\State\GameState.h"
#include "Player.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.cpp
Purpose: IMplementation of the Controller component. This for all intentive purposes
is the player.
ramifications properly.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/01/2018
- End Header --------------------------------------------------------*/

extern InputFacade * pIF;
extern FrameRate_Controller* pFRC;
extern GraphicsManager * pGfx;
extern GameObjectFactory* pGOF;
extern Audio_Manager* pAM;

extern GameState* current_state;

float MaxX = 0;
float MaxY = 0;
float MinX = 0;
float MinY = 0;

Controller::Controller() : Component(CONTROLLER)
{
	mpOwner = nullptr;
}

Controller::~Controller()
{
}

void Controller::Update() 
{//replace transform with body

	string Y_input = "NONE";
	string X_input = "NONE";
	if (nullptr != pGfx) {
		MaxY = pGfx->GetWindowHeight() / 2.f;
		MaxX = pGfx->GetWindowWidth() / 2.f;
		MinX = -MaxX;
		MinY = -MaxY;
	}
	float dt;
	if (nullptr != pFRC) {
		dt = pFRC->GetDT();
	}
	else {
		dt = 1.0f;
	}
	if (nullptr != mpOwner && nullptr != pIF) {
	Transform* pTrans = dynamic_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	float x, y, a, prevAng, width;
	if (nullptr != pTrans) {
		a = pTrans->angle;
		width = pTrans->scaleXY.x / 2.f;
		prevAng = a;
	}
	x = 0; y = 0;
	//Handle controls

#ifdef _DEBUG
		if (pIF->IsButtonTriggered(FREEZE)) {
			PauseEvent* pauseE = new PauseEvent();
			current_state->pEM->BroadcastEvent(pauseE);
		}
#endif
		Player* player = static_cast<Player*>(mpOwner->GetComponent(PLAYER));
		Sprite *pSprite = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
		if (!player->is_animated)
		{

			if (pIF->IsButtonPressed(LEFT) ||
				pIF->IsButtonTriggered(LEFT))
			{
					player->last_facing = player->is_facing;
					player->is_facing = FACELEFT;
				if (x >= MinX + width) 
				{
					if (x >= MinX + width)
					{
						X_input = "LEFT";
					}
				}

			}
			
			if (pIF->IsButtonPressed(RIGHT) ||
				pIF->IsButtonTriggered(RIGHT)) 
			{
				if (x <= MaxX - width)
				{					
						player->last_facing = player->is_facing;
						player->is_facing = FACERIGHT;
					if (x <= MaxX - width)
					{
						X_input = "RIGHT";
						
					}
				}

			}
			if (pIF->IsButtonPressed(UNLOCKED) ||
				pIF->IsButtonTriggered(UNLOCKED))
			{
				player->Unlock();
				for (int counter = 0; counter < player->Loadout.size(); counter++)
				{
					AbilityEvent* a = new AbilityEvent();
					a->a_type = player->Loadout[counter]->ab_type;
					a->make_solid = false;
					current_state->pEM->BroadcastEvent(a);
					if(pIF->IsButtonTriggered(UNLOCKED)) {
						Body* pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
						current_state->pPM->EmitParticle("Starparticle.png", vec2(pB->pos.x,pB->pos.y+30), vec2(0,5), 20, 1.f,false);
						pAM->load_audio_no_loop("move.wav");
					}
				}
			}
			if (pIF->IsButtonReleased(UNLOCKED) )
			{
				player->Lock();
				for (int counter = 0; counter < player->Loadout.size(); counter++)
				{
					AbilityEvent* a = new AbilityEvent();
					a->a_type = player->Loadout[counter]->ab_type;
					a->make_solid = true;
					current_state->pEM->BroadcastEvent(a);
				}
			}

			if (pIF->IsButtonReleased(RIGHT) ||
				pIF->IsButtonReleased(LEFT))
			{
					player->last_facing = player->is_facing;
					player->is_facing = FACESTILL;
					
			}
			if (pIF->IsButtonTriggered(JUMP_BUTTON) )
			{
				if (player->IncrementJump())
				{
					Y_input = "UP";
					player->NegateHasJumped();
					Body* pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
					pB->acc.y = 0;
					pB->vel.y = 0;
					player->current_equipped = JUMPSKIN;
					player->jump_countdown = 12;
				}
			}

			vec4 moveposition = player->MakeMove(X_input, Y_input, dt);
			if (pIF->IsButtonPressed(DOWN) ||
				pIF->IsButtonTriggered(DOWN)) {
				if(player->HasAbility(STOMP)) {
					moveposition.y -= 1.0f;
					player->Stomp();
				}
				//change object sprite to crouch sprite
			}
			else {
				player->NoStomp();
			}



			for (int counter = 0; counter < player->Loadout.size(); counter++)
			{
				player->Loadout[counter]->Update(dt);
			}
		
			if (!paused) {
				

				if (pIF->IsButtonTriggered(ABILITY_LEFT)) {
					//insert ability LEFT code
					player->ExecuteAbility(ABILITY_LEFT, _TRIGGER_);

				}
				if (pIF->IsButtonTriggered(ABILITY_RIGHT)) {
					player->ExecuteAbility(ABILITY_RIGHT,_TRIGGER_);
				}
				if (pIF->IsButtonTriggered(ABILITY_TOP)) {
					player->ExecuteAbility(ABILITY_TOP, _TRIGGER_);
				}
				//Bottom Triggerred is jump by default


				if (pIF->IsButtonPressed(ABILITY_LEFT)) {
					//insert ability LEFT code
					player->ExecuteAbility(ABILITY_LEFT, _PRESS_);

				}
				if (pIF->IsButtonPressed(ABILITY_RIGHT)) {
					player->ExecuteAbility(ABILITY_RIGHT, _PRESS_);
				}
				if (pIF->IsButtonPressed(ABILITY_TOP)) {
					player->ExecuteAbility(ABILITY_TOP, _PRESS_);
				}

				if (pIF->IsButtonPressed(ABILITY_BOTTOM)) {
					player->ExecuteAbility(ABILITY_BOTTOM, _PRESS_);
				}

				if (pIF->IsButtonReleased(ABILITY_LEFT)) {
					//insert ability LEFT code
					player->ExecuteAbility(ABILITY_LEFT,_RELEASE_);

				}
				if (pIF->IsButtonReleased(ABILITY_RIGHT)) {
					player->ExecuteAbility(ABILITY_RIGHT,_RELEASE_);
				}
				if (pIF->IsButtonReleased(ABILITY_TOP)) {
					player->ExecuteAbility(ABILITY_TOP, _RELEASE_);
				}

				if (pIF->IsButtonReleased(ABILITY_BOTTOM)) {
					player->ExecuteAbility(ABILITY_BOTTOM, _RELEASE_);
				}
				if (moveposition.x != 0 || moveposition.y != 0 || prevAng != a) {
					MoveEvent* move = new MoveEvent(moveposition.x, moveposition.y, a);
					current_state->pEM->SendEvent(move, mpOwner);
				}

			}
		}
		
	}

}

void Controller::Serialize(std::string obj){}

void Controller::Init() {
	
}

void Controller::HandleEvent(Event * pevent)
{

	switch (pevent->mType) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case COLLIDE:
		break;
	case RESET:
		break;
	} 
	
}



