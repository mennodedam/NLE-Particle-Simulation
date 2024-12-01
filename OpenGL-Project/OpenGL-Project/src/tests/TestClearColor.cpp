/**
 * @file TestClearColor.cpp
 * @brief This file contains the clear color test class and its methods.
 *
 * @details This file contains the implementation of the clear color test.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

/**
 * @brief The test namespace contains the Test class and its methods.
 * 
 * @details
 * The test namespace contains the Test class and its methods.
 */
namespace test {

	/**
	 * @brief This is the constructor of the TestClearColor class.
	 * 
	 * @details
	 * This constructor initializes the TestClearColor class.
	 */
	TestClearColor::TestClearColor()
		: m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	/**
	 * @brief This is the destructor of the TestClearColor class.
	 * 
	 * @details
	 * This destructor destroys the TestClearColor class.
	 */
	TestClearColor::~TestClearColor()
	{
	}

	/**
	 * @brief This method updates the TestClearColor class.
	 * 
	 * @param deltaTime The time between the last frame and the current frame.
	 * 
	 * @details
	 * This method updates the TestClearColor class.
	 * This method is called every frame.
	 */
	void TestClearColor::OnUpdate(float deltaTime)
	{
	}

	/**
	 * @brief This method renders the TestClearColor class.
	 * 
	 * @details
	 * This method renders the TestClearColor class.
	 */
	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	/**
	 * @brief This method renders the ImGui of the TestClearColor class.
	 * 
	 * @details
	 * This method renders the ImGui of the TestClearColor class.
	 */
	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}