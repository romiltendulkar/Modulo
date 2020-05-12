#include "stdafx.h"
#include "Particle.h"

using namespace glm;
Particle::Particle(std::string n, glm::vec2 p, glm::vec2 v, float s, float dur, bool f)
{
	pos = p;
	trans = glm::translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f));
	name = n;
	current_duration = dur;
	duration = dur;
	size = s;
	scale = glm::scale(mat4(1.0f), vec3(size, size, 1.0f));
	fade = f;
}

Particle::~Particle()
{
}

void Particle::Update(float dt)
{
	current_duration -= dt;
	if (fade)
		color = glm::vec4(current_duration / duration);
	else
		color = color;
	if (color.r < 0.f) color =glm::vec4( 0.f);
	vel.x = vel.x + 3.f* ((rand() % 10) - 5.f) / 1.f;
	pos += vel * dt;
	trans[3][0] = pos.x;
	trans[3][1] = pos.y;
}

void Particle::Reset()
{
	current_duration = duration;
}

float Particle::GetTimeLeft()
{
	return current_duration;
}

void Particle::SetFade(bool f) {
	fade = f;
}

void Particle::SetPos(glm::vec2 p)
{
	pos = p;
	trans[3][0] = p.x;
	trans[3][1] = p.y;
}

void Particle::SetVel(glm::vec2 v)
{
	vel = v;
}

float Particle::GetSize()
{
	return size;
}

void Particle::SetSize(float s)
{
	scale = glm::scale(mat4(1.0f), vec3(size, size, 1.0f));
	size = s;
}

glm::vec2 Particle::GetPos()
{
	return pos;
}

std::string Particle::GetName()
{
	return name;
}


glm::vec4 Particle::GetColor()
{
	return color;
}

glm::mat4 Particle::MakeTransform()
{
	return trans * scale;
}
