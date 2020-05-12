#pragma once
#include "stdafx.h"
class Audio_Engine;
class GameObject_Manager;
class GameState;
enum RESOURCE {
	GAME_OBJECT_FILE,
	LEVEL_FILE,
	TEXTURE_FILE,
	AUDIO_FILE,
	MENU_FILE,
	CONFIG_FILE
};
class FileManager
{
public:
	static std::string GetJSONString(std::string name, RESOURCE resource);
	static void GetAndLoadTexture(unsigned char** buffer, std::string item,
		int* width, int* height, int* bpp, unsigned int* renderer_id);
	static Audio_Engine* GetAndLoadAudio(std::string a_path);
	static Audio_Engine* GetAndLoadAudioNoLoop(std::string path);
	static GameObject_Manager* LoadLevel(std::string level_name,
		float *level_width, float *level_height, std::string *background_name,
		std::string *music, std::string *cam_type, std::string *next_level, std::string *narrative);
	static void SaveLevel(GameState* state, std::string path, float level_width, float level_height);
	static void SaveGame(std::string game_state, std::string save_file);
	static void SaveConfig(std::string game_conf, std::string save_file);
	static void LoadPopupMenu(std::string menu_file, 
		GameObject_Manager* pGOM, glm::vec4 pos = glm::vec4(0));
	FileManager();
	~FileManager();
};

