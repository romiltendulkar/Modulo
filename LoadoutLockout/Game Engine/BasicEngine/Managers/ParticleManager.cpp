#include "stdafx.h"
#include "ParticleManager.h"
#include "..\Graphics\Particle.h"

ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{
	for (auto a : activeParticles) {
		delete a;
	}
	for (auto i : inactiveParticles) {
		delete i;
	}
}

std::vector<Particle*> ParticleManager::GetActiveParticles()
{
	return activeParticles;
}

void ParticleManager::Update(float dt)
{
	for (auto it = activeParticles.begin(); it < activeParticles.end();) {
		(*it)->Update(dt);
		if ((*it)->GetTimeLeft() <= 0) {
			inactiveParticles.push_back(*it);
			it = activeParticles.erase(it);
		}
		else {
			++it;
		}
	}
}

Particle * ParticleManager::EmitParticle(std::string name, glm::vec2 pos, glm::vec2 vel, float size, float duration, bool fade)
{
	//first search the inactive particles
	Particle* part;
	for (auto it = inactiveParticles.begin(); it != inactiveParticles.end(); ++it) {
		if ((*it)->GetName() == name) {
			(*it)->Reset();
			(*it)->SetPos(pos);
			(*it)->SetVel(vel);
			(*it)->SetSize(size);
			(*it)->SetFade(fade);
			part = *it;
			inactiveParticles.erase(it);
			activeParticles.push_back(part);
			return part;
		}
	}
	//else create a new particle
	 part = new Particle(name, pos, vel, size, duration, fade);
	activeParticles.push_back(part);
	
	return part;
}
