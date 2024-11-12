#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>

std::string ReadShaderFile(const std::string& filePath);
GLuint CompileShader(GLenum shaderType, const char* shaderCode);

GLuint LoadVertexShader(const char* vertexPath);
GLuint LoadFragmentShader(const char* fragmentPath);
GLuint LoadComputeShader(const char* computePath);