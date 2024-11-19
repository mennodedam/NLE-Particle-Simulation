#include "TestParticles.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// temp values.
glm::vec3 position = { 1.0f, 1.0f, 0.0f };
glm::vec3 velocity = { 1.0f, 1.0f, 0.0f };
glm::vec3 accelleration = { 1.0f, 1.0f, 0.0f };
glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };

float mass = 1.0f;
float radius = 1.0f;

int particleID = 0;

ImVec2 mousePos;

namespace test {

    TestParticles::TestParticles()
    {
        std::cout << "Start Particle Test" << std::endl;
        //m_Particlesystem.LoadComputeShader("res/shaders/BasicCompute.glsl");
        m_Particlesystem.LoadComputeShader2("res/shaders/BasicCompute.glsl");
        m_Particlesystem.LoadVertexFragmentProgram("res/shaders/BasicVertex.glsl", "res/shaders/BasicFragment.glsl");
        m_Particlesystem.initSSBOs();
    }

    TestParticles::~TestParticles()
    {
        std::cout << "End Particle Test" << std::endl;
    }

    void TestParticles::OnUpdate(float deltaTime)
    {
        //m_Particlesystem.UploadParticleData();
        //m_Particlesystem.UpdateParticles(deltaTime);    
        //m_Particlesystem.RetrieveParticleData();            ///< sommige data wordt fout overgeschreven. ID bijvoorbeeld klopt niet.
    }

    void TestParticles::OnRender()
    {
        m_Particlesystem.RenderParticles(); ///< Render the particles using batch rendering.
    }

    void TestParticles::OnImGuiRender()
    {
        ImGui::Text("Particle count: %d", m_Particlesystem.GetParticleCount());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Mouse Clicked at: (%.3f,%.3f)", mousePos.x, mousePos.y);

        if (ImGui::Button("Create Particle"))
        {
            m_Particlesystem.CreateParticle(position, velocity, accelleration, mass, radius, color, m_Particlesystem.GetParticleCount() + 1);
        }

        ImGui::InputInt("Particle ID", &particleID);
        if (ImGui::Button("Remove Particle"))
        {
            m_Particlesystem.DestroyParticle(particleID);
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

