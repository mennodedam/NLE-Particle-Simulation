#version 430

struct Particle 
{
    vec3 position; 
    vec3 velocity; 
    vec3 acceleration; 
    float radius;
    float mass;  
    vec4 color;
    uint id;
};

///< Binding point 0 for particle SSBO
layout(std430, binding = 0) buffer ParticleBuffer 
{
    Particle particles[]; 
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float deltaTime;

void main()
{
    uint idx = gl_GlobalInvocationID.x;

    Particle particle = particles[idx];
    particle.velocity += particle.acceleration * deltaTime;
    particle.position += particle.velocity * deltaTime;
    particles[idx] = particle;
}


