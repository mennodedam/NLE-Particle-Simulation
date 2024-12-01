/**
 * @file Texture.cpp
 * @brief This file contains the Texture class and its methods implementations.
 *
 * @details This file contains the Texture class and its methods implementations.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "Texture.h"

#include "stb_image/stb_image.h"

/**
 * @brief Texture constructor
 * 
 * @param path The path to the texture file
 * 
 * @details
 * This constructor loads the texture from the given path and stores it in the local buffer.
 */
Texture::Texture(const std::string& path)
	: m_RendererID(0), m_Filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // RGBA == 4

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

/**
 * @brief Texture destructor
 * 
 * @details
 * This destructor deletes the texture from the GPU.
 */
Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

/**
 * @brief Bind the texture to the GPU
 * 
 * @param slot The slot to bind the texture to
 * 
 * @details
 * This method binds the texture to the GPU.
 */
void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

/**
 * @brief Unbind the texture from the GPU
 * 
 * @details
 * This method unbinds the texture from the GPU.
 */
void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

