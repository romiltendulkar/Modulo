#include "stdafx.h"
#include "Component.h"
#include "Spikes.h"
#include "Player.h"
#include "definitions.h"

#include "..\Events\CollisionEvent.h"
#include "..\Events\DestroyEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
#include "..\Audio\Audio_Manager.h"
extern Audio_Manager* pAM;
extern GameState* current_state;
extern EventManager* pEM;

void Spikes::Update()
{
}

void Spikes::Serialize(std::string obj)
{
}

void Spikes::HandleEvent(Event * pevent)
{
	EVENT_TYPE type = pevent->mType;
	CollisionEvent* cE = nullptr;
	GameObject* other;
	Player* pP;
	switch (type) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b2->mpOwner;
		if (other == mpOwner) {
			other = cE->b1->mpOwner;
		}
		pP = static_cast<Player*>(other->GetComponent(PLAYER));
		if (pP != nullptr ) {
			if (!pP->HasAbility(SPIKER)) {
				current_state->pEM->SendEvent(new DestroyEvent(), other);
				pAM->load_audio_no_loop("zapped.wav");
			}
		}
		break;
	}
}

Spikes::Spikes() : Component(SPIKE)
{
}


Spikes::~Spikes()
{
}
