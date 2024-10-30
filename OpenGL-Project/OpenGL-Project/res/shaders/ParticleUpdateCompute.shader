#shader compute
#version 330 core

layout (local_size_x = 1024) in;

struct Particle {
    glm::vec4 Position;
    glm::vec4 Velocity;
    glm::vec4 Acceleration;
}

uniform float u_DeltaTime;

void main()
{

}


