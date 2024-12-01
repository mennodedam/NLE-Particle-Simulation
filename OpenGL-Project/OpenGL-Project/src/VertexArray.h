/**
 * @file  VertexArray.h
 * @brief VertexArray class header file.
 *
 * @details This file contains the VertexArray class declaration.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include "VertexBuffer.h"

/**
 * @class VertexBufferLayout
 * @brief A class that represents a VertexBufferLayout
 * 
 * @details
 * This class represents a VertexBufferLayout that can be used to store VertexBuffers.
 */
class VertexBufferLayout;	///< Forward declaration of VertexBufferLayout

/**
 * @class VertexArray
 * @brief A class that represents a VertexArray
 * 
 * @details
 * This class represents a VertexArray that can be used to store VertexBuffers.
 */
class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};