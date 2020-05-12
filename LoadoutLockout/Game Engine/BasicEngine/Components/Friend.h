#include "stdafx.h"
#pragma once
#include "Component.h"
class Friend :
	public Component
{
public:
	Friend();
	~Friend();
	virtual Friend* clone() {
		Friend* f = new Friend();
		f->saved = false;
		f->saved_file = saved_file;
		return f;
	};
	std::string saved_file;
	bool saved = false;
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);

private:
	int anim_counter;
};

