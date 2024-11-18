#version 430

struct Particle
{
    vec3 pos; 
    vec3 vel;
    vec3 acc;
    float mass, radius;
    vec4 color;
    unsigned int id;
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


///< DEZE CODE WERKT.
/*

#version 430

class Particle
{
    vec3 pos; 
    vec3 vel;
    vec3 acc;
    float mass, radius;
    vec4 color;
    unsigned int id;
};

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in; // Specify workgroup size

layout(std430, binding = 0) buffer DataBuffer {
    float data[];
};

void main() {
    uint index = gl_GlobalInvocationID.x;
    data[index] *= 2.0; // Example operation
}


*/