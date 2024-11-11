#include "Particle.h"

Particle::Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color)
	: m_Position(pos), m_Velocity(vel), m_Acceleration(acc),
	m_Mass(m), m_Radius(r), 
	m_PastPosition({0.0f, 0.0f, 0.0f}), m_PastVelocity({ 0.0f, 0.0f, 0.0f }), m_PastAcceleration({ 0.0f, 0.0f, 0.0f }),
	m_ParticleID(0),
	m_ParticleColor({ color[0],color[2],color[3],color[4] })
{
}

Particle::~Particle()
{
}


