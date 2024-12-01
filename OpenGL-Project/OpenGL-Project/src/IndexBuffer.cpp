/**
 * @file IndexBuffer.cpp
 * @brief Implements the IndexBuffer class for managing index buffers in the simulation.
 *
 * @details This file includes the method definitions for creating, binding, and
 * unbinding index buffers within the IndexBuffer class.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "IndexBuffer.h"

#include "Renderer.h"

/**
 * @brief Constructor
 * 
 * @param data data to be stored in the buffer
 * @param count number of elements in the buffer
 * 
 * @details 
 * Creates a buffer and stores the data in it
 * Binds the buffer to the GL_ELEMENT_ARRAY_BUFFER
 */
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

/**
 * @brief Destructor
 * 
 * @details
 * Deletes the buffer
 */
IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

/**
 * @brief Bind the buffer
 * 
 * @details
 * Bind the buffer to the GL_ELEMENT_ARRAY_BUFFER
 */
void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

/**
 * @brief Unbind the buffer
 * 
 * @details
 * Unbind the buffer from the GL_ELEMENT_ARRAY_BUFFER
 */
void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}