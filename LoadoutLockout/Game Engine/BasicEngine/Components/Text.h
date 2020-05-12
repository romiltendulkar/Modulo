#include "stdafx.h"
#pragma once
#include "Component.h"
using namespace glm;
class Text :
	public Component
{
public:
	bool show;
	bool reveal;
	vec4 regColor;
	vec4 specialColor;
	std::string kbText;
	std::string conText;
	virtual Text* clone() {
		Text* text = new Text();
		text->kbText = kbText;
		text->conText = conText;
		text->regColor = regColor;
		text->specialColor = specialColor;
		text->show = show;
		text->reveal;
		return text;
	};
	Text();
	~Text();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Init();
};

