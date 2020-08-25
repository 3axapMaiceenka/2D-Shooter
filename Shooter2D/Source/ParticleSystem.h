#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
	ParticleSystem(const sf::Vector2f& position, std::size_t particlesCount);
	ParticleSystem(const ParticleSystem& rhs);
	ParticleSystem(ParticleSystem&& rhs) noexcept;
	ParticleSystem& operator=(const ParticleSystem& rhs);
	ParticleSystem& operator=(ParticleSystem&& rhs) noexcept;

	bool update(float time);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	struct Particle
	{
		sf::Vector2f velocity;
		float lifeTime;
	};

	sf::VertexArray vertices;
	std::vector<Particle> particles;
	const float Time = 3000;
};
