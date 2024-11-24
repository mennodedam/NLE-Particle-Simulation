#include "ComputeShader.h"

#include "GLmacros.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

ComputeShader::ComputeShader(const std::string& filepath)
    : m_Filepath(filepath), m_RendererID(0), m_SSBO(0), m_SSBO_ActiveID(0)
{  
    m_RendererID = CreateShader(filepath);
}

ComputeShader::~ComputeShader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

unsigned int ComputeShader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        if (message == NULL) { std::cout << "Memory allocation failed" << std::endl; }
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" :
                type == GL_FRAGMENT_SHADER ? "fragment" :
                type == GL_COMPUTE_SHADER ? "compute" :
                "unknown")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        free(message);
        return 0;
    }

    return id;
}

unsigned int ComputeShader::CreateShader(const std::string& computeshader)
{
    unsigned int program = glCreateProgram();

    std::string shadercode = ReadShaderFile(computeshader);
    unsigned int cs = CompileShader(GL_COMPUTE_SHADER, shadercode);

    glAttachShader(program, cs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(cs);

    return program;
}

std::string ComputeShader::ReadShaderFile(const std::string& filepath)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(filepath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filepath << "\n";
        return "";
    }
}

void ComputeShader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void ComputeShader::Unbind() const
{
    GLCall(glUseProgram(0));
}

/**
 * @brief Initialize Shader Storage Buffer Object
 *
 * Preallocate memory to the gpu, sizeof(Data) * maxSize
 */
void ComputeShader::initSSBO(unsigned int maxSize)
{
    GLCall(glGenBuffers(1, &m_SSBO));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, maxSize * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Initialize Shader Storage Buffer Object containing the list of active id's
 *
 * Preallocate memory to the gpu, sizeof(Data) * maxSize
 */
void ComputeShader::initActiveIDlist(unsigned int maxSize)
{
    GLCall(glGenBuffers(1, &m_SSBO_ActiveID));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO_ActiveID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, maxSize * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief upload the list of active id's
 *
 * Upload the entire vector of active id's to the gpu
 */
void ComputeShader::UpdateIDlist(const std::vector<unsigned int>& idlist)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO_ActiveID));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO_ActiveID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, idlist.size() * sizeof(unsigned int), idlist.data(), GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief upload the list of active id's
 *
 * Upload the entire vector of particles to the gpu
 */
void ComputeShader::UploadData(ParticleSystem& particlesystem)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, particlesystem.size() * sizeof(Particle), particlesystem.data(), GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Add a single element to the ssbo at a position.
 *
 * Dont forget to update the list of active id's
 */
void ComputeShader::UploadAddElement(ParticleSystem& particlesystem, Particle& newParticle, unsigned int position)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO));
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, position * sizeof(Particle), sizeof(Particle), &newParticle);
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Call compute shader
 */
void ComputeShader::Update(ParticleSystem& particlesystem, float deltaTime)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform1f(glGetUniformLocation(m_RendererID, "deltaTime"), deltaTime));
    GLCall(glDispatchCompute((particlesystem.size() + 63) / 64, 1, 1));
    GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}

/**
 * @brief retrieve data from compute shader
 */
void ComputeShader::RetrieveData(ParticleSystem& particlesystem)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    Particle* mappedData = (Particle*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    if (mappedData == nullptr)
    {
        std::cerr << "Error: Failed to map SSBO buffer." << std::endl;
        GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
        return;
    }

    std::memcpy(particlesystem.data(), mappedData, particlesystem.size() * sizeof(Particle));
    GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void ComputeShader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void ComputeShader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void ComputeShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void ComputeShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int ComputeShader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "'doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}