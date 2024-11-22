#pragma once

#include <vector>
#include <iostream>

#include "vendor/glm/glm.hpp" 

class Particle
{
public:
	Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color, unsigned int id);
	~Particle();

	unsigned int getID() const { return m_ParticleID; }

private:
	unsigned int m_ParticleID;	///< Id of the particle

	float m_Radius;
	float m_Mass;

	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;

	glm::vec3 m_PastPosition;
	glm::vec3 m_PastVelocity;
	glm::vec3 m_PastAcceleration;

	glm::vec4 m_ParticleColor;	
};


