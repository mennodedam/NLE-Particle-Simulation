#version 430

struct Particle
{
    unsigned int id;
    float mass, radius;

    vec3 pos; 
    vec3 vel;
    vec3 acc;

    vec3 p_pos;
    vec3 p_vel;
    vec3 p_acc;

    vec4 color;
};

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer DataBuffer {
    Particle particles[];
};

uniform float deltaTime;

void main() 
{
    uint i = gl_GlobalInvocationID.x;
    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime + ((particles[i].acc * deltaTime * deltaTime)/2) ; 
    particles[i].vel = particles[i].acc * deltaTime + particles[i].vel;
}