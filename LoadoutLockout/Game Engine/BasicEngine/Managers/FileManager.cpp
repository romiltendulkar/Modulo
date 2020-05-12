#include "stdafx.h"
#include "FileManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include "..\Graphics\renderer.h"
#include "../Audio/Audio_Engine.h"
#include "..\GameObject.h"
#include "GameObject_Manager.h"
#include "..\GameObjectFactory.h"
#include "..\BasicEngine\State\GameState.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Graphics\Camera.h"

#define RESOURCE_LEVEL_PATH ".\\Resources\\Level\\"
#define RESOURCE_GO_PATH ".\\Resources\\GameObjects\\"
#define RESOURCE_SOUND_PATH ".\\Resources\\Audio\\"
#define RESOURCE_TEXTURE_PATH ".\\Resources\\Textures\\"
#define CONFIG_PATH ".\\Config\\"
#define SAVE_PATH ".\\Save\\"
#define MENU_PATH ".\\Resources\\Menu\\"

using namespace nlohmann;
extern GraphicsManager* pGfx;

std::string FileManager::GetJSONString(std::string name, RESOURCE resource)
{
	std::string full_path = "";
	switch (resource) {
	case LEVEL_FILE:
		full_path = RESOURCE_LEVEL_PATH + name;
		break;
	case GAME_OBJECT_FILE:
		full_path = RESOURCE_GO_PATH + name;
		break;
	case CONFIG_FILE:
		full_path = CONFIG_PATH + name;
		break;
	case MENU_FILE:
		full_path = MENU_PATH + name;
		break;
	default:
		break;
	}
	std::ifstream ifs(full_path, std::ifstream::in);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	return str;
}

