/**
 * @file TestParticles.cpp
 * @brief This file contains the Particle test class and its methods.
 *
 * @details This file contains the implementation of the Particle test.
 *
 * For more information, see the documentation at:
 * @link https://github.com/mennodedam/NLE-Particle-Simulation @endlink
 *
 * @date 1-11-2024
 * @author Menno Eijkelenboom
 */

#include "TestParticles.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// temp values.
glm::vec3 position = { 1.0f, 1.0f, 0.0f };
glm::vec3 velocity = { 10.0f, 1000.0f, 0.0f };
glm::vec3 accelleration = { 1.0f, 1.0f, 0.0f };
glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
float mass = 1.0f;
float radius = 1.0f;

int particleID = 0;
int memorySize = 0;

ImVec2 mousePos;

/**
 * @brief The test namespace contains the TestParticles class and its methods.
 * 
 * @details
 * The test namespace contains the TestParticles class and its methods.
 */
namespace test {

    /**
     * @brief This is the constructor of the TestParticles class.
     * 
     * @details
     * This constructor initializes the TestParticles class.
     */
    TestParticles::TestParticles()
        : m_TimeElapsed(0.0f)
    {
        std::cout << "Start Particle Test" << std::endl;

        m_Shader        = std::make_unique<Shader>("res/shaders/ParticleShaders/Vertex.glsl", "res/shaders/ParticleShaders/Fragment.glsl");
        m_ComputeShader = std::make_unique<ComputeShader>("res/shaders/ParticleShaders/Compute.glsl");

        m_ComputeShader->initSSBO(m_Particlesystem.GetMaxNumber());
        m_ComputeShader->initSSBOActiveIDlist(m_Particlesystem.GetMaxNumber());

        m_Particlesystem.InitFreelist();

        std::cout << "size of Particle class: " << sizeof(Particle) << std::endl;
        std::cout << "Maximum amount of particles: " << m_Particlesystem.GetMaxNumber() << std::endl;
        memorySize = m_Particlesystem.GetMaxNumber();
    }

    /**
     * @brief This is the destructor of the TestParticles class.
     * 
     * @details
     * This destructor destroys the TestParticles class.
     */
    TestParticles::~TestParticles()
    {
        std::cout << "End Particle Test" << std::endl;
    }

    /**
     * @brief This method updates the TestParticles class.
     * 
     * @param deltaTime The time between the last frame and the current frame.
     * 
     * @details
     * This method updates the TestParticles class.
     * This method is called every frame.
     */
    void TestParticles::OnUpdate(float deltaTime)
    {
        if (m_Particlesystem.GetParticleCount() != 0)
        {
            m_ComputeShader->Update(m_Particlesystem, deltaTime);
            m_ComputeShader->RetrieveData(m_Particlesystem);  // alleen doen wanneer nodig, ipv bij elke dispatch van de computeshader
            m_TimeElapsed += deltaTime;
        }
    }
    
    /**
     * @brief This method renders the TestParticles class.
     * 
     * @details
     * This method renders the TestParticles class.
     */
    void TestParticles::OnRender()  ///< NOG NIET AF.
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        {
            m_Shader->Bind();
            //renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);   ///< *m_VAO en *m_IndexBuffer zijn placeholder.
        }
    }

    /**
     * @brief This method renders the ImGui of the TestParticles class.
     * 
     * @details
     * This method renders the ImGui of the TestParticles class.
     */
    void TestParticles::OnImGuiRender()
    {
        ImGui::Text("Particle count: %d", m_Particlesystem.GetParticleCount());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Total time elapsed:%.3f", m_TimeElapsed);
        
        if (ImGui::Button("Create Particle"))
        {
            int freeindex = m_Particlesystem.CreateParticle(position, velocity, accelleration, mass, radius, color);
            //m_ComputeShader->UploadData(m_Particlesystem);          ///< op dit moment overwrite dit de preallocated memory van initSSBO()

            unsigned int NewestID = m_Particlesystem.ReturnVectorSize()-1;
            Particle newParticle = m_Particlesystem.ReturnParticle(NewestID); 
            m_ComputeShader->UploadAddElement(m_Particlesystem, newParticle, NewestID);

            //m_ComputeShader->UploadIDlist(m_Particlesystem.IDlistData());     
        }

        ImGui::InputInt("Particle ID", &particleID);
        if (ImGui::Button("Remove Particle"))
        {
            m_Particlesystem.DestroyParticle(particleID);
            m_ComputeShader->UploadData(m_Particlesystem);          ///< op dit moment overwrite dit de preallocated memory van initSSBO()
        }

        ImGui::Text("Mouse Clicked at: (%.3f,%.3f)", mousePos.x, mousePos.y);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            mousePos = ImGui::GetMousePos();
        }

        if (ImGui::Button("Print ID's"))
        {
            m_Particlesystem.PrintIDlist();
        }

        ImGui::Text("Memory Pool: %d Particles", m_Particlesystem.GetMaxNumber());
        ImGui::InputInt("Memory pool", &memorySize);
        if (ImGui::Button("Update Memory Pool"))
        {
            m_Particlesystem.MemorySize(memorySize);
            std::cout << "updated Memory pool to " << memorySize << std::endl;
            m_ComputeShader->initSSBO(m_Particlesystem.GetMaxNumber());   // reallocate memory on gpu
            m_ComputeShader->UploadData(m_Particlesystem);                // upload data to gpu.
        }

    }

}

