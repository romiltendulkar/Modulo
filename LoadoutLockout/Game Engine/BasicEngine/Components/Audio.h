#pragma once
#include "stdafx.h"
#include "Component.h"
#include "../Events/Event.h"
#include "..\Graphics\Texture.h"
#include "..\Audio\Audio_Manager.h"

class Audio : public Component
{
public:
	Audio();
	~Audio();

	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	virtual Audio* clone() {
		Audio* au = new Audio();
		au->file = file;
		return au;
	};
	void ExecuteAudio(std::string f ="");
	Audio_Manager *pAud;
	std::string file;
};

