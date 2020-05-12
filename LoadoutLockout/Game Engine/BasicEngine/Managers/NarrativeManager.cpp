#include "stdafx.h"
#include "NarrativeManager.h"
#include "..//Components/definitions.h"
#include "..//Managers/GameObject_Manager.h"
#include "..//State/GameState.h"
#include "..//Graphics/GraphicsManager.h"


#define RESOURCE_PATH ".\\Resources\\Narrative\\"


extern GameState* current_state;
extern GraphicsManager* pGfx;

NarrativeManager::NarrativeManager(const std::string &path)
{
	std::string fullpath = RESOURCE_PATH + path;
	fopen_s(&fp, fullpath.c_str(), "r");
	char entityName[256] = { 0 };
	fscanf_s(fp, "%s", entityName, sizeof(entityName));
	std::string strEntityName = entityName;
	if ("Outside" == strEntityName)
	{
		BackgroundT = OUTSIDE;
	}
	if ("Scrapyard" == strEntityName)
	{
		BackgroundT = SCRAPYARD;
	}
	if ("Factory" == strEntityName)
	{
		BackgroundT = FACTORY;
	}
	if ("ControlRoom" == strEntityName)
	{
		BackgroundT = CONTROLROOM;
	}
	
}

NarrativeManager :: ~NarrativeManager()
{
	fclose(fp);
}

int NarrativeManager::ParseScript()
{

	char entityName[256] = { 0 };
	fscanf_s(fp, "%s", entityName, sizeof(entityName));
	std::string strEntityName = entityName;
	if ("Player" == strEntityName)
	{
		switch (BackgroundT)
		{
		case OUTSIDE:
			current_state->background = "OutsideBackgroundStoryTemplate_hero.png";
			break;
		case SCRAPYARD:
			current_state->background = "ScrapyBackgroundStoryTemplate_hero.png";
			break;
		case FACTORY:
			current_state->background = "FactoryBackgroundStoryTemplate_hero.png";
			break;
		case CONTROLROOM:
			current_state->background = "ControlRoomBackgroundStoryTemplate_hero.png";
			break;
		default:
			break;
		}
		char funcName[256] = { 0 };
		fscanf_s(fp, "%s", funcName, sizeof(funcName));
		std::string strFuncName = funcName;
		if ("Walk" == strFuncName)
		{
			float frames;
			fscanf_s(fp, "%f\n", &frames);
			cout << "Walk : " << frames << endl;
			return 0;
		}
		if ("Say" == strFuncName)
		{
			char dialogue[256] = { 0 };
			fgets(dialogue, sizeof(dialogue), fp);
			fgets(dialogue, sizeof(dialogue), fp);
			std::string strDialogue = "";// "Modulo: ";
			strDialogue += dialogue;
			cout << strDialogue << endl;
			strDialogue.erase(strDialogue.size() - 1);
			pGfx->renderString = strDialogue;
			pGfx->useText = true;
			pGfx->textx = -80;
			pGfx->texty = -30;
			return 60 * 10;
		}
	}
	else if ("Enemy" == strEntityName)
	{
		switch (BackgroundT)
		{
		case OUTSIDE:
			//current_state->background = "OutsideBackgroundStoryTemplate_hero.png";
			break;
		case SCRAPYARD:
			//current_state->background = "ScrapyBackgroundStoryTemplate_hero.png";
			break;
		case FACTORY:
			//current_state->background = "FactoryBackgroundStoryTemplate_hero.png";
			break;
		case CONTROLROOM:
			current_state->background = "ControlRoomBackgroundStoryTemplate_Boss.png";
			break;
		default:
			break;
		}
		char funcName[256] = { 0 };
		fscanf_s(fp, "%s", funcName, sizeof(funcName));
		std::string strFuncName = funcName;
		if ("Walk" == strFuncName)
		{
			float frames;
			fscanf_s(fp, "%f\n", &frames);
			cout << "Walk : " << frames << endl;
			return 0;
		}
		if ("Say" == strFuncName)
		{
			char dialogue[256] = { 0 };
			fgets(dialogue, sizeof(dialogue), fp);
			fgets(dialogue, sizeof(dialogue), fp);
			std::string strDialogue = "";// "Enemy: ";
			strDialogue += dialogue;
			cout << strDialogue << endl;
			strDialogue.erase(strDialogue.size() - 1);
			pGfx->renderString = strDialogue;
			pGfx->useText = true;
			pGfx->textx = -80;
			pGfx->texty = -30;
			return 60 * 10;
		}
	}
	else if ("Narration" == strEntityName)
	{
		switch (BackgroundT)
		{
		case OUTSIDE:
			current_state->background = "OutsideBackgroundStoryTemplate_narrator.png";
			break;
		case SCRAPYARD:
			current_state->background = "ScrapyBackgroundStoryTemplate_narrator.png";
			break;
		case FACTORY:
			current_state->background = "FactoryBackgroundStoryTemplate_narrator.png";
			break;
		case CONTROLROOM:
			current_state->background = "ControlRoomBackgroundStoryTemplate_narrator.png";
			break;
		default:
			break;
		}
		char funcName[256] = { 0 };
		fscanf_s(fp, "%s", funcName, sizeof(funcName));
		std::string strFuncName = funcName;
		if ("Say" == strFuncName)
		{
			char dialogue[256] = { 0 };
			fgets(dialogue, sizeof(dialogue), fp);
			fgets(dialogue, sizeof(dialogue), fp);
			std::string strDialogue;
			strDialogue = dialogue;
			cout << strDialogue << endl;
			strDialogue.erase(strDialogue.size() - 1);
			pGfx->renderString = strDialogue;
			pGfx->useText = true;
			pGfx->textx = -80;
			pGfx->texty = -30;
			return 60 * 10;
		}
	}
	else if ("End" == strEntityName)
	{
		return -1;
	}
}