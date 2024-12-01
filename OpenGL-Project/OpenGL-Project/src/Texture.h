/**
 * @file texture.h
 * @brief This file contains the Texture class and its methods.
 *
 * @details 
 * This file contains the Texture class and its methods.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include "Renderer.h"

/**
 * @class Texture
 * @brief A class that represents a texture
 * 
 * @details
 * This class represents a texture that can be loaded from a file and bound to the GPU.
 */
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};