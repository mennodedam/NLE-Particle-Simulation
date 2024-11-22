/**
 * @file Particlesystem.h
 * @brief Manages particle creation, updating, and destruction in the simulation.
 *
 * @details The ParticleSystem class allows for creating, updating, and deleting particles
 * with specific properties like position, velocity, and color. It utilizes compute shaders
 * for GPU-based particle updates.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include "vendor/glm/glm.hpp"

#include "Particle.h"

 /**
  * @class ParticleSystem
  * @brief Manages a system of particles, including creation, updating, and destruction.
  *
  * Detailed description. TO DO
  */
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	size_t size() const { return m_Particles.size(); }
	Particle* data() { return m_Particles.data(); }  
	const Particle* data() const { return m_Particles.data(); }

	void CreateParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color, unsigned int id);
	void DestroyParticle(unsigned int id);
	void PrintIDlist();
	unsigned int GetParticleCount() const;

private:
	std::vector<Particle> m_Particles;  ///< Collection of pointers to particles in the system.
	unsigned int m_ParticleCount = 0;	///< The current count of particles (initialized as 0).

	std::vector<unsigned int> m_IDlist;	///< list van alle id's 
};