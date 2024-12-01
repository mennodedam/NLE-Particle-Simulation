/**
 * @file Particle.cpp
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

#include "Particle.h"

/**
 * @brief Default constructor for the Particle class.
 * 
 * @details
 * This constructor initializes a Particle object with default values.
 */
Particle::Particle
(
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
    glm::vec3 vel = { 0.0f, 0.0f, 0.0f },
    glm::vec3 acc = { 0.0f, 0.0f, 0.0f },
    float m = 1.0f,
    float r = 1.0f,
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
    unsigned int id = 0
)
    : m_Position(pos), m_Velocity(vel), m_Acceleration(acc),
    m_Mass(m), m_Radius(r),
    m_PastPosition({ 0.0f, 0.0f, 0.0f }), m_PastVelocity({ 0.0f, 0.0f, 0.0f }), m_PastAcceleration({ 0.0f, 0.0f, 0.0f }),
    m_ParticleID(id),
    m_ParticleColor(color)
{
}

/**
 * @brief Destructor for the Particle class.
 * 
 * @details
 * This destructor destroys a Particle object.
 */
Particle::~Particle()
{

}