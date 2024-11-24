#include "TestCircle.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define NUMBER 32

namespace test {

    TestCircle::TestCircle()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_Translation(200, 200, 0),
        m_CircleColor{ 0.2f, 0.3f, 0.8f, 1.0f }
    {
        float r = 50.0f;
        float CenterX = 0.0f;
        float CenterY = 0.0f;

        float positions[4 * (NUMBER + 1)];

        positions[0] = CenterX;         // x
        positions[1] = CenterY;         // y
        positions[2] = 0.5f;            // u
        positions[3] = 0.5f;            // v

        for (int i = 0; i < NUMBER; i++) {
            float angle = i * (2.0f * glm::pi<float>() / NUMBER); // Angle for this vertex
            float x = CenterX + r * cos(angle); // x position
            float y = CenterY + r * sin(angle); // y position

            // Set the position and texture coordinates for each vertex
            positions[(i + 1) * 4 + 0] = x;           // x
            positions[(i + 1) * 4 + 1] = y;           // y
            positions[(i + 1) * 4 + 2] = (cos(angle) + 1.0f) * 0.5f; // u
            positions[(i + 1) * 4 + 3] = (sin(angle) + 1.0f) * 0.5f; // v
        }

        unsigned int indices[3 * NUMBER];
        for (int i = 0; i < NUMBER; i++) {
            indices[i * 3 + 0] = 0;               // Center vertex
            indices[i * 3 + 1] = i + 1;           // Current edge vertex
            indices[i * 3 + 2] = (i + 1) % NUMBER + 1; // Next edge vertex
        }

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, (NUMBER + 1) * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * NUMBER);

        m_Shader = std::make_unique<Shader>("res/shaders/Circle/Vertex.glsl", "res/shaders/Circle/Fragment.glsl");

        m_Shader->Bind();

        m_Shader->SetUniform4f("u_Color", m_CircleColor[0], m_CircleColor[1], m_CircleColor[2], m_CircleColor[3]);
    }


    TestCircle::~TestCircle()
    {
    }


    void TestCircle::OnUpdate(float deltaTime)
    {
    }

    void TestCircle::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        m_Shader->SetUniform4f("u_Color", m_CircleColor[0], m_CircleColor[1], m_CircleColor[2], m_CircleColor[3]);
    }

    void TestCircle::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_CircleColor);
        ImGui::SliderFloat3("Translation A", &m_Translation.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}
