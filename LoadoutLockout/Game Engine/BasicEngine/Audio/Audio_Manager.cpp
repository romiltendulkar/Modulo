#include "stdafx.h"
#include "Audio_Manager.h"
#include "..//Managers/Resource_Manager.h"
#include "..//State/GameState.h"

#define AUDIO_SCALE .3

#define RESOURCE_SOUND_PATH ".\\Resources\\Audio\\"

extern GameState *current_state;

Audio_Manager::Audio_Manager(const std::string & path)
{
	GameEngine *gE = current_state->mpOwner;
	pAE = FileManager::GetAndLoadAudio(path);
	sound_path = RESOURCE_SOUND_PATH + path;
	audio_channel = pAE->PlaySound(sound_path, Vector3{ (0),(0),(0) }, audio_scale*gE->master * gE->music);
	looped = true;
}

Audio_Manager::Audio_Manager(const std::string & path, bool loop)
{
	GameEngine *gE = current_state->mpOwner;
	if (loop)
	{
		pAE = FileManager::GetAndLoadAudio(path);
		sound_path = RESOURCE_SOUND_PATH + path;
		audio_channel = pAE->PlaySound(sound_path, Vector3{ (0),(0),(0) }, audio_scale* gE->master * gE->music);
		looped = true;
	}
	else
	{
		pAE = FileManager::GetAndLoadAudioNoLoop(path);
		
		sound_path = RESOURCE_SOUND_PATH + path;
		pAE->PlaySound(sound_path, Vector3{ (0),(0),(0) }, audio_scale* gE->master * gE->sfx);
		looped = false;
	}
	
}

Audio_Manager::~Audio_Manager()
{
	Pause();
	delete pAE;
}

void Audio_Manager::load_audio(const std::string& path)
{
	GameEngine *gE = current_state->mpOwner;
	sound_path = RESOURCE_SOUND_PATH + path;
	pAE->Loadsound(sound_path, true, true, true);
	audio_channel = pAE->PlaySound(sound_path, Vector3{ (0),(0),(0) }, audio_scale*gE->master * gE->music);
}

void Audio_Manager::load_audio_no_loop(const std::string& path)
{
	GameEngine *gE = current_state->mpOwner;
	sound_path = RESOURCE_SOUND_PATH + path;
	pAE->Loadsound(sound_path, true, false, true);
	pAE->PlaySound(sound_path, Vector3{ (0),(0),(0) }, audio_scale*gE->master * gE->sfx);
}
void Audio_Manager::Stop()
{
	pAE->StopChannel(audio_channel);
}

void Audio_Manager::Pause()
{
	pAE->PauseChannel(audio_channel);
}

void Audio_Manager::Resume()
{
	pAE->ResumeChannel(audio_channel);
}

void Audio_Manager::SetMasterVolume(float Volume)
{
	GameEngine *gE = current_state->mpOwner;
	pAE->SetMusicvolume(audio_channel, audio_scale*gE->music * Volume);
	pAE->SetSFXvolume(audio_channel, audio_scale*gE->sfx * Volume);
}

void Audio_Manager::SetMusicVolume(float Volume)
{
	GameEngine *gE = current_state->mpOwner;
	pAE->SetMusicvolume(audio_channel, audio_scale* Volume * gE->master);
}

void Audio_Manager::SetSFXVolume(float Volume)
{
	GameEngine *gE = current_state->mpOwner;
	pAE->SetSFXvolume(audio_channel, audio_scale*Volume * gE->master);
}