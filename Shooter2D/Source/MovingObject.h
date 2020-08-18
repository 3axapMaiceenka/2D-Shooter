#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

class MovingObject
{
public:
	MovingObject(const sf::Texture* const pTexture, const sf::Vector2f& pos);
	virtual ~MovingObject();

	virtual int width() const = 0;
	virtual int height() const = 0;
	virtual void move(float time) = 0;

	const auto getSprite() const { return pSprite.get(); }
	const sf::Vector2i size() const { return { width(), height() }; }

protected:
	virtual float speed() const = 0;

protected:
	sf::Vector2f position;
	std::shared_ptr<sf::Sprite> pSprite;
	float speedX;
};