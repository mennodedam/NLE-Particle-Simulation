#pragma once

#include <vector>
#include <iostream>

#include "vendor/glm/glm.hpp"

#include "Particle.h"
#include "Shader.h"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void CreateParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color);
	void UpdateParticles(const std::vector<Particle>& particles);
	void DestroyParticle(unsigned int id);

	unsigned int GetParticleCount() const;

private:
	std::vector<Particle*> m_Particles;
	unsigned int m_ParticleCount;

};