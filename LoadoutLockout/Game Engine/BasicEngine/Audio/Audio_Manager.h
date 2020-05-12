#pragma once
#include "stdafx.h"
#include "Audio_Engine.h"
#include "..//Managers/FileManager.h"

class Audio_Manager
{
public:
	Audio_Manager(const std::string& path);
	Audio_Manager(const std::string& path, bool loop);
	~Audio_Manager();
	void load_audio(const std::string& path);
	void load_audio_no_loop(const std::string& path);
	void Stop();
	void Pause();
	void Resume();
	void SetMasterVolume(float Volume);
	void SetSFXVolume(float Volume);
	void SetMusicVolume(float Volume);
	Audio_Engine* pAE =nullptr;
	bool paused =false;
	FileManager *pFM;
	std::string sound_path;
	int audio_channel;
	bool looped;
	float audio_scale = .25;

};

