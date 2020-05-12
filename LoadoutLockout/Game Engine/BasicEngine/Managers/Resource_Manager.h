#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "stdafx.h"
#include"../BasicEngine/Graphics/Texture.h"
#include"../BasicEngine/Graphics/Shader.h"
#include"../Audio/Audio_Manager.h"

struct SDL_Surface;
struct SDL_Texture;

class Resource_Manager
{
public:
	Resource_Manager();
	~Resource_Manager();
	Texture* LoadTexture(const std::string& filePath);
	Audio_Manager*  load_audio(const std::string& filePath);
	Audio_Manager*  load_audio_no_loop(const std::string& filePath);

private:
	std::unordered_map<std::string, Texture *> mTexture;
	std::unordered_map<std::string, Audio_Manager *> mAudio;

};

#endif // !RESOURCE_MANAGER_H
