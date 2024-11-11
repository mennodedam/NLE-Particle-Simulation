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
 */
ParticleSystem::ParticleSystem()
    :m_SSBO(0), m_PositionSSBO(0), m_VelocitySSBO(0), m_AccelerationSSBO(0), m_ComputeShaderProgram(0)
{
}

/**
 * @brief Destructor for ParticleSystem.
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
 */
void ParticleSystem::CreateParticle(glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
									glm::vec3 vel = { 0.0f, 0.0f, 0.0f }, 
									glm::vec3 acc = { 0.0f, 0.0f, 0.0f }, 
									float m = 1.0, float r = 1.0, 
									glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f}, 
									unsigned int id = 0)
{
	Particle particle(pos, vel, acc, m, r, color, id);
	m_Particles.emplace_back(pos, vel, acc, m, r, color, id);
}

/**
 * @brief Initialize SSBO 
 */
void ParticleSystem::initSSBOs()
{

}

/**
 * @brief Updates all particles in the particle system.
 *
 * @param particles A vector of particles to be updated using ComputeShader --> to be implemented later
 */
void ParticleSystem::UpdateParticles(float deltaTime)
{
	// Placeholder: Update logic will be added here in the future.
	// Call computeshader
}

/**
 * @brief Retrieve particle data
 */
void ParticleSystem::RetrieveParticleData()
{

}

/**
 * @brief Destroys a particle by its unique identifier.
 *
 * @param id The identifier of the particle to destroy.
 * @note This function ensures the specified particle is deleted and removed from the system.
 */
void ParticleSystem::DestroyParticle(unsigned int id)
{
	m_Particles.erase(
		std::remove_if(m_Particles.begin(), m_Particles.end(),
			[id](const Particle& p) { return p.getID() == id; }),
		    m_Particles.end());
}

bool ParticleSystem::LoadComputeShader(const std::string& filePath) {
    // Read shader source from file
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open compute shader file: " << filePath << std::endl;
        return false;
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    // Create and compile the compute shader
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    GLCall(glShaderSource(shader, 1, &shaderSource, nullptr));
    GLCall(glCompileShader(shader));

    // Check for compile errors
    GLint success;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success) 
    {
        char infoLog[512];
        GLCall(glGetShaderInfoLog(shader, 512, nullptr, infoLog));
        std::cerr << "Compute shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }

    // Link shader into a program
    m_ComputeShaderProgram = glCreateProgram();
    GLCall(glAttachShader(m_ComputeShaderProgram, shader));
    GLCall(glLinkProgram(m_ComputeShaderProgram));

    // Check for linking errors
    GLCall(glGetProgramiv(m_ComputeShaderProgram, GL_LINK_STATUS, &success));
    if (!success) 
    {
        char infoLog[512];
        GLCall(glGetProgramInfoLog(m_ComputeShaderProgram, 512, nullptr, infoLog));
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        return false;
    }

    // Clean up the shader as it’s no longer needed after linking
    GLCall(glDeleteShader(shader));

    std::cout << "Compute shader loaded successfully from " << filePath << std::endl;
    return true;
}


/**
 * @brief Retrieves the current count of particles in the particle system.
 * 
 * @return The number of particles in the system.
 */
unsigned int ParticleSystem::GetParticleCount() const 
{ 
	return m_Particles.size(); 
}	