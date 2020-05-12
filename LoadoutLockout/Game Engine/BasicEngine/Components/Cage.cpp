#include "stdafx.h"
#include "Cage.h"
#include "Player.h"
#include "..\GameObject.h"
#include "..\Events\ChangeSpriteEvent.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\Events\AbilityEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
#include "..\Audio\Audio_Manager.h"

extern GameState* current_state;
extern Audio_Manager* pAM;

Cage::Cage() : Component(CAGE)
{
	s_key = "";
}


Cage::~Cage()
{
}

void Cage::Update()
{
	if (nullptr == player) return;
	Body* b1 = static_cast<Body*>(mpOwner->GetComponent(BODY));
	Body* b2 = static_cast<Body*>(player->GetComponent(BODY));
	Rect* r2 = static_cast<Rect*>(b2->shape);
	if (b1->shape->TestPoint(b2->pos.x, b2->pos.y) &b1->ridged) {
		Rect* r1 = static_cast<Rect*>(b1->shape);
		b2->pos = b2->pos + vec4(0, r1->height / 2 + r2->height / 2 + 5,0,0);
	}

	collide = b1->shape->TestPoint(b2->pos.x,r2->height/2 +b2->pos.y) || 
		b1->shape->TestPoint(b2->pos.x, b2->pos.y - r2->height / 2+10);
	
	if (collide) {
		AbilityEvent* a = new AbilityEvent();
		a->a_type = key;
		a->make_solid = false;
		current_state->pEM->SendEvent(a, mpOwner);
	}
	else {
		player = nullptr;
		AbilityEvent* a = new AbilityEvent();
		a->a_type = key;
		a->make_solid = true;
		current_state->pEM->SendEvent(a, mpOwner);
	}
}

void Cage::Serialize(std::string obj)
{
	json j_ab = json::parse(obj);
	s_key = SafeGet<std::string>(j_ab, "key", s_key);
	key = AbilityFactory::GetType(s_key);
	subscribe = true;
}
void Cage::Init() {

}

void Cage::HandleEvent(Event * pevent)
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
		if (other == this->mpOwner) {
			other = cE->b2->mpOwner;
		}
		if (nullptr != other->GetComponent(PLAYER))
			player = other;
		break;
	case ABILITY:
		if (paused) break;
		aE = static_cast<AbilityEvent*>(pevent);
		if (aE->a_type == key) {
			self =static_cast<Body*>(mpOwner->GetComponent(BODY));
			if (nullptr != self) {
				self->ridged = aE->make_solid;
				ChangeSpriteEvent* pCSE;
				if (!aE->make_solid) {
					pCSE = new ChangeSpriteEvent();
					pCSE->new_name = "Open.png";
					current_state->pEM->SendEvent(pCSE, mpOwner);
					break;
				}
				else {
					if (player != nullptr) {
						Body* b1 = static_cast<Body*>(mpOwner->GetComponent(BODY));
						Body* b2 = static_cast<Body*>(player->GetComponent(BODY));
						Rect* r2 = static_cast<Rect*>(b2->shape);
						if (b1->shape->TestPoint(b2->pos.x, b2->pos.y) &b1->ridged) {
							Rect* r1 = static_cast<Rect*>(b1->shape);
							b2->pos = b2->pos + vec4(0, r1->height / 2 + r2->height / 2 + 5, 0, 0);
							pAM->load_audio_no_loop("other_jump.wav");
						}
					}
				}
				switch (key) {
				case DOPPLE:
					pCSE = new ChangeSpriteEvent();
					pCSE->new_name = "JumpLock.png";
					current_state->pEM->SendEvent(pCSE, mpOwner);
					break;
				case DASH:
					pCSE = new ChangeSpriteEvent();
					pCSE->new_name = "DashLock.png";
					current_state->pEM->SendEvent(pCSE, mpOwner);
					break;
				default:
					break;
				}

			}
		}
		break;
	case DESTROY:
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}
