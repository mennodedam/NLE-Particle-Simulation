/**
 * @file vertexbuffer.cpp
 * @brief This file contains the vertexbuffer class and its methods.
 *
 * @details this file contains the vertexbuffer class and its methods.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "VertexBuffer.h"

#include "Renderer.h"

/**
 * @brief VertexBuffer constructor
 * 
 * @param data The data to store in the buffer
 * @param size The size of the data
 * 
 * @details
 * This constructor generates a new VertexBuffer object and stores the given data in it.
 */
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/**
 * @brief VertexBuffer destructor
 * 
 * @details
 * This destructor deletes the VertexBuffer object from the GPU.
 */
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

/**
 * @brief Bind the VertexBuffer to the GPU
 * 
 * @details
 * This method binds the VertexBuffer to the GPU.
 */
void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

/**
 * @brief Unbind the VertexBuffer from the GPU
 * 
 * @details
 * This method unbinds the VertexBuffer from the GPU.
 */
void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}