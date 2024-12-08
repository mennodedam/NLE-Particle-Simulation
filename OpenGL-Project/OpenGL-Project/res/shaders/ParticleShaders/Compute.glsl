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


void main() 
{
    uint i = gl_GlobalInvocationID.x;
    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime + ((particles[i].acc * deltaTime * deltaTime)/2) ;
    particles[i].vel = particles[i].acc * deltaTime + particles[i].vel;
}