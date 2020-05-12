#include "stdafx.h"
#include "Victory.h"
#include "..\State\GameState.h"
#include "..\Managers\ParticleManager.h"
#include "..\Managers\FrameRate_Controller.h"
#include "..\Audio\Audio_Manager.h"

using namespace nlohmann;

extern GameState* current_state;
extern FrameRate_Controller* pFRC;
extern Audio_Manager* pAM;

Victory::Victory() : Component(_VICTORY)
{
	time = 0;
	pAM->load_audio_no_loop("victory.wav");
}


Victory::~Victory()
{
	//delete pAM;
	//pAM = nullptr;
}

void Victory::Update()
{
	if (paused) return;
	float dt = pFRC->GetDT();
	if (current_time < 0) {
		//emit particles
		//top particles
		Body* pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
		Rect* rec = static_cast<Rect*>(pB->shape);
		Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		for (int i = 15; i <= 60; i += 5) {
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() - rec->width / 2.2f, pT->GetY() + rec->height / 2.f), vec2(-i, i), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX(), pT->GetY() + rec->height / 2.2f), vec2(0, i), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() + rec->width / 2.2f, pT->GetY() + rec->height / 2.f), vec2(i, i), i, 1.25f);
			//side particles
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() - rec->width / 2.2f, pT->GetY() + rec->height / 4.f), vec2(-6*i, i/2), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() - rec->width / 2.2f, pT->GetY() - rec->height / 4.f), vec2(-6*i, -i/2), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() + rec->width / 2.2f, pT->GetY() + rec->height / 4.f), vec2(6*i, i/2), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() + rec->width / 2.2f, pT->GetY() - rec->height / 4.f), vec2(6*i, -i/2), i, 1.25f);
			//bottom particles
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() - rec->width / 2.2f, pT->GetY() - rec->height / 2.f), vec2(-i, -i), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX(), pT->GetY() - rec->height / 2.2f), vec2(i, -i), i, 1.25f);
			current_state->pPM->EmitParticle("Starparticle.png", vec2(pT->GetX() + rec->width / 2.2f, pT->GetY() - rec->height / 2.f), vec2(i, -i), i, 1.25f);
		}
		current_time = time;
	}
	else {
		current_time -= dt;
	}
}

void Victory::Serialize(std::string obj)
{
	json j_fire = json::parse(obj);
	time = SafeGet<float>(j_fire, "time", time);
	current_time = 0;
}

void Victory::HandleEvent(Event * pevent)
{
	switch (pevent->mType) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	default:
		break;
	}
}
