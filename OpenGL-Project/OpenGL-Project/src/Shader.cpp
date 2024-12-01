/**
 * @file Shader.cpp
 * @brief This file contains implementations for the Shader class.
 *
 * @details 
 * The Shader class is used to create, bind, and unbind shaders.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "Shader.h"

#include "GLmacros.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
 * @brief Constructor
 * 
 * @param filepath_vertex path to the vertex shader
 * @param filepath_fragment path to the fragment shader
 * 
 * @details
 * Create the shader program
 */
Shader::Shader(const std::string& filepath_vertex, const std::string& filepath_fragment)  ///< tweede argument weg halen, doet op dit moment niks maar code breekt als het weghaalt
	: m_Filepath_vertex(filepath_vertex), m_Filepath_fragment(filepath_fragment), m_RendererID(0)
{   
    //ShaderProgramSource source = ParseShader(filepath);
    //m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    m_RendererID = CreateShader(filepath_vertex, filepath_fragment);
}

/**
 * @brief Destructor
 * 
 * @details
 * Delete the shader program
 */
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

/**
 * @brief Read the shader file
 * 
 * @param filepath path to the shader file
 * @return std::string content of the shader file
 * 
 * @details
 * Read the shader file
 * Return the content of the shader file
 */
std::string Shader::ReadShaderFile(const std::string& filepath)
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
 * @brief Compile a shader and return its ID
 * 
 * @param type type of the shader
 * @param source GLSL source code for the shader
 * @return unsigned int ID of the shader
 * 
 * @details
 * Compile a shader and return its ID
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
 * @brief Create a shader
 * 
 * @param vertexShader vertex shader
 * @param fragmentShader fragment shader
 * @return unsigned int ID of the shader
 * 
 * @details
 * Create a shader program
 */
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    std::string shadercode_vertex = ReadShaderFile(vertexShader);
    std::string shadercode_fragment = ReadShaderFile(fragmentShader);

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, shadercode_vertex);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shadercode_fragment);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

/**
 * @brief Bind the shader
 * 
 * @details
 * Bind the shader
 */
void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

/**
 * @brief Unbind the shader
 * 
 * @details
 * Unbind the shader
 */
void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
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
void Shader::SetUniform1i(const std::string& name, int value)
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
void Shader::SetUniform1f(const std::string& name, float value)
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
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
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
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
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
 */
int Shader::GetUniformLocation(const std::string& name)
{   
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation (m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "'doesn't exist!" << std::endl;
   
    m_UniformLocationCache[name] = location;
    return location;
}