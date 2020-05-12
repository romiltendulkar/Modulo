#include "stdafx.h"
#include "DashAbility.h"
#include "../Components/Body.h"
#include "../Events/MoveEvent.h"
#include "../Managers/EventManager.h"
#include "../State/GameState.h"
#include "../Components/Transform.h"
#include "../Managers/FrameRate_Controller.h"
#include "..\Managers\ParticleManager.h"


extern GameState *current_state;
extern FrameRate_Controller* pFRC;

DashAbility :: DashAbility() : Ability(DASH, 2, false)
{

	sprite_file = "DashAbility.png";	 
	DashSpeed = 11.f;
	max_speed = 20.f * DashSpeed*.2;
	min_speed = -max_speed;
	increment = max_speed;
}

DashAbility :: ~DashAbility()
{

}

void DashAbility::Update(float DT)
{
	if (cooldowntimer > 0)
	{
		cooldowntimer = cooldowntimer - DT;
		shown = false;
	}
	else
	{
		cooldowntimer = 0;
		if (!shown) {
			shown = true;
			Body* pB = static_cast<Body*>(abOwner->mpOwner->GetComponent(BODY));
			Rect* rec = static_cast<Rect*>(pB->shape);
			float x = pB->pos.x;
			float y = pB->pos.y + rec->height / 1.25f;
			current_state->pPM->EmitParticle("DashAbility.png", vec2(x, y), vec2(0.f, 10.f), rec->width / 2.f, .6f);//change to 
		}
	}
}

DashAbility::DashAbility(float Speed) : Ability(DASH, 2, false)
{
	DashSpeed = Speed;
}

void DashAbility::ExecuteAbility(BUTTON AbilityDir, PRESS_TYPE type)
{
	float dt = pFRC->GetDT();
	Body* pB = static_cast<Body*>(abOwner->mpOwner->GetComponent(BODY));
	Rect* rec = static_cast<Rect*>(pB->shape);
	int w = rec->width / 5;
	int h = rec->height / 5;
	if (cooldowntimer <= 0)
	{
		float x = 0, y = 0, a = 0;
		float move_amount;
		
		move_amount = DashSpeed;
		
		if (type == _RELEASE_) {
			if (AbilityDir == ABILITY_LEFT)
			{

				x = current_speed *dt;
				for (float i = 0; i < 5; ++i) {
					float x = pB->pos.x + ((rand() % w) + rec->width / 3.f);
					float y = pB->pos.y - rec->height / 4.f + ((rand() % h) - rec->height / 10.f);
					current_state->pPM->EmitParticle("DashParticle.png", vec2(x, y), vec2(x / 3.f, 15.f), 10.f, 1.f);

				}
			}
			if (AbilityDir == ABILITY_RIGHT)
			{
				x = current_speed *dt;
				for (float i = 0; i < 5; ++i) {
					float x = pB->pos.x - ((rand() % w) - rec->width / 3.f);
					float y = pB->pos.y - rec->height / 4.f + ((rand() % h) - rec->height / 10.f);
					current_state->pPM->EmitParticle("DashParticle.png", vec2(x, y), vec2(-x / 3.f, 15.f), 10.f, 1.f);

				}
			}
			pB->forces.x += current_speed;
			current_speed = 0;
			cooldowntimer = cooldown;
		}
		else if (type == _PRESS_) {
			if (AbilityDir == ABILITY_RIGHT)
			{
				current_speed += increment*dt;
				if (current_speed >= max_speed) 
					current_speed = max_speed;
				float x = pB->pos.x - ((rand() % w) + rec->width / 3.f);
				float y = pB->pos.y - rec->height / 4.f + ((rand() % h) - h / 4.f);
				current_state->pPM->EmitParticle("DashParticle.png", vec2(x, y), vec2(x / 3.f, 15.f), 10.f, .3f);
			}
			if (AbilityDir == ABILITY_LEFT)
			{
				current_speed -= increment*dt;
				if (current_speed <= min_speed) current_speed = min_speed;
				
				float x = pB->pos.x + ((rand() % w) + rec->width / 3.f);
				float y = pB->pos.y - rec->height / 4.f + ((rand() % h) - h/4.f);
				current_state->pPM->EmitParticle("DashParticle.png", vec2(x, y), vec2(-x / 3.f, 15.f), 10.f, .3f);
			}
		}
		else {
			if (AbilityDir == ABILITY_RIGHT)
				current_speed = max_speed * .25;
			else if(AbilityDir == ABILITY_LEFT)
				current_speed = min_speed * .25;
		}
	}
}