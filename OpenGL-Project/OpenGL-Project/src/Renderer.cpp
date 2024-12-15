/**
 * @file Renderer.cpp
 * @brief This file contains the method definitions for creating, updating, and
 * retrieving data from compute shaders. 
 *
 * @details This file contains the method definitions for the ComputeShader class. 
 * The ComputeShader class is used to create, update, and retrieve data from compute shaders.
 * The class is used to create a compute shader, update the data in the shader, and retrieve the data from the shader.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "Renderer.h"

#include <iostream>

/**
 * @brief Clear the screen
 * 
 * @details
 * Clear the screen
 */
void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

/**
 * @brief Draw the vertex array
 * 
 * @param va vertex array
 * @param ib index buffer
 * @param shader shader
 * 
 * @details
 * Draw the vertex array
 * Bind the vertex array
 * Bind the index buffer
 * Draw the elements
 */
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawInstanced(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr, count))
}

