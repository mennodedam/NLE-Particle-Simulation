#include "Shader.h"

#include "GLmacros.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath_vertex, const std::string& filepath_fragment)  ///< tweede argument weg halen, doet op dit moment niks maar code breekt als het weghaalt
	: m_Filepath_vertex(filepath_vertex), m_Filepath_fragment(filepath_fragment), m_RendererID(0)
{   
    //ShaderProgramSource source = ParseShader(filepath);
    //m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    m_RendererID = CreateShader(filepath_vertex, filepath_fragment);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

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

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

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