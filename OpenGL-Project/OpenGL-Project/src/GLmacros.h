/**
 * @file GLmacros.h
 * @brief Implements the GLmacros class for managing OpenGL error handling.
 *
 * @details This file includes the functions and macros for handling OpenGL errors.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include "GL/glew.h"

/**
 * @brief Asserts a condition and triggers a debug break if the condition is false.
 * 
 * @param x The condition to check.
 * 
 * @note This macro uses the `__debugbreak()` function, which is platform-specific and typically works on Windows.
 */
#define ASSERT(x) if (!(x)) __debugbreak();

/**
 * @brief Calls a specified OpenGL function after clearing any existing OpenGL errors, 
 *        and checks for errors after the call.
 * 
 * @param x The OpenGL function to call.
 * 
 * @note This macro ensures that OpenGL errors are logged and handled. It requires 
 *       `GlClearError()` and `GlLogCall()` to be implemented.
 */
#define GLCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);