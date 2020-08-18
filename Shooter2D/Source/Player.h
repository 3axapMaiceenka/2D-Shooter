#pragma once

#include "AnimatedObject.h"

#include <SFML/Graphics.hpp>

class IO;
struct MappingKeysToControls;

enum class PlayerControls
{
	UP,
	DOWN,
	LEFT, 
	RIGHT,
	SHOOT
};

class Player : public AnimatedObject
{
public:
	Player(IO* io_, const sf::Vector2f& pos, std::shared_ptr<MappingKeysToControls> pMappingKeysToControls);

	virtual int width()  const override;
	virtual int height() const override;
	virtual void move(float time) override;

protected:
	virtual float speed() const override;
	virtual float maxFrame() const override;
	virtual void  changeFrame(float time) override;
	virtual float frameChangeSpeed() const override;

private:
	void update(float time);

private:
	std::shared_ptr<MappingKeysToControls> pMapping;
	IO* io;
	float speedY;
	float textureRow;
};