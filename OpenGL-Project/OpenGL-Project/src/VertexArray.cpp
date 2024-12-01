/**
 * @file VertexArray.cpp
 * @brief This file contains the VertexArray class and its method implementations.
 *
 * @details This file contains the VertexArray class and its method implementations.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

/**
 * @brief VertexArray constructor
 * 
 * @details
 * This constructor generates a new VertexArray object.
 */
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

/**
 * @brief VertexArray destructor
 * 
 * @details
 * This destructor deletes the VertexArray object from the GPU.
 */
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

/**
 * @brief Add a buffer to the VertexArray
 * 
 * @param vb The VertexBuffer to add
 * @param layout The VertexBufferLayout to add
 * 
 * @details
 * This method adds a VertexBuffer to the VertexArray with the given VertexBufferLayout.
 */
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}	
}

/**
 * @brief Bind the VertexArray to the GPU
 * 
 * @details
 * This method binds the VertexArray to the GPU.
 */
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

/**
 * @brief Unbind the VertexArray from the GPU
 * 
 * @details
 * This method unbinds the VertexArray from the GPU.
 */
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
