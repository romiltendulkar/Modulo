#include "stdafx.h"
#ifndef __COMPONENT_CREATOR__
#define __COMPONENT_CREATOR__

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ComponentCreator.h
Purpose: This is a factory of component creation. Used in the Game
Object Factory.
Platform: <Visual Studio 2017, Win32>
Project: trevor.harron, Milestone2
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/
#include "Components/Component.h"
#include "Components/Sprite.h"
#include "Components/Controller.h"
#include "Components/Transform.h"
#include "Components/Body.h"
#include "Components/FireMissle.h"
#include "Components/LevelState.h"
#include "Components/JumpDown.h"
#include "Components/Bullet.h"
#include "Components/Walk.h"
#include "Components/Audio.h"
#include "Components/Spikes.h"
#include "Components/CheckPoint.h"
#include "Components/Player.h"
#include "Components/AbilityNode.h"
#include "Components/Button.h"
#include "Components/Doom.h"
#include "Components/Cursor.h"
#include "Components/Enemy.h"
#include "Components/Cage.h"
#include "Components/Text.h"
#include "Components/Friend.h"
#include "Components/Platform.h"
#include "Components/Victory.h"
#include "Components/Tutorial.h"
class ComponentCreator
{
public:
	~ComponentCreator() {};
	virtual Component* Create() {
		return nullptr;
	};
};


class BulletCreator : public ComponentCreator
{
	Component* Create() {
		return new Bullet();
	}
};

class SpriteCreator : public ComponentCreator 
{
	Component* Create() {
		return new Sprite();
	}
};

class JumpDownCreator : public ComponentCreator
{
	Component* Create() {
		return new JumpDown();
	}
};

class TransformCreator : public ComponentCreator
{
	Component* Create() {
		return new Transform();
	}
};

class ControllerCreator : public ComponentCreator
{
	Component* Create() {
		return new Controller();
	}
};

class BodyCreator : public ComponentCreator
{
	Component* Create() {
		return new Body();
	}
};

class LevelStateCreator : public ComponentCreator
{
	Component* Create() {
		return new LevelState();
	}
};

class FireMissleCreator : public ComponentCreator
{
	Component* Create() {
		return new FireMissle();
	}
};

class WalkCreator : public ComponentCreator
{
	Component* Create() {
		return new Walk();
	}
};

class AudioCreator :public ComponentCreator
{
	Component* Create() {
		return new Audio();
	}
};
class SpikeCreator : public ComponentCreator {
	Component* Create() {
		return new Spikes();
	}
 };

class CheckPointCreator :public ComponentCreator
{
	Component* Create() {
		return new CheckPoint();
	}
};

class PlayerCreator : public ComponentCreator {
	Component* Create() {
		return new Player();
	}
};

class AbilityNodeCreator : public ComponentCreator {
	Component* Create() {
		return new AbilityNode();
	}
};

class DoomCreator : public ComponentCreator {
	Component* Create() {
		return new Doom();
	}
};

class ButtonCreator : public ComponentCreator {
	Component* Create() {
		return new Button();
	}
};

class CursorCreator : public ComponentCreator {
	Component* Create() {
		return new Cursor();
	}
};
class EnemyCreator : public ComponentCreator {
	Component* Create() {
		return new Enemy();
	}
};

class CageCreator :public ComponentCreator {
	Component* Create() {
		return new Cage();
	}
};

class TextCreator: public ComponentCreator {
	Component* Create() {
		return new Text();
	}
};

class FriendCreator : public ComponentCreator {
	Component* Create() {
		return new Friend();
	}
};

class PlatformCreator : public ComponentCreator {
	Component* Create() {
		return new Platform();
	}
};
class VictoryCreator : public ComponentCreator {
	Component* Create() {
		return new Victory();
	}
};
class TutorialCreator : public ComponentCreator {
	Component* Create() {
		return new Tutorial();
	}
};
#endif // !__COMPONENT_CREATOR__
