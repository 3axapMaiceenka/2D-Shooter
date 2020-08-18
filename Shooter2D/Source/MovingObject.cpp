#include "MovingObject.h"

MovingObject::MovingObject(const sf::Texture* const pTexture, const sf::Vector2f& pos)
	: position(pos),
	  pSprite(std::make_shared<sf::Sprite>(*pTexture)),
	  speedX(0.0f)
{ }

MovingObject::~MovingObject() = default;