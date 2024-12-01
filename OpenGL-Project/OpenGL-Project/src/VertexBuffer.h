/**
 * @file vertexbuffer.h
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

#pragma once

/**
 * @class VertexBuffer
 * @brief A class that represents a vertexbuffer
 * 
 * @details
 * This class represents a vertexbuffer that can be used to store vertex data.
 */
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};