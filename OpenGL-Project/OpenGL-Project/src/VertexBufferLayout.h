/**
 * @file vertexbufferlayout.h
 * @brief This file contains the VertexBufferLayout class and its methods.
 *
 * @details this file contains the VertexBufferLayout class and its methods.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <vector>
#include "Renderer.h"

/**
 * @struct VertexBufferElement
 * @brief A class that represents a VertexBufferElement
 * 
 * @details
 * This class represents a VertexBufferElement that can be used to store the type, count and normalized value of a VertexBufferElement.
 */
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

/**
 * @class VertexBufferLayout
 * @brief A class that represents a VertexBufferLayout
 * 
 * @details
 * This class represents a VertexBufferLayout that can be used to store VertexBufferElements.
 */
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		//static_assert(false); //werkt niet in vs2022
		static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::Push");
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};