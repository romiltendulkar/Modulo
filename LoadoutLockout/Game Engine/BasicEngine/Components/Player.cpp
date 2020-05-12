#include "stdafx.h"
#include "Player.h"
#include "..\BasicEngine\State\GameState.h"
#include "..\BasicEngine\Managers\EventManager.h"
#include "..\BasicEngine\Events\DestroyEvent.h"
#include "../Events/CollisionEvent.h"
#include "..\Abilities\DashAbility.h"
#include "..\Abilities\DoppleJump.h"
#include "..\State\GameState.h"
#include "..\Managers\ParticleManager.h"
#include "..\Components\definitions.h"
#include "..\Components\Checkpoint.h"
#include "../Managers/EventManager.h"
#include "../Events/ResetPlayerEvent.h"
#include "../Audio/Audio_Manager.h"
#include "..\Events\AbilityEvent.h"

extern GameState* current_state;
extern Audio_Manager* pAM;


Player::Player() : Component(PLAYER)
{
	jump_limiter = 1;
	jump_counter = 0;
	player_speed = 20;
	has_jumped = false;
	left_move = false;
	right_move = false;
	last_equipped = NORMALSKIN;
	current_equipped = last_equipped;
	is_facing = FACESTILL;
	last_facing = is_facing;
	is_animated = false;
	animation_frames = -1;
	jump_countdown = -1;
	current_animation = NO_ANIMATION;
}


Player::~Player()
{
	//update
}


void Player::Update()
{
	if (paused) return;
	if (!is_animated)
	{
		if (last_equipped == JUMPSKIN)
		{
			if (jump_countdown == 0)
			{
				current_equipped = NORMALSKIN;
			}
			jump_countdown--;
		}
		taken_damage = false;
		if (last_equipped != current_equipped || last_facing != is_facing)
		{
			//sprite lua script call;
			string spritestring;
			luaL_dofile(current_state->lState, ".\\Scripts\\PlayerSpriteScript.lua");
			{
				lua_getglobal(current_state->lState, "spritechange");

				if (lua_isfunction(current_state->lState, -1))
				{
					lua_pushnumber(current_state->lState, current_equipped);
					lua_pushnumber(current_state->lState, is_facing);
					lua_pcall(current_state->lState, 2, 1, 0);
					spritestring = lua_tostring(current_state->lState, -1);
				}
			}
			Sprite *psprite = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
			psprite->file = spritestring;
			//equate the current and last states
			last_equipped = current_equipped;
			last_facing = is_facing;
		}
	}
	else
	{
		//animation update code
		int return_frames = 0;
		string spritestring;
		/*luaL_dofile(current_state->lState, ".\\Scripts\\PAnimScript.lua");
		{
			lua_getglobal(current_state->lState, "animscript");

			if (lua_isfunction(current_state->lState, -1))
			{
				lua_pushnumber(current_state->lState, current_animation);
				lua_pushnumber(current_state->lState, animation_frames);
				lua_pcall(current_state->lState, 2, 1, 0);
				spritestring = lua_tostring(current_state->lState, -1);
			}
		}*/
		if (animation_frames <= 120)
		{
			spritestring = "D_Frame1.png";
		}
		if (animation_frames <= 100)
		{
			spritestring = "D_Frame2.png";
		}
		if (animation_frames <= 80)
		{
			spritestring = "D_Frame3.png";
		}
		if (animation_frames <= 60)
		{
			spritestring = "D_Frame4.png";
		}
		if (animation_frames <= 50)
		{
			spritestring = "D_Frame5.png";
		}
		if (animation_frames <= 40)
		{
			spritestring = "D_Frame6.png";
		}
		if (animation_frames <= 20)
		{
			spritestring = "D_Frame7.png";
		}
		animation_frames--;
		Sprite *pSprite = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
		cout << spritestring << endl;
		pSprite->file = spritestring;
		pSprite->animate = false;
		pSprite->x_off = 0.0f;
		pSprite->y_off = 0.0f;
		pSprite->y_mul = 1.0f;
		pSprite->x_mul = 1.0f;
		pSprite->speed = 1.0f;
		if (animation_frames == 0)
		{
			is_animated = false;
			if (current_animation == PLAYER_DEATH)
			{
				current_state->pEM->SendEvent(new ResetPlayerEvent(), mpOwner);
				pSprite->animate = true;
				pSprite->x_off = 0.0f;
				pSprite->y_off = 0.0f;
				pSprite->y_mul = 1.0f;
				pSprite->x_mul = 0.2f;
				pSprite->speed = 0.2f;
			}
		}
	}
	
}
bool Player::HasAbility(A_TYPE type) {
	for (auto a : Loadout) {
		if (a->ab_type == type)
			return true;
	}
	return false;
}
void Player::RemoveAbility(A_TYPE type) {
	for (auto it = Loadout.begin(); it != Loadout.end(); ++it) {
		if ((*it)->ab_type == type) {
			(*it)->DestroyAbility();
			delete (*it);
			Loadout.erase(it);
			return;
		}
	}
}
void Player::AddAbility(Ability* a) {
	a->abOwner = this;
	a->ExecuteAbility(ABILITY_TOP);
	Loadout.push_back(a);
}

