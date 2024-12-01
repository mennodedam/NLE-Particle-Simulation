/**
 * @file TestTexture2D.cpp
 * @brief This file contains the texture test class and its methods.
 *
 * @details This file contains the implementation of the texture test.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @brief The test namespace contains the TestTexture2D class and its methods.
 * 
 * @details
 * The test namespace contains the TestTexture2D class and its methods.
 */
namespace test {

    /**
     * @brief This is the constructor of the TestTexture2D class.
     * 
     * @details
     * This constructor initializes the TestTexture2D class.
     */
    TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), 
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{

        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f, //0
            200.0f, 100.0f, 1.0f, 0.0f, //1
            200.0f, 200.0f, 1.0f, 1.0f, //2
            100.0f, 200.0f, 0.0f, 1.0f  //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Texture/Vertex.glsl", "res/shaders/Texture/Fragment.glsl");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        m_Texture = std::make_unique<Texture>("res/textures/image.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

    /**
     * @brief This is the destructor of the TestTexture2D class.
     * 
     * @details
     * This destructor destroys the TestTexture2D class.
     */
	TestTexture2D::~TestTexture2D()
	{
	}

    /**
     * @brief This method updates the TestTexture2D class.
     * 
     * @param deltaTime The time between the last frame and the current frame.
     * 
     * @details
     * This method updates the TestTexture2D class.
     * This method is called every frame.
     */
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

    /**
     * @brief This method renders the TestTexture2D class.
     * 
     * @details
     * This method renders the TestTexture2D class.
     */
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

    /**
     * @brief This method renders the ImGui of the TestTexture2D class.
     * 
     * @details
     * This method renders the ImGui of the TestTexture2D class.
     */
	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}