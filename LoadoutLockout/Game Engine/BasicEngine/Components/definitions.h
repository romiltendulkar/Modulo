#ifndef _DEFS
#define _DEFS
#include "stdafx.h"
#include "../Events/Event.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: definitions.h
Purpose: The a set of common definitions.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/
enum C_TYPE {
	SPRITE,
	TRANSFORM,
	CONTROLLER,
	FALLDOWN,
	BODY,
	BALL,
	BULLET,
	FIRE_MISSLE,
	ENEMY_FIRE,
	JUMP_DOWN,
	HEALTH, //if it has health it is an enemy
	DAMAGE,
	LEVELSTATE,//should be singleton
	WALK,
	SIDETOSIDE,
	AUDIO,
	SPIKE,
	STARTPOINT,
	CHECKPOINT,
	PLAYER,
	ABILITY_NODE,
	DOOM,
	_BUTTON,
	_CURSOR,
	_VIEW,
	COLLECTABLE,
	SWITCH,
	CAGE,
	ENEMY,
	_TEXT,
	FRIEND,
	_SLIDER,
	PLATFORM,
	_VICTORY,
	_TUTORIAL,
	UNDEFINED
};

enum A_TYPE
{
	DASH,
	DOPPLE,
	SPIKER,
	STOMP,
	NUM_A_TYPE
};

enum PRESS_TYPE {
	_TRIGGER_,
	_PRESS_,
	_RELEASE_,
	NUM_PRESS_TYPE
};

enum player_equipment
{
	NORMALSKIN,
	JUMPSKIN,
	DOPPLESKIN
};

enum player_direction
{
	FACESTILL,
	FACELEFT,
	FACERIGHT
};

enum enemy_type
{
	NORMALENEMY
};

enum player_animations
{
	NO_ANIMATION,
	PLAYER_DEATH,
	PLAYER_VICTORY
};
#endif
