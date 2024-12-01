/**
 * @file Renderer.h
 * @brief This file contains the ComputeShader class and its methods.
 *
 * @details This file contains the ComputeShader class and its methods.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "GLmacros.h"

/**
 * @class Renderer
 * @brief Renderer class
 * 
 * @details 
 * This class includes the method definitions for creating, binding, and
 * unbinding compute shaders within the ComputeShader class.
 * 
 * The ComputeShader class is used to  bind and unbind shaders.
 * The class also includes the method for drawing the vertex array.
 */
class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};