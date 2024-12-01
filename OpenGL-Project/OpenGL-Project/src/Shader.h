/**
 * @file Shader.h
 * @brief This file contains the Shader class and its methods.
 *
 * @details This file contains the shader class and its methods.
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
 * @struct ShaderProgramSource
 * 
 * @brief Shader program source
 * 
 * @details
 * This struct contains the source code for the vertex and fragment shader
 */
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

/** 
 * @class Shader
 * @brief Shader class
 * 
 * @details This class includes the method definitions for creating, binding, and
 * unbinding shaders within the Shader class.
 * 
 * The Shader class is used to create, bind, and unbind shaders.
 * The class also includes methods for uploading data to the GPU, setting uniforms,
 * and retrieving data from the GPU.
 */
class Shader
{
private:
	std::string m_Filepath_vertex;
	std::string m_Filepath_fragment;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath_vertex, const std::string& filepath_fragment);	///< tweede argument weg halen, doet op dit moment niks maar code breekt als het weghaalt
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	std::string ReadShaderFile(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};