#include "stdafx.h"
#include "Platform.h"
#include "Player.h"
#include "..\GameObject.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\Events\AbilityEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"

extern GameState* current_state;

Platform::Platform() : Component(PLATFORM)
{
	s_key = "";
}


Platform::~Platform()
{
}

void Platform::Update()
{
	if (nullptr == player) return;
	Body* b1 = static_cast<Body*>(mpOwner->GetComponent(BODY));
	Body* b2 = static_cast<Body*>(player->GetComponent(BODY));
	int prev_size = current_state->pCM->mContacts.size();
	current_state->pCM->CheckCollision(b1->shape, b1->pos.x, b1->pos.y, b2->shape, b2->pos.x, b2->pos.y);

	collide = current_state->pCM->mContacts.size() > prev_size;
	current_state->pCM->Clear();

	if (!collide) {
		player = nullptr;
		AbilityEvent* a = new AbilityEvent();
		a->a_type = key;
		a->make_solid = false;
		current_state->pEM->SendEvent(a, mpOwner);
	}
}

void Platform::Serialize(std::string obj)
{
	json j_ab = json::parse(obj);
	s_key = SafeGet<std::string>(j_ab, "key", s_key);
	key = AbilityFactory::GetType(s_key);
	subscribe = true;
}
void Platform::Init() {

}

void Platform::HandleEvent(Event * pevent)
{
	AbilityEvent* aE;
	CollisionEvent* cE;
	Player* pP;
	GameObject* other;
	Body* self;
	switch (pevent->mType) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1->mpOwner;
		if (other == mpOwner) {
			other = cE->b2->mpOwner;
		}
		pP = static_cast<Player*>(other->GetComponent(PLAYER));
		if (nullptr == pP) break;
		
		if (pP->HasAbility(key)) {
			player = other;
			AbilityEvent* a = new AbilityEvent();
			a->a_type = key;
			a->make_solid = true;
			current_state->pEM->SendEvent(a,mpOwner);
			//pP->RemoveAbility(key);
			//add particle for unlocking
		}
		break;
	case ABILITY:
		aE = static_cast<AbilityEvent*>(pevent);
		if (aE->a_type == key) {
			self = static_cast<Body*>(mpOwner->GetComponent(BODY));
			if (nullptr != self)
				self->ridged = aE->make_solid;
		}
		break;
	case DESTROY:
		mpOwner->active = false;
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}
