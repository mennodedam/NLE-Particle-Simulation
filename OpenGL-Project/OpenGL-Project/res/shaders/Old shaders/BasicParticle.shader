#shader vertex
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

layout(std430, binding = 0) buffer DataBuffer 
{
    Particle particles[];
};

layout(location = 0) in vec3 quadVertex;

uniform mat4 projection;
uniform mat4 view;

out vec4 FragmentColor;

void main()
{
    uint particleIndex = gl_InstanceID; // Instance ID determines which particle to use
    vec3 worldPosition = particles[particleIndex].pos + quadVertex * particles[particleIndex].radius;
    FragmentColor = particles[particleIndex].color; // Pass particle color to fragment shader
    gl_Position = projection * view * vec4(worldPosition, 1.0);
}


#shader fragment
#version 430 core

in vec4 FragmentColor;
out vec4 color;

void main()
{
    color = FragmentColor;
}