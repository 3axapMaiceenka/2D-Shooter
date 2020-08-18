#pragma once

#include "MovingObject.h"

class AnimatedObject : public MovingObject
{
public:
	AnimatedObject(const sf::Texture* const pTexture, const sf::Vector2f& pos);

protected:
	virtual float maxFrame() const = 0;
	virtual void  changeFrame(float time);
	virtual float frameChangeSpeed() const = 0;
	
	void changeFrameImpl(float time);

protected:
	sf::Vector2i textureCoord;
	float currentFrame;
};
