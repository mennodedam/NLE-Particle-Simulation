/**
 * @file test.h
 * @brief This file contains the Test class and its methods.
 *
 * @details This file contains the declaration of the Test class and its methods.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#pragma once

#include <vector>
#include <string>
#include <functional> 
#include <iostream>

/**
 * @brief The test namespace contains the Test class and its methods.
 * 
 * @details
 * The test namespace contains the Test class and its methods.
 */
namespace test {

	/**
	 * @class Test
	 * @brief The Test class is a virtual class that is used to create tests.
	 * 
	 * @details
	 * The Test class is a virtual class that is used to create tests.
	 */
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	/**
	 * @class
	 * @brief The TestMenu class is a class that is used to create a menu for the tests.
	 * 
	 * @details
	 * The TestMenu class is a class that is used to create a menu for the tests.
	 */
	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test " << name << std::endl;	

			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}