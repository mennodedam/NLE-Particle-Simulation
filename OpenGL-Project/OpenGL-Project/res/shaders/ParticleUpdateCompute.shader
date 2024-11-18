#version 430

///< Particle struct. Mirrors CPU-side particle class.
//struct Particle {
  //  vec3 position;
  //  vec3 velocity;
  //  vec3 acceleration;
  // float mass;
  //  float radius;
  //  vec4 color;
  //  uint id;
//};

///< Binding point 0 for particle SSBO
//layout(std430, binding = 0) buffer Particles {
//    Particle particles[];
//};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float deltaTime;

void main()
{
    //uint index = gl_GlobalInvocationID.x;

    ///< Access the current particle in the SSBO
    //Particle p = particles[index];

    ///< Update velocity based on acceleration
    //p.velocity += p.acceleration * deltaTime;

    ///< Update position based on velocity
    //p.position += p.velocity * deltaTime;

    ///< Write updated particle data back to the SSBO
    //particles[index] = p;
}


