#version 430 core

struct Particle
{
    uint id;           // Matches m_ParticleID
    float radius;      // Matches m_Radius
    float mass;        // Matches m_Mass
    float _padding1;   // Ensure proper alignment (vec3 alignment adds padding)

    vec3 pos;          // Matches m_Position
    float _padding2;   // vec3 alignment padding
    vec3 vel;          // Matches m_Velocity
    float _padding3;   // vec3 alignment paddin
    vec3 acc;          // Matches m_Acceleration
    float _padding4;   // vec3 alignment padding

    vec3 p_pos;        // Matches m_PastPosition
    float _padding5;   // vec3 alignment padding
    vec3 p_vel;        // Matches m_PastVelocity
    float _padding6;   // vec3 alignment padding
    vec3 p_acc;        // Matches m_PastAcceleration
    float _padding7;   // vec3 alignment padding

    vec4 color;        // Matches m_ParticleColor
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
//    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime; 
    particles[i].vel = particles[i].acc * deltaTime + particles[i].vel;
}


//////////////////////////DEZE WERKT NOG NIET////////////////////////////

//#version 430 core
//
//struct Particle
//{
//    uint id;           // Matches m_ParticleID
//    float radius;      // Matches m_Radius
//    float mass;        // Matches m_Mass
//    float _padding1;   // Ensure proper alignment (vec3 alignment adds padding)
//
//    vec3 pos;          // Matches m_Position
//    float _padding2;   // vec3 alignment padding
//    vec3 vel;          // Matches m_Velocity
//    float _padding3;   // vec3 alignment paddin
//    vec3 acc;          // Matches m_Acceleration
//    float _padding4;   // vec3 alignment padding
//
//    vec3 p_pos;        // Matches m_PastPosition
//    float _padding5;   // vec3 alignment padding
//    vec3 p_vel;        // Matches m_PastVelocity
//    float _padding6;   // vec3 alignment padding
//    vec3 p_acc;        // Matches m_PastAcceleration
//    float _padding7;   // vec3 alignment padding
//
//    vec4 color;        // Matches m_ParticleColor
//};
//
//layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
//
//layout(std430, binding = 0) buffer DataBuffer 
//{
//    Particle particles[];
//};
//
//layout(std430, binding = 1) buffer IDBuffer
//{
//    uint activeIDs[];
//};
//
//uniform float deltaTime;
//
//void main() 
//{
//    uint i = gl_GlobalInvocationID.x;
//
//    if (i >= activeIDs.length())
//        return;
//
//    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime + ((particles[i].acc * deltaTime * deltaTime)/2) ;
////    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime; 
//    particles[i].vel = particles[i].acc * deltaTime + particles[i].vel;
//}