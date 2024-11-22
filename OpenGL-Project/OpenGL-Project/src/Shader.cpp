#include "Shader.h"

//#include "Renderer.h"
#include "GLmacros.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath, const std::string& shadertype)
	: m_Filepath(filepath), m_RendererID(0), m_SSBO(0)
{   
    if (shadertype == "renderer")
    {
        ShaderProgramSource source = ParseShader(filepath);
        m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    }
    else if (shadertype == "compute")
    {
        m_RendererID = CreateShaderCompute(filepath);
    }
    else
    {
        std::cout << "Error: unknown shadertype!" << std::endl;
    }
    
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
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


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::CreateShaderCompute(const std::string& computeshader)
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

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::initSSBO()
{
    GLCall(glGenBuffers(1, &m_SSBO));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void Shader::UploadData(ParticleSystem &particlesystem)
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, particlesystem.size() * sizeof(Particle), particlesystem.data(), GL_DYNAMIC_DRAW));
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO));
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void Shader::Update(ParticleSystem &particlesystem, float deltaTime)
{
    GLCall(glUseProgram(m_RendererID));
    GLCall(glUniform1f(glGetUniformLocation(m_RendererID, "deltaTime"), deltaTime));
    GLCall(glDispatchCompute((particlesystem.size() + 63) / 64, 1, 1));
    GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}

void Shader::RetrieveData(ParticleSystem &particlesystem)
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