#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(const sf::Texture* const pTexture, const sf::Vector2f& pos)
	: MovingObject(pTexture, pos),
	  textureCoord(0, 0),
	  currentFrame(0.0f)
{ }

void AnimatedObject::changeFrame(float time)
{
	changeFrameImpl(time);
}

void AnimatedObject::changeFrameImpl(float time)
{
	currentFrame += frameChangeSpeed() * time;

	if (currentFrame >= maxFrame())
	{
		currentFrame = 0.0f;
	}

	textureCoord.x = static_cast<int>(currentFrame) * width();

	pSprite->setTextureRect({ textureCoord.x, textureCoord.y, width(), height() });
}
