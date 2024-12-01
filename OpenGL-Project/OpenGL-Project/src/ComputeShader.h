/**
 * @file ComputeShader.h
 * @brief This file contains the ComputeShader class and its methods.
 *
 * @details This file contains the ComputeShader class and its methods.
 * The ComputeShader class is used to create, bind, and unbind compute shaders.
 * The class also includes methods for uploading data to the GPU, setting uniforms,
 * and retrieving data from the GPU.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <string>
#include <unordered_map>
#include <stack>

#include <GL/glew.h>
#include "glm/glm.hpp" 

#include "Particlesystem.h"

/** 
 * @class ComputeShader
 * @brief ComputeShader class
 * 
 * @details This class includes the method definitions for creating, binding, and
 * unbinding compute shaders within the ComputeShader class.
 * 
 * The ComputeShader class is used to create, bind, and unbind compute shaders.
 * The class also includes methods for uploading data to the GPU, setting uniforms,
 * and retrieving data from the GPU.
 */
class ComputeShader
{
private:
	std::string m_Filepath;

	unsigned int m_RendererID;

	std::unordered_map<std::string, int> m_UniformLocationCache;

	GLuint m_SSBO;
	GLuint m_SSBO_ActiveID;

public:
	ComputeShader(const std::string& filepath);
	~ComputeShader();

	void Bind() const;
	void Unbind() const;

	void initSSBO(unsigned int size);
	void initSSBOActiveIDlist(unsigned int size);
	void UploadIDlist(const std::vector<unsigned int>& idlist);
	void UploadData(ParticleSystem& particlesystem);
	void UploadAddElement(ParticleSystem& particlesystem, Particle& newParticle, unsigned int position);
	void Update(ParticleSystem& particlesystem, float deltaTime);
	void RetrieveData(ParticleSystem& particlesystem);

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	std::string ReadShaderFile(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& computeshader);
	
	int GetUniformLocation(const std::string& name);
};