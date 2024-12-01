/**
 * @file ParticleSystem.cpp
 * @brief Implements the ParticleSystem class for managing particles in the simulation.
 *
 * @details This file includes the method definitions for creating, updating, and
 * destroying particles within the ParticleSystem class.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "Particlesystem.h"

#include "Shader.h"
#include "Renderer.h"

 /**
  * @brief Constructs a ParticleSystem instance with an initial particle count of zero.
  * 
  * @details
  * The constructor initializes the particle count to zero and sets the maximum number of particles
  * that can be created in the system. The freelist is populated with the maximum number of particles
  * and the IDlist is initialized with the same values.
  */
ParticleSystem::ParticleSystem()
{
}

/**
 * @brief Destructor for ParticleSystem.
 * 
 * @details
 * Deletes all particles created within the system to free memory. 
 */
ParticleSystem::~ParticleSystem()
{
}

/**
 * @brief Creates a new particle with specified properties and adds it to the particle system.
 *
 * @param pos The initial position of the particle (glm::vec3).
 * @param vel The initial velocity of the particle (glm::vec3).
 * @param acc The acceleration applied to the particle (glm::vec3).
 * @param m The mass of the particle.
 * @param r The radius of the particle.
 * @param color The color of the particle (glm::vec4).
 * 
 * @details
 * This function creates a new particle with the specified properties and adds it to the particle system.
 * The particle is assigned a unique identifier and added to the list of particles. The particle count is
 * updated and the unique identifier is returned.
 */
size_t ParticleSystem::CreateParticle(
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
    glm::vec3 vel = { 0.0f, 0.0f, 0.0f },
    glm::vec3 acc = { 0.0f, 0.0f, 0.0f },
    float m = 1.0, float r = 1.0,
    glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f })
{
    if (!m_Freelist.empty())
    {
        size_t freeIndex = m_Freelist.top();
        m_Freelist.pop();
        m_Particles.emplace_back(pos, vel, acc, m, r, color, freeIndex);
        m_IDlist.push_back(freeIndex);
        m_ParticleCount = GetParticleCount();
        return freeIndex;
    }
    else
    {
        std::cerr << "No free slots available for new particles!" << std::endl;
    }
    return 0;
}

/**
 * @brief Destroys a particle by its unique identifier.
 *
 * @param id The identifier of the particle to destroy.
 * @note This function ensures the specified particle is deleted and removed from the system.
 * 
 * @details
 * This function destroys a particle by its unique identifier. The particle is removed from the list of particles
 * and the unique identifier is added back to the freelist. The particle count is updated and the unique identifier
 * is removed from the list of identifiers.
 */
void ParticleSystem::DestroyParticle(unsigned int id)       ///< werkt niet als computeshader wordt gecalled.
{
    m_Freelist.push(id);

    m_Particles.erase
    (
        std::remove_if(m_Particles.begin(), m_Particles.end(),
            [id](const Particle& p) { return p.getID() == id; }),
        m_Particles.end()
    );

    m_Particles.shrink_to_fit();
    m_ParticleCount = GetParticleCount();
    m_IDlist.erase(std::remove(m_IDlist.begin(), m_IDlist.end(), id), m_IDlist.end());
}

/**
 * @brief print list of id's to the console
 * 
 * @details
 * This function prints the list of unique identifiers to the console.
 */
void ParticleSystem::PrintIDlist()
{
    std::cout << "Printing id list, size: " << m_IDlist.size() << std::endl;
    for (int i = 0; i < m_IDlist.size(); i++)
        std::cout << m_IDlist[i] << std::endl;
    std::cout << "\n" << std::endl;
}


