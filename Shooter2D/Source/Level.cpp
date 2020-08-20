#include "Level.h"
#include "Player.h"
#include "IO.h"
#include "Enemies.h"

#include <algorithm>

Level::Level(IO* io_)
	: pEnemies(new std::list<Enemy*>, &Level::enemiesDeleter),
	  pShots(std::make_unique<std::list<Shot>>()),
	  io(io_)
{ 
	// temp
	for (int i = 0; i < 5; i++)
	{
		pEnemies->emplace_back(EnemiesFactory::createEnemy());
	}
}

bool Level::update(float time)
{
	updateShots(time);
	return updateEnemies(time);
}

bool Level::updateEnemies(float time)
{
	std::for_each(pEnemies->cbegin(), pEnemies->cend(), [time](Enemy* pEnemy) { pEnemy->move(time); });

	// temporary
	return !std::any_of(pEnemies->cbegin(), pEnemies->cend(), [](const Enemy* pEnemy) 
	{ 
		return pEnemy->getX() + pEnemy->width() >= GameBackground::RightBound; 
	});
}

void Level::updateShots(float time)
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
	std::for_each(pEnemies->cbegin(), pEnemies->cend(), [this](const Enemy* pEnemy) { io->draw(pEnemy->getSprite()); });
}

void Level::enemiesDeleter(std::list<Enemy*>* pEnemies)
{
	std::for_each(pEnemies->cbegin(), pEnemies->cend(), [](Enemy* pEnemy) { delete pEnemy; });
	delete pEnemies;
}
