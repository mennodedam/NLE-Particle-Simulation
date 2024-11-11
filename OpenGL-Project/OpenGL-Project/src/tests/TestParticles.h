#pragma once

#include <iostream>

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "Particle.h"
#include "Particlesystem.h"

#include <memory>

namespace test {

	class TestParticles : public Test
	{
	public:
		TestParticles();
		~TestParticles();

		void OnUpdate(float delatTime) override;
		void OnRender() override;
		void OnImGuiRender() override;	 
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		ParticleSystem m_Particlesystem;
	};

}