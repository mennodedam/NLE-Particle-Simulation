#pragma once

#include <vector>
#include "vendor/glm/glm.hpp" 

class Particle
{
public:
	Particle();
	~Particle();

	void UpdateParticle() const;
	void CheckCollision() const;

private:
	unsigned int m_ParticleID;

	float m_Radius;
	float m_Mass;

	glm::vec4 m_Position;
	glm::vec4 m_Velocity;
	glm::vec4 m_Acceleration;

	glm::vec4 m_PastPosition;
	glm::vec4 m_PastVelocity;
	glm::vec4 m_PastAcceleration;

	glm::vec4 m_ParticleColor;
};