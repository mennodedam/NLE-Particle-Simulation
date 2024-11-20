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