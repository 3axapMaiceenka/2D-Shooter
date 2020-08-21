#include "Level.h"
#include "Player.h"
#include "IO.h"

#include <algorithm>
#include <random>

Level::Level(IO* io_)
	: pEnemies(new std::list<Enemy*>, &Level::enemiesDeleter),
	  pEnemiesFactory(std::make_unique<EnemiesFactory>()),
	  pShots(std::make_unique<std::list<Shot>>()),
	  io(io_)
{ 
	// temp
	for (int i = 0; i < 5; i++)
	{
		pEnemies->emplace_back(pEnemiesFactory->createEnemy());
	}
}

bool Level::update(float time)
{
	updateShots(time);
	return updateEnemies(time);
}

bool Level::updateEnemies(float time)
{
	pEnemies->erase(std::remove_if(pEnemies->begin(), pEnemies->end(), [](Enemy* pEnemy) 
	{ 
		if (pEnemy->isDead())
		{
			delete pEnemy;
			return true;
		}

		return false;
	}), pEnemies->end());

	return !std::any_of(pEnemies->cbegin(), pEnemies->cend(), [this, time](Enemy* pEnemy) 
	{
		pEnemy->move(time);

		auto itShot = std::find_if(pShots->begin(), pShots->end(), [pEnemy](const Shot& shot) { return pEnemy->hit(shot); });
		if (itShot != pShots->end())
		{
			pShots->erase(itShot);
		}

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

Level::EnemiesFactory::EnemiesFactory()
	: enemiesSamples(),
	  enemiesTypesAllowed(0)
{
	enemiesSamples[0] = std::make_unique<WeakEnemy>(sf::Vector2f(0.0f, 0.0f));
	//enemiesSamples[1] = std::make_unique<DiagonalMovingEnemy>(sf::Vector2f(0.0f, 0.0f));
	//...
}

Enemy* Level::EnemiesFactory::createEnemy()
{
	Enemy* pEnemy = nullptr;
	auto enemyType = getRandomEnemyType();

	switch (enemyType)
	{
		case 0:
		{
			pEnemy = new WeakEnemy({ GameBackground::LeftBound - static_cast<float>(enemiesSamples[enemyType]->width()), getRandomYCoordinate(enemyType) });
		} break;

		case 1:
		{
			static_assert("EnemiesFactory::createEnemy(). Case 1. Not implemented yet");
		} break;

		case 2:
		{
			static_assert("EnemiesFactory::createEnemy(). Case 2. Not implemented yet");
		} break;

		case MaxEnemiesTypes:
		{
			static_assert("EnemiesFactory::createEnemy(). Case 3. Not implemented yet");
		} break;
	}

	return pEnemy;
}

void Level::EnemiesFactory::increseAllowedEnemiesTypes()
{
	if (++enemiesTypesAllowed > MaxEnemiesTypes)
	{
		enemiesTypesAllowed = MaxEnemiesTypes;
	}
}

float Level::EnemiesFactory::getRandomYCoordinate(unsigned char enemyType)
{
	std::random_device rd;
	std::mt19937 generator(rd());

	std::uniform_real_distribution<float> dist
	(
		GameBackground::UpperBound - static_cast<float>(enemiesSamples[enemyType]->height()), 
		GameBackground::LowerBound - static_cast<float>(enemiesSamples[enemyType]->height())
	);

	return dist(generator);
}

unsigned char Level::EnemiesFactory::getRandomEnemyType()
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, enemiesTypesAllowed);

	return dist(generator);
}
