#include "stdafx.h"
#include "Audio.h"
#include "Component.h"
#include "..\State\GameState.h"
#include "..\Managers\Resource_Manager.h"
#include "..\Events\ChangeSpriteEvent.h"

extern GameState* current_state;
extern Audio_Manager* pAM;

Audio::Audio() : Component(AUDIO)
{
	file = "";
	pAud = nullptr;
}


Audio::~Audio()
{
	//delete pAud;
}

void Audio::Update()
{
}

void Audio::ExecuteAudio(std::string f)
{
	if (paused) return;
	if (f.empty()) f = file;
		//pAud = new Audio_Manager(file, false);
		//pAud = current_state->pRM->load_audio_no_loop(file);
	//pAM->SetAudioVolume(current_state->mpOwner->sfx*current_state->mpOwner->master);
	pAM->load_audio_no_loop(f);
}

void Audio::Serialize(std::string obj)
{
	json s_json;
	s_json = json::parse(obj);
	file = SafeGet<std::string>(s_json, "audio", file);
}

void Audio::HandleEvent(Event * pevent)
{
	switch (pevent->mType)
	{
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		if (paused)
			pAM->Pause();
		else
			pAM->Resume();
		break;
	default:
		break;
	}
}
