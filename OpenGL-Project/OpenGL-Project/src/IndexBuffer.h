/**
 * @file IndexBuffer.h
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

#pragma once

/**
 * @class IndexBuffer
 * @brief IndexBuffer class
 * 
 * @details
 * This class is used to manage index buffers in the simulation.
 * It includes methods for creating, binding, and unbinding index buffers.
 */
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count;  }
};