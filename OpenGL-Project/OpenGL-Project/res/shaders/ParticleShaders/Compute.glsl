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

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer DataBuffer 
{
    Particle particles[];
};

uniform float deltaTime;

// 960 x 540
vec3 screenMin = {-0.5, -0.5, 0.0};  // Minimum screen bounds (e.g., {0.0, 0.0, 0.0})
vec3 screenMax = {800.0, 600.0, 0.0};

//vec3 screenMin = {-100.0, -100.0, 0.0};
//vec3 screenMax = {600.0, 400.0, 0.0};

//uniform vec3 screenMin;  // Minimum screen bounds (e.g., {0.0, 0.0, 0.0})
//uniform vec3 screenMax;

float frictionW = 0.95;
float frictionP = 0.96;

void Update(uint i)
{
    particles[i].pos = particles[i].pos + particles[i].vel * deltaTime + ((particles[i].acc * deltaTime * deltaTime)/2);
    particles[i].vel = particles[i].acc * deltaTime + particles[i].vel;
}

void CheckCollisionWall(uint i)
{
    if (particles[i].pos.x - particles[i].radius < screenMin.x || particles[i].pos.x + particles[i].radius > screenMax.x) 
    {
        particles[i].vel.x = -particles[i].vel.x * frictionW; 
        particles[i].pos.x = clamp(particles[i].pos.x, screenMin.x + particles[i].radius, screenMax.x - particles[i].radius);
    }

    if (particles[i].pos.y - particles[i].radius < screenMin.y || particles[i].pos.y + particles[i].radius > screenMax.y) 
    {
        particles[i].vel.y = -particles[i].vel.y * frictionW; 
        particles[i].pos.y = clamp(particles[i].pos.y, screenMin.y + particles[i].radius, screenMax.y - particles[i].radius);
    }

    if (particles[i].pos.z - particles[i].radius < screenMin.z || particles[i].pos.z + particles[i].radius > screenMax.z) 
    {
        particles[i].vel.z = -particles[i].vel.z * frictionW; 
        particles[i].pos.z = clamp(particles[i].pos.z, screenMin.z + particles[i].radius, screenMax.z - particles[i].radius);
    }
}

void CheckCollionParticlesSimple(uint i)
{
    for (uint j = 0; j < particles.length(); ++j)
    {
        if (i != j) 
        {
            vec3 diff = particles[i].pos - particles[j].pos;
            float distance = length(diff);
            float collisionDistance = particles[i].radius + particles[j].radius;

            if (distance < collisionDistance) 
            {
                // Resolve collision: invert velocities
                vec3 normal = normalize(diff);
                particles[i].vel = reflect(particles[i].vel, normal) * frictionP;
                particles[j].vel = reflect(particles[j].vel, -normal) * frictionP;

                // Separate particles to avoid sticking
                float overlap = 0.5 * (collisionDistance - distance);
                particles[i].pos += normal * overlap;
                particles[j].pos -= normal * overlap;
            }
        }
    }
}

void main() 
{
    uint i = gl_GlobalInvocationID.x;

    Update(i);
    CheckCollisionWall(i);
    CheckCollionParticlesSimple(i);
}