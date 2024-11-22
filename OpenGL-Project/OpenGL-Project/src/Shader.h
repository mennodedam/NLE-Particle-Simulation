#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include "glm/glm.hpp" 

#include "Particlesystem.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_Filepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	GLuint m_SSBO;

public:
	Shader(const std::string& filepath, const std::string& shadertype);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void initSSBO();
	void UploadData(ParticleSystem &particlesystem);
	void Update(ParticleSystem &particlesystem, float deltaTime);
	void RetrieveData(ParticleSystem &particlesystem);

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateShaderCompute(const std::string& computeshader);
	std::string ReadShaderFile(const std::string& filepath);

	int GetUniformLocation(const std::string& name);
};