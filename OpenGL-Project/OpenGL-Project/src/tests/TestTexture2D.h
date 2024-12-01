/**
 * @file TestTexture2D.h
 * @brief This file contains the TestTexture2D class and its methods.
 *
 * @details This file contains the declaration of the TestTexture2D class.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

/**
 * @brief The test namespace contains the TestCircle class and its methods.
 * 
 * @details
 * The test namespace contains the TestCircle class and its methods.
 */
namespace test {

	/**
	 * @class TestCircle
	 * @brief The TestCircle class is a class that is used to create a circle test.
	 * 
	 * @details
	 * The TestCircle class is a class that is used to create a circle test.
	 */
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float delatTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
	};

}