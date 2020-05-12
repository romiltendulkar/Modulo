#include "stdafx.h"
#pragma once
#include "Component.h"
#include "../Abilities/Ability.h"
#include "../Events/MoveEvent.h"
#include "..\Components\definitions.h"


class Player :
	public Component
{
public:
	Player();
	~Player();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);

	vec4 MakeMove(std::string xInput, std::string yInput, float dt);

	bool IncrementJump();
	void ResetJump();
	void IncrementCounter(int count);
	void DecrementCounter(int count);
	void Unlock() { unlock = true; }
	void Lock() { unlock = false; }
	bool IsUnlock() { return unlock; }
	void Stomp() { stomp = true; }
	void NoStomp() { stomp = false; }
	bool IsStomping() { return stomp; }

	bool HasAbility(A_TYPE type);
	void RemoveAbility(A_TYPE type);
	void AddAbility(Ability* a);
	void ExecuteAbility(BUTTON button, PRESS_TYPE press = _TRIGGER_);

	void ResetHasJumped();
	void NegateHasJumped();


	std::vector<Ability *> Loadout;
	GameObject* last_check_point;
	int jump_limiter, jump_counter, jump_countdown;	//variables to handle jump limitations
	bool left_move, right_move;		//variables to handle sideways collisions;

	bool stomp, unlock;
	bool use_lives = true;
	bool preset_abilities= false;
	int lives = 3;
	bool taken_damage = false;
	virtual Player* clone() {
		Player* player = new Player();
		player->Loadout = Loadout;
		player->left_move = left_move;
		player->right_move = right_move;
		player->jump_counter = jump_counter;
		player->jump_limiter = jump_limiter;
		player->stomp = stomp;
		player->saved_count = saved_count;
		return player;
	}
	int saved_count;

	float player_speed;
	bool has_jumped;

	//animation lock
	bool is_animated;
	int animation_frames;
	player_animations current_animation;

	//required for the sprite state machine
	player_equipment last_equipped;
	player_equipment current_equipped;
	player_direction is_facing;
	player_direction last_facing;

};

