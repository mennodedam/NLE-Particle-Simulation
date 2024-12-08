#version 430 core

struct Particle
{
    uint id;           
    float radius;      
    float mass;        
    float _padding1;

    vec3 pos;          
    float _padding2;   
    vec3 vel;          
    float _padding3;   
    vec3 acc;          
    float _padding4;   

    vec3 p_pos;        
    float _padding5;   
    vec3 p_vel;        
    float _padding6;   
    vec3 p_acc;        
    float _padding7;   

    vec4 color;        
};

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer DataBuffer 
{
    Particle particles[];
};

uniform float deltaTime;

vec3 screenMin = {0.0, 0.0, 0.0};  // Minimum screen bounds (e.g., {0.0, 0.0, 0.0})
vec3 screenMax = {800.0, 600.0, 0.0};

//uniform vec3 screenMin;  // Minimum screen bounds (e.g., {0.0, 0.0, 0.0})
//uniform vec3 screenMax;

void main() 
{
    uint i = gl_GlobalInvocationID.x;

    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime + ((particles[i].acc * deltaTime * deltaTime)/2) ;
    particles[i].vel = particles[i].acc * deltaTime + particles[i].vel;

    if (particles[i].pos.x - particles[i].radius < screenMin.x || particles[i].pos.x + particles[i].radius > screenMax.x) 
    {
        particles[i].vel.x = -particles[i].vel.x; 
        particles[i].pos.x = clamp(particles[i].pos.x, screenMin.x + particles[i].radius, screenMax.x - particles[i].radius);
    }

    if (particles[i].pos.y - particles[i].radius < screenMin.y || particles[i].pos.y + particles[i].radius > screenMax.y) 
    {
        particles[i].vel.y = -particles[i].vel.y; 
        particles[i].pos.y = clamp(particles[i].pos.y, screenMin.y + particles[i].radius, screenMax.y - particles[i].radius);
    }

//    if (particles[i].pos.z - particles[i].radius < screenMin.z || particles[i].pos.z + particles[i].radius > screenMax.z) 
//    {
//        particles[i].vel.z = -particles[i].vel.z; // Invert Z velocity
//        // Correct position to prevent sticking
//        particles[i].pos.z = clamp(particles[i].pos.z, screenMin.z + particles[i].radius, screenMax.z - particles[i].radius);
//    }
}