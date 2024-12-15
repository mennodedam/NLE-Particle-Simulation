/**
 * @file GLmacros.cpp
 * @brief Implements the GLmacros functions for managing OpenGL error handling.
 *
 * @details This file includes the functions for handling OpenGL errors.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "GLmacros.h"

#include <iostream>

/**
 * @brief Clears the OpenGL error buffer.
 * 
 * @details Opengl maintains an error buffer that stores all errors that occur.
 *         This function clears that buffer.
 */
void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/**
 * @brief Logs an OpenGL error.
 * 
 * @param function The function that caused the error.
 * @param file The file that caused the error.
 * @param line The line that caused the error.
 * @return `true` if no errors occurred, `false` otherwise.
 * 
 * @details This function logs an OpenGL error.
 */
bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}