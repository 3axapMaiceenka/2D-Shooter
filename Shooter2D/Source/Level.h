#pragma once

#include "Shot.h"

#include <list>

class IO;

class Level
{
public:
	Level(IO* io_);

	void update(float time);
	void addShot(const sf::Vector2f& position, Player* pPlayer);
	void draw() const;

private:
	std::unique_ptr<std::list<Shot>> pShots;
	IO* io;
};
