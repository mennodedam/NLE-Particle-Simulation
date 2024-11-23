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

ImVec2 mousePos;

namespace test {

    TestParticles::TestParticles()
        : m_TimeElapsed(0.0f)
    {
        std::cout << "Start Particle Test" << std::endl;

        m_Shader        = std::make_unique<Shader>("res/shaders/basicParticle.shader", "renderer");
        m_ComputeShader = std::make_unique<Shader>("res/shaders/BasicCompute.glsl", "compute");

        m_ComputeShader->initSSBO();

        std::cout << "size of Particle class: " << sizeof(Particle) << std::endl;

        m_Particlesystem.InitFreelist();
    }

    TestParticles::~TestParticles()
    {
        std::cout << "End Particle Test" << std::endl;
    }

    void TestParticles::OnUpdate(float deltaTime)
    {
        if (m_Particlesystem.GetParticleCount() != 0)
        {
            //m_ComputeShader->UploadData(m_Particlesystem);    // doe dit alleen wanneer data van Particle vector verandert.
            m_ComputeShader->Update(m_Particlesystem, deltaTime);
            //m_ComputeShader->RetrieveData(m_Particlesystem);  // alleen doen wanneer nodig, ipv bij elke dispatch van de computeshader
            m_TimeElapsed += deltaTime;
        }
    }

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

    void TestParticles::OnImGuiRender()
    {
        ImGui::Text("Particle count: %d", m_Particlesystem.GetParticleCount());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Total time elapsed:%.3f", m_TimeElapsed);
        ImGui::Text("Mouse Clicked at: (%.3f,%.3f)", mousePos.x, mousePos.y);

        if (ImGui::Button("Create Particle"))
        {
            //m_Particlesystem.CreateParticle(position, velocity, accelleration, mass, radius, color, m_Particlesystem.GetParticleCount() + 1);
            m_Particlesystem.CreateParticle(position, velocity, accelleration, mass, radius, color);
            m_ComputeShader->UploadData(m_Particlesystem);
        }

        ImGui::InputInt("Particle ID", &particleID);
        if (ImGui::Button("Remove Particle"))
        {
            m_Particlesystem.DestroyParticle(particleID);
            m_ComputeShader->UploadData(m_Particlesystem);
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            mousePos = ImGui::GetMousePos();
        }

        if (ImGui::Button("Print ID's"))
        {
            m_Particlesystem.PrintIDlist();
        }

    }

}

