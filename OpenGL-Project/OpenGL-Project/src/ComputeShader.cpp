/**
 * @file ComputeShader.cpp
 * @brief This file contains the implementation for the ComputeShader class.
 *
 * @details This file contains the method definitions for creating, updating, and
 * retrieving data from compute shaders. The ComputeShader class is used to create,
 * bind, and unbind compute shaders, as well as upload data to the GPU, set uniforms,
 * and retrieve data from the GPU.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "ComputeShader.h"

#include "GLmacros.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
 * @brief Constructor
 * 
 * @param filepath path to the compute shader 
 */
ComputeShader::ComputeShader(const std::string& filepath)
    : m_Filepath(filepath), m_RendererID(0), m_SSBO(0), m_SSBO_ActiveID(0)
{  
    m_RendererID = CreateShader(filepath);
}

/**
 * @brief Destructor
 */
ComputeShader::~ComputeShader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

/**
 * @brief Read the shader file
 * 
 * @param filepath path to the shader file
 * @return std::string content of the shader file
 */
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

/**
 * @brief Compiles a shader and returns its ID.
 * 
 * @param type The type of shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER).
 * @param source The GLSL source code for the shader.
 * @return The OpenGL ID of the compiled shader. Returns 0 if compilation fails.
 * 
 * @details 
 * This method compiles a shader from the provided source code. If the shader fails to compile, 
 * an error message is printed to the console, and the shader ID is not created (returns 0). 
 * The method also handles OpenGL-specific compilation errors and cleans up resources if needed.
 */
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
            << (type == GL_VERTEX_SHADER   ? "vertex" :
                type == GL_FRAGMENT_SHADER ? "fragment" :
                type == GL_COMPUTE_SHADER  ? "compute" :
                "unknown")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        free(message);
        return 0;
    }

    return id;
}

/**
 * @brief Create the shader
 * 
 * @param computeshader content of the shader
 * @return unsigned int id of the shader
 * 
 * @details 
 * This method creates a shader from the provided source code. The method first creates a program
 * and then compiles the shader. If the shader fails to compile, an error message is printed to the
 * console, and the shader ID is not created (returns 0). The method also handles OpenGL-specific
 * compilation errors and cleans up resources if needed.
 */
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

/**
 * @brief Bind the shader
 * 
 * @details 
 * This method binds the shader to the OpenGL context. This method is called before
 * rendering to ensure that the correct shader is used.
 */
void ComputeShader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

/**
 * @brief Unbind the shader
 * 
 * @details 
 * This method unbinds the shader from the OpenGL context. This method is called after
 * rendering to ensure that the correct shader is used.
 */
void ComputeShader::Unbind() const
{
    GLCall(glUseProgram(0));
}

/**
 * @brief Initialize Shader Storage Buffer Object
 * 
 * @param size size of the buffer
 * 
 * @details
 * Preallocate memory to the gpu, sizeof(Data) * maxSize
 */
void ComputeShader::initSSBO(unsigned int size)
{
    GLCall(glGenBuffers(1, &m_SSBO));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Initialize Shader Storage Buffer Object for active id's
 * 
 * @param size size of the buffer
 * 
 * @details
 * Preallocate memory to the gpu, sizeof(unsigned int) * maxSize
 * This buffer is used to store the list of active id's
 */
void ComputeShader::initSSBOActiveIDlist(unsigned int size)
{
    GLCall(glGenBuffers(1, &m_SSBO_ActiveID));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO_ActiveID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief upload the list of active id's
 * 
 * @param idlist list of active id's
 * 
 * @details
 * Upload the list of active id's to the gpu
 */
void ComputeShader::UploadIDlist(const std::vector<unsigned int>& idlist)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO_ActiveID));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO_ActiveID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, idlist.size() * sizeof(unsigned int), idlist.data(), GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Upload data to the ssbo
 * 
 * @param particlesystem the data to upload
 * 
 * @details
 * Upload the data to the gpu
 */
void ComputeShader::UploadData(ParticleSystem& particlesystem)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, particlesystem.size() * sizeof(Particle), particlesystem.data(), GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Upload data to the ssbo
 * 
 * @param particlesystem the data to upload
 * @param position position to upload the data
 * @param newParticle the data to upload
 * 
 * @details
 * Upload the data to the gpu
 * This method is used to add a new particle to the ssbo
 * at a specific position
 */
void ComputeShader::UploadAddElement(ParticleSystem& particlesystem, Particle& newParticle, unsigned int position)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO));
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, position * sizeof(Particle), sizeof(Particle), &newParticle);
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

/**
 * @brief Update the compute shader
 * 
 * @param particlesystem the data to update
 * @param deltaTime time between frames
 * 
 * @details
 * Update the compute shader
 * Dispatch the compute shader
 * Memory barrier
 */
void ComputeShader::Update(ParticleSystem& particlesystem, float deltaTime)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform1f(glGetUniformLocation(m_RendererID, "deltaTime"), deltaTime));
    GLCall(glDispatchCompute((particlesystem.size() + 63) / 64, 1, 1));
    GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}

/**
 * @brief Retrieve data from the ssbo
 * 
 * @param particlesystem the data to retrieve
 * 
 * @details
 * Retrieve the data from the gpu
 * Copy the data to the particlesystem
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

/**
 * @brief Set uniform int
 * 
 * @param name name of the uniform
 * @param value value of the uniform
 * 
 * @details
 * Set the uniform int
 */
void ComputeShader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

/**
 * @brief Set uniform float
 * 
 * @param name name of the uniform
 * @param value value of the uniform
 * 
 * @details
 * Set the uniform float
 */
void ComputeShader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

/**
 * @brief Set uniform float4
 * 
 * @param name name of the uniform
 * @param v0 value of the uniform
 * @param v1 value of the uniform
 * @param v2 value of the uniform
 * @param v3 value of the uniform
 * 
 * @details
 * Set the uniform float4
 */
void ComputeShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

/**
 * @brief Set uniform mat4
 * 
 * @param name name of the uniform
 * @param matrix value of the uniform
 * 
 * @details
 * Set the uniform mat4
 */
void ComputeShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

/**
 * @brief Get the uniform location
 * 
 * @param name name of the uniform
 * @return int location of the uniform
 * 
 * @details
 * Get the uniform location
 * If the uniform location is not found, return -1
 */
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