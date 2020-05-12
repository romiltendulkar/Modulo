#include "stdafx.h"
#pragma once
class Particle
{
private:
	std::string name;
	glm::vec2 pos;
	glm::vec2 vel;
	glm::mat4 scale;
	glm::mat4 trans;
	float size;
	float duration;
	float current_duration;
	glm::vec4 color;
	bool  fade;
public:
	Particle(std::string n,glm::vec2 p, glm::vec2 v, float s, float dur, bool f =true);
	~Particle();
	void Update(float dt);
	void Reset();
	float GetTimeLeft();
	void SetPos(glm::vec2 p);
	void SetVel(glm::vec2 v);
	void SetFade(bool f);
	float GetSize();
	void SetSize(float s);
	glm::vec2 GetPos();
	std::string GetName();
	glm::vec4 GetColor();
	glm::mat4 MakeTransform();
};

