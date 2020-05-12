#include "stdafx.h"
#include"Resource_Manager.h"

Resource_Manager::Resource_Manager()
{
	mTexture = std::unordered_map<std::string, Texture*>();
	mAudio = std::unordered_map<std::string, Audio_Manager*>();
}
Resource_Manager::~Resource_Manager()
{	
	
	for (auto tex : mTexture) {
		delete tex.second;
	}
	/*for (auto au : mAudio) {
		delete au.second;
	}*/
	mAudio.clear();
}

Texture * Resource_Manager::LoadTexture(const std::string& filePath)
{
	
	Texture *pTextures = mTexture[filePath];

	if (pTextures)
		return pTextures;

	pTextures = new Texture(filePath);

	if (pTextures)
		mTexture[filePath] = pTextures;

	return pTextures;

	
}

Audio_Manager* Resource_Manager::load_audio(const std::string & filePath)
{
	Audio_Manager *pAudio = mAudio[filePath];
//
	if (pAudio)
		return pAudio;
	pAudio = new Audio_Manager(filePath);
	if (pAudio != nullptr)
	{
		mAudio[filePath] = pAudio;
	}
	return pAudio;
	
}

Audio_Manager* Resource_Manager::load_audio_no_loop(const std::string & filePath)
{
	Audio_Manager *pAudio = mAudio[filePath];
	//
	if (pAudio)
		return pAudio;
	pAudio = new Audio_Manager(filePath,false);
	if (pAudio != nullptr)
	{
		mAudio[filePath] = pAudio;
	}
	return pAudio;

}



