/**
 * @file TestClearColor.h
 * @brief This file contains the clear color test class and its methods.
 *
 * @details This file contains the declaration of the clear color test.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include "Test.h"

/**
 * @brief The test namespace contains the TestClearColor class and its methods.
 * 
 * @details
 * The test namespace contains the TestClearColor class and its methods.
 */
namespace test {

	/**
	 * @class TestClearColor
	 * @brief The TestClearColor class is a class that is used to create a clear color test.
	 * 
	 * @details
	 * The TestClearColor class is a class that is used to create a clear color test.
	 */
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float delatTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
	};

}