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

#include <stack>

 /**
  * @class ParticleSystem
  * @brief Manages a system of particles, including creation, updating, and destruction.
  * 
  * @details
  * The ParticleSystem class manages a collection of particles in the simulation. It provides
  * functionality for creating new particles, updating their properties, and destroying them.
  * The class also maintains a list of unique identifiers for each particle in the system.
  */
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	size_t size() const { return m_Particles.size(); }
	Particle* data() { return m_Particles.data(); }  
	const Particle* data() const { return m_Particles.data(); }

	std::vector<unsigned int> IDlistData() { return m_IDlist; }

	size_t CreateParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color);
	void DestroyParticle(unsigned int id);

	void PrintIDlist();
	unsigned int GetParticleCount() const { return m_Particles.size(); };
	
	void InitFreelist() { for (int i = m_MaxParticles - 1; i >= 0; --i) { m_Freelist.push(i); }	}

	unsigned int GetMaxNumber() const { return m_MaxParticles; }

	void MemorySize(unsigned int size) { m_MaxParticles = size; }

	Particle ReturnParticle(unsigned int id) { return m_Particles[id]; }
	unsigned int ReturnVectorSize(void) { return m_Particles.size(); }

private:
	std::vector<Particle> m_Particles;  ///< Collection of pointers to particles in the system.
	unsigned int m_ParticleCount = 0;	///< The current count of particles (initialized as 0).

	std::vector<unsigned int> m_IDlist;	///< list van alle id's 

	size_t m_MaxParticles = 100000;
	std::stack<size_t> m_Freelist;

	unsigned int m_NewestParticleID = 0;

};