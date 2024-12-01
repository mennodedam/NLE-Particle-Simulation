/**
 * @file TestParticles.h
 * @brief This file contains the Particle test class and its methods.
 *
 * @details This file contains the declaration of the Particle test.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <iostream>
#include <memory>

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "Particle.h"
#include "Particlesystem.h"

/**
 * @brief The test namespace contains the TestParticles class and its methods.
 * 
 * @details
 * The test namespace contains the TestParticles class and its methods.
 */
namespace test {

	/**
	 * @class TestParticles
	 * @brief The TestParticles class is a class that is used to create a Particle test.
	 * 
	 * @details
	 * The TestParticles class is a class that is used to create a Particle test.
	 */
	class TestParticles : public Test
	{
	public:
		TestParticles();
		~TestParticles();

		void OnUpdate(float delatTime) override;
		void OnRender() override;
		void OnImGuiRender() override;	

		float m_TimeElapsed;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<ComputeShader> m_ComputeShader;

		ParticleSystem m_Particlesystem;

	};

}