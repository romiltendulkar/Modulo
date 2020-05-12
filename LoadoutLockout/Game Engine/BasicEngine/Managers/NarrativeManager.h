#pragma once
#include "stdafx.h"
#include "../State/GameState.h"
#include "../Managers/GameObject_Manager.h"

enum Back_Type
{
	OUTSIDE,
	SCRAPYARD,
	FACTORY,
	CONTROLROOM
};

class NarrativeManager
{
public:
	NarrativeManager(const std::string &path);
	~NarrativeManager();
	int ParseScript();		//update like funcion to parse the script step by step
	Back_Type BackgroundT;
private:
	//file pointer variable to keep track of position
	FILE *fp;
};