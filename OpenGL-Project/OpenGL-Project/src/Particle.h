/**
 * @file Particle.h
 * @brief Implements the Particle class for managing particles in the simulation.
 *
 * @details This file includes the method definitions for creating, updating, and
 * destroying particles within the Particle class.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <vector>
#include <iostream>

#include "vendor/glm/glm.hpp" 

/**
 * @class Particle
 * @brief The Particle class for managing particles in the simulation.
 * 
 * @details
 * This class contains the properties of a particle, such as its position, velocity,
 * acceleration, mass, radius, color, and ID.
 */
class Particle
{
public:
	Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r, glm::vec4 color, unsigned int id);
	~Particle();

	unsigned int getID() const { return m_ParticleID; }

private:
    unsigned int m_ParticleID; ///< Id of the particle (4 bytes)

    float m_Radius;            
    float m_Mass;              
    float padding1 = 0.0f;//float padding1;            

    glm::vec3 m_Position;      
    float padding2 = 0.0f;//float padding2;            

    glm::vec3 m_Velocity;      
    float padding3 = 0.0f;//float padding3;

    glm::vec3 m_Acceleration;  
    float padding4 = 0.0f;//float padding4;            

    glm::vec3 m_PastPosition;  
    float padding5 = 0.0f;//float padding5;            

    glm::vec3 m_PastVelocity;  
    float padding6 = 0.0f;//float padding6;            

    glm::vec3 m_PastAcceleration; 
    float padding7 = 0.0f;// float padding7;               

    glm::vec4 m_ParticleColor;
};


