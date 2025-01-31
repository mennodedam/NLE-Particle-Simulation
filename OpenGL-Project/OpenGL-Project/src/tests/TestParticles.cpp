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

#define NUMBER 32

// temp values.
glm::vec3 position = { 400.0f, 300.0f, 0.0f };
glm::vec3 velocity = { 10.0f, 10.0f, 0.0f };
glm::vec3 accelleration = { 0.0f, -2.0f, 0.0f };  //{ 0.0f, -100.0f, 0.0f };
glm::vec4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
float mass = 7.0f;
float radius = 1.0f;

int particleID = 0;
int memorySize = 0;

ImVec2 mousePos;

bool flag = 0;
bool flag_m = 0;

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

        float r = radius;//1.0f;
        float CenterX = 0.0f;
        float CenterY = 0.0f;

        float positions[4 * (NUMBER + 1)];

        positions[0] = CenterX;         // x
        positions[1] = CenterY;         // y
        positions[2] = r / 2;//0.5f;            // u
        positions[3] = r / 2;//0.5f;            // v

        for (int i = 0; i < NUMBER; i++)
        {
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
        for (int i = 0; i < NUMBER; i++)
        {
            indices[i * 3 + 0] = 0;               // Center vertex
            indices[i * 3 + 1] = i + 1;           // Current edge vertex
            indices[i * 3 + 2] = (i + 1) % NUMBER + 1; // Next edge vertex
        }

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        //m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, (NUMBER + 1) * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        //m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * NUMBER);

        m_Shader->Bind();
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
    int nr = 0;

    void TestParticles::OnUpdate(float deltaTime)
    {
        if (m_Particlesystem.GetParticleCount() != 0)
        {
            m_ComputeShader->Update(m_Particlesystem, deltaTime);
            m_ComputeShader->RetrieveData(m_Particlesystem);  // alleen doen wanneer nodig, ipv bij elke dispatch van de computeshader
            m_TimeElapsed += deltaTime;
        }

        if (flag)
        {
            int freeindex = m_Particlesystem.CreateParticle(position, velocity, accelleration, mass, radius, color);
            m_ComputeShader->UploadData(m_Particlesystem);
            nr++;
        }
    }
    
    /**
     * @brief This method renders the TestParticles class.
     * 
     * @details
     * This method renders the TestParticles class.
     */
    void TestParticles::OnRender() 
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        {
            m_Shader->Bind();

            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);  

            m_Shader->SetUniformMat4f("view", view);
            m_Shader->SetUniformMat4f("projection", projection);

            //renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);   ///< *m_VAO en *m_IndexBuffer zijn placeholder.
            renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, m_Particlesystem.GetParticleCount());

            m_Shader->Unbind();
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
            
            ///< op dit moment overwrite dit de preallocated memory van initSSBO()
            m_ComputeShader->UploadData(m_Particlesystem);          

            ///< Werkt grotendeels, maar moet handmatig memory updated
            //unsigned int NewestID = m_Particlesystem.ReturnVectorSize()-1;
            //Particle newParticle = m_Particlesystem.ReturnParticle(NewestID); 
            //m_ComputeShader->UploadAddElement(m_Particlesystem, newParticle, NewestID);

            ///< Werkt niet
            //m_ComputeShader->UploadIDlist(m_Particlesystem.IDlistData());     
        }

        ImGui::InputInt("Particle ID", &particleID);
        if (ImGui::Button("Remove Particle"))
        {
            m_Particlesystem.DestroyParticle(particleID);
            m_ComputeShader->UploadData(m_Particlesystem);          ///< op dit moment overwrite dit de preallocated memory van initSSBO()
        }

        ImGui::Text("Mouse Clicked at: (%.3f,%.3f)", mousePos.x, mousePos.y);
        //if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            if (flag_m)
            {
                mousePos = ImGui::GetMousePos();

                ImGuiIO& io = ImGui::GetIO(); // Get ImGui's input/output structure
                float invertedY = io.DisplaySize.y - mousePos.y;

                glm::vec3 MouseClick = { mousePos.x, invertedY, 0.0 };

                int freeindex = m_Particlesystem.CreateParticle(MouseClick, velocity, accelleration, mass, radius, color);
                m_ComputeShader->UploadData(m_Particlesystem);
            }
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

        //ImGui::Text("Total Energy in system: %.3fJ", 999.999f); ///< method maken voor berekenen totale kinetische energie.

        if (ImGui::Button("Toggle Particles"))
        {
            if (!flag) { flag = 1; }
            else { flag = 0; }    
        }

        if (ImGui::Button("Toggle mouseclick Particles"))
        {
            if (!flag_m) { flag_m = 1; }
            else { flag_m = 0; }
        }


    }

}

