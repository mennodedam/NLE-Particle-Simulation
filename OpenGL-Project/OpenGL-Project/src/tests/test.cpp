/**
 * @file test.cpp
 * @brief This file contains the Test class and its methods.
 *
 * @details This file contains the implementation of the Test class and its methods.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "test.h"
#include "imgui/imgui.h"

/**
 * @brief This is the constructor of the Test class.
 * 
 * @details
 * This constructor initializes the Test class.
 */
namespace test {

	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}