vec4 Player::MakeMove(std::string xInput, std::string yInput, float dt)
{
	luaL_dofile(current_state->lState, ".\\Scripts\\MovementScript.lua");
	{
		float x = 0;
		float y = 0;
		lua_getglobal(current_state->lState, "move");
		
		if (lua_isfunction(current_state->lState, -1))
		{

			lua_pushstring(current_state->lState, xInput.c_str());
			lua_pushstring(current_state->lState, yInput.c_str());
			lua_pushnumber(current_state->lState, dt);
			lua_pushnumber(current_state->lState, player_speed);
			lua_pcall(current_state->lState, 4, 2, 0);
			y += lua_tonumber(current_state->lState, -1);
			x += lua_tonumber(current_state->lState, -2);
		}
		vec4 position;
		position.x = x;
		position.y = y;
		return position;
	}
}

void Player::Serialize(std::string obj)
{
	json j_player = json::parse(obj);
	player_speed = SafeGet<float>(j_player, "Speed", player_speed);
	json p = json::parse(obj);
	use_lives = SafeGet<bool>(p, "use_lives", use_lives);
	lives = SafeGet<int>(p, "lives", lives);
	preset_abilities = SafeGet<bool>(p, "preset_abilities", preset_abilities);
	if (preset_abilities) {
		DashAbility *Dash = new DashAbility();
		AddAbility(Dash);
		DoppleJump *dop = new DoppleJump(1);
		AddAbility(dop);
	}
	else {
		for (auto a : Loadout) {
			a->DestroyAbility();
			delete a;
		}
		Loadout.clear();
	}
}

void Player::ExecuteAbility(BUTTON button, PRESS_TYPE press) {
	if (Loadout.empty()) return;

	for (auto a : Loadout) {
		a->ExecuteAbility(button, press);
	}
	
}