void FileManager::GetAndLoadTexture(unsigned char** buffer,std::string item, 
	int* width, int* height, int* bpp,unsigned int* renderer_id)
{
	std::string path = RESOURCE_TEXTURE_PATH + item;
	stbi_set_flip_vertically_on_load(true);
	*buffer = stbi_load(path.c_str(), width, height, bpp, 4);

	GLCall(glGenTextures(1, renderer_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, *renderer_id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (*buffer)
		stbi_image_free(*buffer);
}

Audio_Engine* FileManager::GetAndLoadAudio(std::string path)
{
	std::string fpath = RESOURCE_SOUND_PATH + path;
	Audio_Engine *pAud = new Audio_Engine();
	pAud->Init();
	pAud->Loadsound(fpath, true, true, true);
	//pAud->PlaySound(fpath, Vector3{ (0),(0),(0) }, 0.4f);
	return pAud;
}

Audio_Engine* FileManager::GetAndLoadAudioNoLoop(std::string path)
{
	std::string fpath = RESOURCE_SOUND_PATH + path;
	Audio_Engine *pAud = new Audio_Engine();
	pAud->Init();
	pAud->Loadsound(fpath, true, false, true);
	
	return pAud;
}

GameObject_Manager* FileManager::LoadLevel(std::string level_name, 
	float *level_width, float *level_height, std::string *background_name,
	std::string *music, std::string *cam_type, std::string *next_level, std::string *narrative)
{
	std::string level_path = RESOURCE_LEVEL_PATH + level_name;
	std::ifstream level(level_path);

	std::vector<std::vector<std::string> > level_list;

	std::string line = "";
	std::vector<std::string> items;
	// Iterate through each line and split the content using delimeter
	while (getline(level, line))
	{
		boost::algorithm::split(items, line, boost::is_any_of(","));
		level_list.push_back(items);
	}
	// Close the File
	level.close();
	
	//iterate over level_list
	//first line is always the .conf file used
	json level_conf;
	int l_height = level_list.size(); int height = l_height - 1;
	int width = -1;
	float grid_size,half_draw_x, half_draw_y,gw,gh;
	json bindings;
	std::string key;
	std::string file_name;
	json overrides;
	GameObject_Manager* pGOM =  new GameObject_Manager();
	for (int i = 0; i < l_height; ++i) {
		if (i == 0) {
			//json conf parsing
			*next_level = level_list[0][1];
			if (2 < level_list[0].size())
				*narrative = level_list[0][2];
			else
				*narrative = "";
			level_conf = json::parse(FileManager::GetJSONString(level_list[0][0], CONFIG_FILE));
			//config processing
			*background_name = level_conf["background"];
			*music = level_conf["music"];
			*cam_type = level_conf["cam-type"];
			bool match = level_conf["match-window"];
			grid_size = level_conf["grid_size"];
			gw = gh = grid_size;
			if (match) {
				*level_width = (float)pGfx->GetWindowWidth();
				*level_height = (float)pGfx->GetWindowHeight();
			}
			auto found = level_conf.find("bindings");//if no bindings the rest of the file should be blank
			if (level_conf.end() != found) {
				bindings = level_conf["bindings"];
			}
			if (height > 0) {
				width = level_list[1].size();
				if (match) {
					gw = *level_width / width;
					gh = *level_height / height;
				}  
			}
			*level_width = gw * width;
			*level_height = gh * height;
			
			half_draw_x = *level_width / 2.f;
			half_draw_y = *level_height / 2.f;
			
		}
		else {
			for (int j = 0; j < width; ++j) {
				key = level_list[i][j];
				if (key == "" || key == " ") continue;
				auto found = bindings.find(key);
				if (bindings.end() != found) {
					json json_obj = bindings[key];
					file_name = json::parse(json_obj["file"].dump());
					GameObject* go = GameObjectFactory::LoadObject(file_name.c_str());
					
					if (nullptr != go) {
						float current_x = ((gw * j) + (gw / 2.f)) - half_draw_x;
						float current_y = half_draw_y - ((gh * (i-1) ) + (gh / 2.f));
						Transform* pT = static_cast<Transform*>(go->GetComponent(TRANSFORM));
						if (nullptr == pT) continue;
						pT->SetX(current_x);
						pT->SetY(current_y);
						pT->scaleXY.x = gw;//make more dynamic
						pT->scaleXY.y = gh;
						found = json_obj.find("overrides");
						if (json_obj.end() != found) {
							overrides = json_obj["overrides"];
							for (json::iterator it = overrides.begin(); it != overrides.end(); ++it) {
								Component* comp = go->GetComponent(GameObjectFactory::String_To_C_TYPE(it.key()));
								if (nullptr == comp) {
									comp = GameObjectFactory::CreateComponent(it.key());
									if (nullptr != comp) go->AddComponent(comp);
								}
								if (nullptr != comp) {
									std::string val = it.value().dump();
									comp->Serialize(val);
								}
							}
						}
						if (go->type == TYPE_PLAYER) {
							Transform* pT = static_cast<Transform*>(go->GetComponent(TRANSFORM));
							pGfx->GetCamera()->MoveCamera(pT->GetX(), pT->GetY());
						}
						pGOM->PushGameObject(go);
					}
				}
			}
			std::cout << std::endl;
		}
	}
	//init all of the game objects
	for (auto go : pGOM->GetObjects())
		GameObjectFactory::InitGameObject(go);
	
	return pGOM;
}



void FileManager::SaveLevel(GameState* state, std::string path, float level_width, float level_height)
{
	// TODO: save the level file from game editor
}

void FileManager::SaveGame(std::string game_state, std::string save_file)
{
	std::string	full_path = CONFIG_PATH + save_file;
	std::ofstream ofs(full_path);
	ofs << game_state;
	ofs.close();
}

void FileManager::SaveConfig(std::string game_conf, std::string save_file)
{
	std::string	full_path = CONFIG_PATH + save_file;
	std::ofstream ofs(full_path);
	ofs << game_conf;
	ofs.close();	
}

void FileManager::LoadPopupMenu(std::string menu_file, GameObject_Manager * pGOM, glm::vec4 pos)
{
	json menu = json::parse(FileManager::GetJSONString(menu_file, MENU_FILE));
	for (json::iterator it = menu.begin(); it != menu.end(); ++it) {
		std::string sj = menu[it.key()].dump();
		json json_obj = json::parse(sj);
		std::string file = json_obj["button"].dump();
		if(file[0] == '\"')
			file.erase(remove(file.begin(), file.end(), '\"'), file.end());
		GameObject* go = GameObjectFactory::LoadObject(file.c_str());
		if (nullptr != go) {
			pGOM->PushGameObject(go);
			auto found = json_obj.find("overrides");
			if (json_obj.end() != found) {
				json overrides = json_obj["overrides"];
				for (json::iterator it = overrides.begin(); it != overrides.end(); ++it) {
					Component* comp = go->GetComponent(GameObjectFactory::String_To_C_TYPE(it.key()));
					if (nullptr == comp) {
						comp = GameObjectFactory::CreateComponent(it.key());
						if (nullptr != comp) go->AddComponent(comp);
					}
					if (nullptr != comp) {
						std::string val = it.value().dump();
						comp->Serialize(val);
					}
				}
			}
		}
		Transform* pT = static_cast<Transform*>(go->GetComponent(TRANSFORM));
		if (nullptr != pT) {
			pT->SetX(pos.x + pT->GetX());
			pT->SetY(pos.y + pT->GetY());
		}
		GameObjectFactory::InitGameObject(go);
	}
}

FileManager::FileManager()
{
}


FileManager::~FileManager()
{
}
