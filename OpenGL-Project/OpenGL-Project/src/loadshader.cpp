#include "loadshader.h"

std::string ReadShaderFile(const std::string& filePath)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << "\n";
        return "";
    }
}

// Compile a shader and check for errors
GLuint CompileShader(GLenum shaderType, const char* shaderCode)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint LoadVertexShader(const char* vertexPath)
{
    std::string vertexCode = ReadShaderFile(vertexPath);
    return CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
}

GLuint LoadFragmentShader(const char* fragmentPath)
{
    std::string fragmentCode = ReadShaderFile(fragmentPath);
    return CompileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
}

GLuint LoadComputeShader(const char* computePath)
{
    std::string computeCode = ReadShaderFile(computePath);
    return CompileShader(GL_COMPUTE_SHADER, computeCode.c_str());
}