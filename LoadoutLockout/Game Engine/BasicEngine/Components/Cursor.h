#pragma once
#include "Component.h"
class Cursor :
	public Component
{
public:
	Cursor();
	~Cursor();
	void Update();
	void Serialize(std::string obj);
	void HandleEvent(Event * pevent);
	void Init();
	bool IsClick;
	glm::vec2 prev_pos;
	virtual Cursor* clone() {
		return new Cursor();
	}
};