void Player::HandleEvent(Event * pevent)
{
	CollisionEvent* cE;
	Body* pB;
	Body* pbody;
	Body* other_B;
	MoveEvent* mE;
	GameObject* other_object;
	Rect* rec;
	
	switch (pevent->mType) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other_object = cE->b1->mpOwner;
		other_B = cE->b1;
		if (other_object == this->mpOwner) {
			other_object = cE->b2->mpOwner;
			other_B = cE->b2;
		}
		if (other_object->type == TYPE_START || other_object->type == TYPE_CHECKPOINT)
		{
			CheckPoint *ch = static_cast<CheckPoint *>(other_object->GetComponent(CHECKPOINT));
			ch->triggered = true;
			last_check_point = other_object;
		}
			
		
		pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
		if ( (cE->n.y > 0 || cE->n.y < 0 ) && other_B->pos.y <  pB->pos.y && other_B->ridged)
			ResetJump();
		break;
	case MOVE:
		if (paused) break;
		pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
		mE = static_cast<MoveEvent*>(pevent);
		rec = static_cast<Rect*>(pB->shape);
		if (pB->vel.y <= 0 && pB->vel.y >= -.025f) {
			if (mE->xPos < 0) {
				current_state->pPM->EmitParticle("Dust.png", vec2(pB->pos.x +rec->width / 3.f, pB->pos.y - rec->height / 2.f), vec2(mE->xPos, 15.f), 10.f, .2f);
			}
			if (mE->xPos > 0) {
				current_state->pPM->EmitParticle("Dust.png", vec2(pB->pos.x-rec->width/3.f, pB->pos.y - rec->height / 2.f), vec2(mE->xPos, 15.f), 10.f, .2f);
			}
		}
		if (mE->yPos > 0) {
			int w = rec->width/3;
			int h = rec->height / 5;
			for (float i = 0; i < 5; ++i) {
				float x = pB->pos.x + ((rand() % w) - rec->width / 4.f);
				float y = pB->pos.y -rec->height/2.f + ((rand() % h) - rec->height / 10.f);
				current_state->pPM->EmitParticle("Dust.png", vec2(x, y), vec2(mE->xPos, 15.f), 10.f, .2f);

			}
		}
		break;
	case DESTROY:
		if (current_animation == PLAYER_DEATH) break;
		is_animated = true;
		current_animation = PLAYER_DEATH;
		animation_frames = 120;
		pbody = static_cast<Body*>(mpOwner->GetComponent(BODY));
		pbody->movable = false;
		pbody->physics_obj = false;
		Lock();
		for (int counter = 0; counter < Loadout.size(); counter++)
		{
			AbilityEvent* a = new AbilityEvent();
			a->a_type = Loadout[counter]->ab_type;
			a->make_solid = true;
			current_state->pEM->BroadcastEvent(a);
		}
		break;
	case RESET:
		if (animation_frames == 0)
		{

		}
		is_animated = false;
		current_animation = NO_ANIMATION;
		is_facing = FACESTILL;
		last_facing = FACELEFT;
		pbody = static_cast<Body*>(mpOwner->GetComponent(BODY));
		pbody->movable = true;
		pbody->physics_obj = true;;
		if (use_lives && !taken_damage) {
			--lives;
			taken_damage = true;
			if (lives < 0) {
				mpOwner->active = false;
				break;
			}
		}
		Transform* self_trans = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		Transform* checkpoint_trans = nullptr;
		Body* ownerBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
		if(nullptr != ownerBody)
			ownerBody->vel = vec4(0.0f);
		if (nullptr != last_check_point)
			checkpoint_trans = static_cast<Transform*>(last_check_point->GetComponent(TRANSFORM));
		if (nullptr != self_trans && nullptr != checkpoint_trans) {
			CheckPoint *ch = static_cast<CheckPoint *>(last_check_point->GetComponent(CHECKPOINT));
			ch->triggered = false;
			self_trans->SetX(checkpoint_trans->GetX());
			self_trans->SetY(checkpoint_trans->GetY());
			ownerBody->pos.x = self_trans->GetX();
			ownerBody->pos.y = self_trans->GetY();
			ownerBody->vel = vec4(0.f);
			ownerBody->acc = vec4(0.0f);
		}
		
		break;
	}
}


void Player::IncrementCounter(int count)
{
	jump_limiter += count;
}

void Player::DecrementCounter(int count)
{
	if (jump_limiter - count > 0) {
		jump_limiter -= count;
	}
	else {
		jump_limiter = 1;
	}
}

bool Player::IncrementJump()
{
	if (jump_counter < jump_limiter)
	{
		Audio *pAud = static_cast<Audio *>(mpOwner->GetComponent(AUDIO));
		pAud->ExecuteAudio();
		++jump_counter;
		return true;
	}
	else
	{
		return false;
	}
}

void Player::ResetHasJumped()
{
	has_jumped = false;
	
}

void Player::NegateHasJumped()
{
	has_jumped = true;
}

void Player::ResetJump()
{
	if (jump_counter > 0)
	{
		pAM->load_audio_no_loop("hit.wav");
	}
	jump_counter = 0;
}

