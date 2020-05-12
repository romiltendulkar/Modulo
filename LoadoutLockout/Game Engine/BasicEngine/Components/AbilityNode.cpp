#include "stdafx.h"
#include "AbilityNode.h"
#include "..\GameObject.h"
#include "Player.h"
#include "Body.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\Events\AbilityEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
#include "..\Audio\Audio_Manager.h"

using namespace nlohmann;

extern GameState* current_state;
extern Audio_Manager* pAM;

AbilityNode::AbilityNode(): Component(ABILITY_NODE)
{
}


AbilityNode::~AbilityNode()
{
	std::string ab = "";
	player = nullptr;
	ability = nullptr;
}

void AbilityNode::Update()
{

}

void AbilityNode::Serialize(std::string obj)
{
	json j_ab = json::parse(obj);
	ab = SafeGet<std::string>(j_ab, "ability", ab);
	ability = AbilityFactory::CreateAbility(ab);
}

void AbilityNode::HandleEvent(Event * pevent)
{
	CollisionEvent* cE;
	Player* pP;
	GameObject* other;
	switch (pevent->mType) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1->mpOwner;
		if (other == mpOwner) {
			other = cE->b2->mpOwner;
		}
		pP = static_cast<Player*>(other->GetComponent(PLAYER));
		if (nullptr == pP) break;
		pP->AddAbility(ability);
		pAM->load_audio_no_loop("move.wav");
		mpOwner->active = false;
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case DESTROY:
		mpOwner->active = false;
		
		break;
	}
}
