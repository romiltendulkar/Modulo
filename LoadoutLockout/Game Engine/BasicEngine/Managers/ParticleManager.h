#include "stdafx.h"
#pragma once
class Particle;
class ParticleManager
{
private:
	std::vector<Particle*> activeParticles;
	std::vector<Particle*> inactiveParticles;
public:
	ParticleManager();
	~ParticleManager();
	std::vector<Particle*> GetActiveParticles();
	void Update(float dt);
	Particle* EmitParticle(std::string name, glm::vec2 pos, glm::vec2 vel, float size, float duration, bool fade =true);
};

