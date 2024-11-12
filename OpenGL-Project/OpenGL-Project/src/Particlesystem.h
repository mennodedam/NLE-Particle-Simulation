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
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include "vendor/glm/glm.hpp"

#include "Particle.h"
#include "Shader.h"

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

	void CreateParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float r,	glm::vec4 color, unsigned int id);
	void DestroyParticle(unsigned int id);

	void UploadParticleData();
	void UpdateParticles (float deltaTime);
	void RetrieveParticleData();
	

	void initSSBOs();
	bool LoadComputeShader(const std::string& filePath);
	bool LoadComputeShader2(const std::string& filePath);
	GLuint LoadVertexShaderProgram(const std::string& filePath);
	GLuint LoadFragmentShaderProgram(const std::string& filePath);
	bool LoadVertexFragmentProgram(const std::string& vertexPath, const std::string& fragmentPath);

	unsigned int GetParticleCount() const;

	void RenderParticles();

private:
	std::vector<Particle> m_Particles;  ///< Collection of pointers to particles in the system.	<-- NOG NIET KLAAR
	unsigned int m_ParticleCount = 0;	///< The current count of particles (initialized as 0).

	GLuint m_ComputeShaderProgram;		///< Handle for the compute shader program used for particle updates.
	GLuint m_ShaderProgram;				///< Handle for shader program (Shader/Vertex)

	GLuint m_SSBO;						///< Handle for Shader buffer storage object

	GLuint m_PositionSSBO, m_VelocitySSBO, m_AccelerationSSBO;

	std::vector<glm::vec3> m_PositionsGPU;
	std::vector<glm::vec3> m_VelocitiesGPU;
	std::vector<glm::vec3> m_AccelerationsGPU;

	GLuint m_ComputeShaderProgram2; /// voor loadhsader 2
};