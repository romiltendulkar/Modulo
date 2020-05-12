#include "stdafx.h"
#include "GameObjectFactory.h"
#include "Managers/GameObject_Manager.h"
#include "Managers/FileManager.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectFactory.cpp
Purpose: The Game Object Factory handles the creation of a given level
as well as the creation of game objects in that level. The file is in
a json format.
Platform: Windows 10, x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/11/2018
- End Header --------------------------------------------------------*/

using namespace nlohmann;
#define RESOURCE_LEVEL_PATH ".\\Resources\\Level\\"

std::map<std::string, ComponentCreator*> GameObjectFactory::creators = std::map<std::string, ComponentCreator*>();

GameObjectFactory* GameObjectFactory::factory;

GameObjectFactory::GameObjectFactory()
{
	prev_objects = std::map<std::string, GameObject*>();
}


GameObjectFactory::~GameObjectFactory(){
	prev_objects.clear();
}

void GameObjectFactory::Init()
{
	creators["sprite"] = new SpriteCreator();
	creators["transform"] = new TransformCreator();
	creators["controller"] = new ControllerCreator();
	creators["body"] = new BodyCreator();
	creators["game-state"] = new LevelStateCreator();
	creators["bullet"] = new BulletCreator();
	creators["jump-down"] = new JumpDownCreator();
	creators["fire-missle"] = new FireMissleCreator();
	creators["walk"] = new WalkCreator();
	creators["audio"] = new AudioCreator();
	creators["spikes"] = new SpikeCreator();
	creators["check"] = new CheckPointCreator();
	creators["player"] = new PlayerCreator();
	creators["ability-node"] = new AbilityNodeCreator();
	creators["cage"] = new CageCreator();
	creators["button"] = new ButtonCreator();
	creators["cursor"] = new CursorCreator();
	creators["doom"] = new DoomCreator();
	creators["enemy"] = new EnemyCreator();
	creators["text"] = new TextCreator();
	creators["friend"] = new FriendCreator();
	creators["platfrom"] = new PlatformCreator();
	creators["victory"] = new VictoryCreator();
	creators["tutorial"] = new TutorialCreator();
	factory = new GameObjectFactory();
}

void GameObjectFactory::Destory()
{
	
	for (auto kv : creators) {
		delete kv.second;
	}
	delete factory;
}

C_TYPE GameObjectFactory::String_To_C_TYPE(std::string name)
{
		if (name == "sprite") {
			return SPRITE;
		}
		else if (name == "transform") {
			return TRANSFORM;
		}
		else if (name == "controller") {
			return CONTROLLER;
		}
		else if (name == "falldown") {
			return FALLDOWN;
		}
		else if (name == "body") {
			return BODY;
		} else if (name == "ball") {
			return BALL;
		}
		else if (name == "bullet") {
			return BULLET;
		}
		else if (name == "game-state") {
			return LEVELSTATE;
		}
		else if (name == "damage") {
			return DAMAGE;
		}
		else if (name == "health") {
			return HEALTH;
		}
		else if (name == "jump-down") {
			return JUMP_DOWN;
		}
		else if (name == "fire-missle") {
			return FIRE_MISSLE;
		}
		else if (name == "walk") {
			return WALK;
		}
		else if (name == "audio") {
			return AUDIO;
		}
		else if (name == "spike") {
			return SPIKE;
		}
		else if (name == "start") {
			return STARTPOINT;
		}
		else if (name == "check") {
			return CHECKPOINT;
		}
		else if (name == "player") {
			return PLAYER;
		}
		else if (name == "button") {
			return _BUTTON;
		}
		else if (name == "cursor") {
			return _CURSOR;
		}
		else if (name == "doom") {
			return DOOM;
		}
		else if (name == "ability-node") {
			return ABILITY_NODE;
		}
		else if (name == "enemy"){
			return ENEMY;
		}
		else if (name == "cage") {
			return CAGE;
		}
		else if (name == "text") {
			return _TEXT;
		}
		else if (name == "friend") {
			return FRIEND;
		}
		else{
			return UNDEFINED;
		}
	
}

void GameObjectFactory::Reset()
{
	factory->prev_objects.clear();
}

GameObject * GameObjectFactory::LoadObject(const char * pfilename)
   {
	auto find = factory->prev_objects.find(pfilename);
	if (find != factory->prev_objects.end() ) {
		if (nullptr != factory->prev_objects[pfilename]) {
			GameObject* go = factory->prev_objects[pfilename];
			return go->clone();
		}
	}
	GameObject * pGO = new GameObject();
	std::string str = FileManager::GetJSONString(pfilename, GAME_OBJECT_FILE);
	json obj = json::parse(str);

	if (obj.empty()) {
		return nullptr;//need to report some error here
	}
	for (json::iterator it = obj.begin(); it != obj.end(); ++it) {
		if (it.key() == "type"){
			pGO->type = obj["type"];
		} else {
			Component* comp = GameObjectFactory::CreateComponent(it.key());
			pGO->AddComponent(comp);
			std::string val = it.value().dump();
			comp->Serialize(val);
		}
	}
	factory->prev_objects[pfilename] = pGO;
	return pGO;
}

Component* GameObjectFactory::CreateComponent(std::string name) {
	if (creators.find(name) == creators.end()) 
		return nullptr;
	return creators[name]->Create();
}


void GameObjectFactory::InitGameObject(GameObject * pNew)
{
	Body * pB = static_cast<Body*>(pNew->GetComponent(BODY));
	if (nullptr != pB) {
		pB->Init();
	}
	Cursor * pCu = static_cast<Cursor*>(pNew->GetComponent(_CURSOR));
	if (nullptr != pCu) {
		pCu->Init();
	}
	Enemy* pE = static_cast<Enemy*>(pNew->GetComponent(ENEMY));
	if (nullptr != pE) {
		pE->Init();
	}
	Walk* pWalk= static_cast<Walk*>(pNew->GetComponent(WALK));
	if (nullptr != pWalk) {
		pWalk->Init();
	}
	Controller* pC = static_cast<Controller*>(pNew->GetComponent(CONTROLLER));
	if (nullptr != pC) {
		pC->Init();
	}
}

