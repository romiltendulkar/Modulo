#include "Component.h"
#include "stdafx.h"
#pragma once
class GameObject;
class Button : public Component
{
public:
	Button();
	~Button();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	bool is_selected;
	bool prev_selected;
	std::string level;
	std::string selected;
	std::string n_selected;
	unsigned int id;
	GameObject* cursor;
	virtual Button* clone() {
		Button* button = new Button();
		button->level = level;
		button->id = id;
		button->is_selected = is_selected;
		button->cursor = cursor;
		return button;
	}
};

