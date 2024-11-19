#include "Particlesystem.h"

ParticleSystem::ParticleSystem()
	: m_ParticleCount(0),
	m_Particles(NULL)
{

}

ParticleSystem::~ParticleSystem()
{
	for (Particle* particle : m_Particles)
	{
		delete particle;
	}
}

void ParticleSystem::CreateParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color)
{
	Particle* particle = new Particle(pos, vel, acc, m, r, color);
	m_Particles.push_back(particle);
	m_ParticleCount++;
}

void ParticleSystem::UpdateParticles(const std::vector<Particle>& particles)
{

}

void ParticleSystem::DestroyParticle(unsigned int id)
{
	if (id < m_Particles.size()) {
		delete m_Particles[id];							// Free the memory allocated for the particle
		m_Particles.erase(m_Particles.begin() + id);	// Remove the pointer from the vector
		m_ParticleCount--;								// Decrement the particle count
	}
	else {
		// Handle the case where the ID is out of range
		std::cerr << "Error: Particle ID " << id << " is out of range." << std::endl;
	}
}

unsigned int ParticleSystem::GetParticleCount() const 
{ 
	return m_ParticleCount; 
}