#include "Level.h"
#include "Player.h"
#include "IO.h"

#include <algorithm>

Level::Level(IO* io_)
	: pShots(std::make_unique<std::list<Shot>>()),
	  io(io_)
{ }

void Level::update(float time)
{
	pShots->erase(std::remove_if(pShots->begin(), pShots->end(), [time](Shot& shot)
	{
		shot.move(time);
		return shot.getX() - shot.width() <= 0;
	}), pShots->end());
}

void Level::addShot(const sf::Vector2f& position, Player* pPlayer)
{
	pShots->emplace_back(position, pPlayer);
}

void Level::draw() const
{
	std::for_each(pShots->cbegin(), pShots->cend(), [this](const Shot& shot) { io->draw(shot.getSprite()); });
}
