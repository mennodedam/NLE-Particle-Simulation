/**
 * @file TestCircle.h
 * @brief This file contains the circle test class and its methods.
 *
 * @details This file contains the declaration of the circle test.
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
	class TestCircle : public Test
	{
	public:
		TestCircle();
		~TestCircle();

		void OnUpdate(float delatTime) override;
		void OnRender() override;
		void OnImGuiRender() override;	 
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;

		float m_CircleColor[4];
	};

}