#include "stdafx.h"
#include "CheckPoint.h"
#include "../Components/Body.h"
#include "../State/GameState.h"
#include "../Managers/EventManager.h"
#include "../State/GameState.h"
#include "../Components/Transform.h"
#include "../Managers/FrameRate_Controller.h"
#include "..\Managers\ParticleManager.h"
#include "..\Audio\Audio_Manager.h"

extern GameState *current_state;
extern Audio_Manager* pAM;
CheckPoint::CheckPoint() : Component(CHECKPOINT)
{
	subscribe = false;
}


CheckPoint::~CheckPoint()
{
}

void CheckPoint::Update()
{
	if (paused) return;
	if(triggered == false)
	{
		shown = false;
	}

	else if(triggered == true)
	{
		if (shown == false)
		{
			shown = true;
			Body* pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
			Rect* rec = static_cast<Rect*>(pB->shape);
			float x = pB->pos.x;
			float y = pB->pos.y + rec->height / 1.5f;
			current_state->pPM->EmitParticle("SpiralParticle.png", vec2(x, y), vec2(0.f, 10.f), rec->width / 2.f, .6f);
			pAM->load_audio_no_loop("pickup.wav");
		}
	}
}

void CheckPoint::Serialize(std::string obj)
{
	json j = json::parse(obj);
	subscribe = SafeGet<bool>(j, "subscribe", subscribe);
}

void CheckPoint::HandleEvent(Event * pevent)
{
	switch (pevent->mType) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	case COLLIDE:
		break;
	case RESET:
		break;
	}
}
