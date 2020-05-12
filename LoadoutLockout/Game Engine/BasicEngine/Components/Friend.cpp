#include "stdafx.h"
#include "Friend.h"
#include "..\Events\CollisionEvent.h"
#include "..\State\GameState.h"
#include "Player.h"
#include "..\Managers\EventManager.h"
#include "..\Events\ChangeSpriteEvent.h"
#include "Audio.h"

extern GameState* current_state;

Friend::Friend() : Component(FRIEND)
{
	anim_counter = -1;
}


Friend::~Friend()
{
}

void Friend::Update()
{
	Sprite *mpSprite = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
	if (saved)
	{
		string spritestring = "Noobsaved.png";
		if (anim_counter <= 120)
		{
			spritestring = "Noobsaved.png";
		}
		if (anim_counter <= 110)
		{
			spritestring = "Noobsaved1.png";
		}
		if (anim_counter <= 100)
		{
			spritestring = "Noobsaved2.png";
		}
		if (anim_counter <= 80)
		{
			spritestring = "Noobsaved3.png";
		}
		if (anim_counter <= 60)
		{
			spritestring = "Noobsaved4.png";
		}
		if (anim_counter <= 40)
		{
			spritestring = "Noobsaved5.png";
		}
		if (anim_counter <= 20)
		{
			spritestring = "Noobsaved6.png";
		}
		if (anim_counter == 0)
		{
			mpOwner->active = false;
		}
		anim_counter--;
		mpSprite->file = spritestring;
	}
}

void Friend::Serialize(std::string obj)
{
	json j = json::parse(obj);
	saved_file = SafeGet<std::string>(j, "saved", saved_file);
}

void Friend::HandleEvent(Event * pevent)
{
	CollisionEvent* cE;
	GameObject* other;
	Player* pP;
	Audio *pAud;
	ChangeSpriteEvent * pCE;
	switch (pevent->mType) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other = cE->b1->mpOwner;
		if (cE->b1->mpOwner == this->mpOwner) {
			other = cE->b2->mpOwner;
		}
		if (saved) break;
		pP = static_cast<Player*>(other->GetComponent(PLAYER));
		if (nullptr != pP) {
			saved = true;
			pP->saved_count += 1;
			pP->lives += 1;
			pAud = static_cast<Audio *>(mpOwner->GetComponent(AUDIO));
			if (nullptr != pAud)
				pAud->ExecuteAudio();
			anim_counter = 120;
		}
		break;
	case DESTROY:
		mpOwner->active = false;
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	default:
		break;
	}
	
}
