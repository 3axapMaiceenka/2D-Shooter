#include "ParticleSystem.h"

#include <vector>

ParticleSystem::ParticleSystem(const sf::Vector2f& position, std::size_t particlesCount)
	: sf::Drawable(), sf::Transformable(),
	  vertices(sf::Points, particlesCount),
	  particles(particlesCount)
{
	for (std::size_t i = 0; i < particlesCount; i++)
	{
		float angle = (std::rand() % 360) * 3.14f / 180.f;
		float speed = (std::rand() % 50) + 50.f;

		vertices[i].position = position;
		particles[i].lifeTime = static_cast<float>((std::rand() % 2000) + 1000);
		//particles[i].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		particles[i].velocity = sf::Vector2f(-0.5f, -0.5f);
	}
}

ParticleSystem::ParticleSystem(const ParticleSystem& rhs)
	: vertices(rhs.vertices),
	  particles(rhs.particles)
{ }

ParticleSystem::ParticleSystem(ParticleSystem&& rhs) noexcept
	: vertices(std::move(rhs.vertices)),
	  particles(std::move(rhs.particles))
{ }

ParticleSystem& ParticleSystem::operator=(const ParticleSystem& rhs)
{
	if (this != &rhs)
	{
		vertices = rhs.vertices;
		particles = rhs.particles;
	}

	return *this;
}

ParticleSystem& ParticleSystem::operator=(ParticleSystem&& rhs) noexcept
{
	if (this != &rhs)
	{
		vertices = std::move(rhs.vertices);
		particles = std::move(rhs.particles);
	}

	return *this;
}

bool ParticleSystem::update(float time)
{
	bool aliveParticles = false;

	for (std::size_t i = 0; i < particles.size(); i++)
	{
		Particle& particle = particles[i];
		sf::Vertex& vertex = vertices[i];

		particle.lifeTime -= time;
		if (particle.lifeTime > 0)
		{
			aliveParticles = true;
		}
		else
		{
			continue;
		}

		float ratio = particle.lifeTime / Time;

		vertex.position += particles[i].velocity * time;
		vertex.color = sf::Color(0, 0, static_cast<sf::Uint8>(ratio * 255.0f));
	}

	return aliveParticles;
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(vertices, states);
}