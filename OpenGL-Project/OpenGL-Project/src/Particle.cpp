#include "Particle.h"

Particle::Particle
(
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
    glm::vec3 vel = { 0.0f, 0.0f, 0.0f },
    glm::vec3 acc = { 0.0f, 0.0f, 0.0f },
    float m = 1.0f,
    float r = 1.0f,
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
    unsigned int id = 0
)
    : m_Position(pos), m_Velocity(vel), m_Acceleration(acc),
    m_Mass(m), m_Radius(r),
    m_PastPosition({ 0.0f, 0.0f, 0.0f }), m_PastVelocity({ 0.0f, 0.0f, 0.0f }), m_PastAcceleration({ 0.0f, 0.0f, 0.0f }),
    m_ParticleID(id),
    m_ParticleColor(color)
{
}

Particle::~Particle()
{

}