#include "Level.h"
#include "Player.h"
#include "IO.h"
#include "GameInfo.h"

#include <algorithm>
#include <random>

Level::Level(IO* io_, std::shared_ptr<GameInfo> pGi)
	: pEnemies(new std::list<Enemy*>, &Level::enemiesDeleter),
	  pEnemiesFactory(std::make_unique<EnemiesFactory>()),
	  pShots(std::make_unique<std::list<Shot>>()),
	  pWave(std::make_unique<Wave>()),
	  pHitEffects(std::make_unique<std::list<ParticleSystem>>()),
	  pGameInfo(pGi),
	  io(io_)
{ }

bool Level::update(float time)
{
	generateEnemies();

	updateHitEffects(time);
	updateShots(time);

	return updateEnemies(time);
}

bool Level::updateEnemies(float time)
{
	removeKilledEnemies();

	return !std::any_of(pEnemies->cbegin(), pEnemies->cend(), [this, time](Enemy* pEnemy) 
	{
		pEnemy->move(time);

		auto itShot = std::find_if(pShots->begin(), pShots->end(), [this, pEnemy](const Shot& shot) { return pEnemy->hit(shot); });
		if (itShot != pShots->end())
		{
			itShot->getPlayer()->increaseMoney(pEnemy->moneyForHit());
			pHitEffects->emplace_back(itShot->getPosition(), 1000);
			pShots->erase(itShot);
		}

		return pEnemy->getX() - pEnemy->width() >= GameBackground::RightBound; 
	});
}

void Level::removeKilledEnemies()
{
	pEnemies->erase(std::remove_if(pEnemies->begin(), pEnemies->end(), [this](Enemy* pEnemy)
	{
		if (pEnemy->isDead())
		{
			delete pEnemy;
			pWave->enemiesKilled++;
			pWave->currentEnemiesCount--;
			pGameInfo->incEnemiesKilled(static_cast<float>(pWave->waveOverallEnemiesCount) / static_cast<float>(pWave->enemiesKilled));
			return true;
		}

		return false;
	}), pEnemies->end());
}

void Level::updateShots(float time)
{
	pShots->erase(std::remove_if(pShots->begin(), pShots->end(), [time](Shot& shot)
	{
		shot.move(time);
		return shot.getX() - shot.width() <= 0;
	}), pShots->end());
}

void Level::updateHitEffects(float time)
{
	pHitEffects->erase(std::remove_if(pHitEffects->begin(), pHitEffects->end(), [time](ParticleSystem& ps)
	{
		return !ps.update(time);
	}), pHitEffects->end());
}

void Level::addShot(const sf::Vector2f& position, Player* pPlayer, unsigned char damage)
{
	pShots->emplace_back(position, pPlayer, damage);
}

void Level::draw() const
{
	std::for_each(pShots->cbegin(), pShots->cend(), [this](const Shot& shot) { io->draw(shot.getSprite()); });
	std::for_each(pEnemies->cbegin(), pEnemies->cend(), [this](const Enemy* pEnemy) { io->draw(pEnemy->getSprite()); });
	std::for_each(pHitEffects->cbegin(), pHitEffects->cend(), [this](const ParticleSystem& ps) { io->draw(&ps); });
}

void Level::enemiesDeleter(std::list<Enemy*>* pEnemies)
{
	std::for_each(pEnemies->cbegin(), pEnemies->cend(), [](Enemy* pEnemy) { delete pEnemy; });
	delete pEnemies;
}

void Level::generateEnemies()
{
	if (pWave->enemiesKilled == pWave->waveOverallEnemiesCount)
	{
		if (!(++pWave->wave % 5))
		{
			pWave->minEnemiesCount++;
			pEnemiesFactory->increseAllowedEnemiesTypes();
		}

		pWave->enemiesKilled = 0;
		pWave->waveOverallEnemiesCount++;
		pGameInfo->incWave();

		pWave->generationDelay -= pWave->GenerationDelayDec;
		if (pWave->generationDelay < pWave->GenerationDelayLimit)
		{
			pWave->generationDelay = pWave->GenerationDelayLimit;
		}
	}
	
	if ((pWave->clock.getElapsedTime().asMilliseconds() >= pWave->generationDelay &&
		pWave->currentEnemiesCount + pWave->enemiesKilled < pWave->waveOverallEnemiesCount) ||
		(pWave->currentEnemiesCount < pWave->minEnemiesCount &&
	    pWave->waveOverallEnemiesCount - pWave->enemiesKilled - pWave->currentEnemiesCount > 0))
	{
		pEnemies->emplace_back(pEnemiesFactory->createEnemy());
		pWave->currentEnemiesCount++;
		pWave->clock.restart();
	}
}

Level::EnemiesFactory::EnemiesFactory()
	: enemiesSamples(),
	  enemiesTypesAllowed(0)
{
	enemiesSamples[0] = std::make_unique<WeakEnemy>(sf::Vector2f(0.0f, 0.0f));
	enemiesSamples[1] = std::make_unique<DiagonalMovingEnemy>(sf::Vector2f(0.0f, 0.0f));
	enemiesSamples[2] = std::make_unique<JumpingEnemy>(sf::Vector2f(0.0f, 0.0f));
	enemiesSamples[MaxEnemiesTypes] = std::make_unique<StrongEnemy>(sf::Vector2f(0.0f, 0.0f));
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
			pEnemy = new DiagonalMovingEnemy({ GameBackground::LeftBound - static_cast<float>(enemiesSamples[enemyType]->width()), getRandomYCoordinate(enemyType) });
		} break;

		case 2:
		{
			pEnemy = new JumpingEnemy({ GameBackground::LeftBound - static_cast<float>(enemiesSamples[enemyType]->width()), getRandomYCoordinate(enemyType) });
		} break;

		case MaxEnemiesTypes:
		{
			pEnemy = new StrongEnemy({ GameBackground::LeftBound - static_cast<float>(enemiesSamples[enemyType]->width()), getRandomYCoordinate(enemyType) });
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
	float enemyHeight = static_cast<float>(enemiesSamples[enemyType]->height());

	std::uniform_real_distribution<float> dist(GameBackground::UpperBound - enemyHeight, GameBackground::LowerBound - enemyHeight);

	return dist(generator);
}

unsigned char Level::EnemiesFactory::getRandomEnemyType()
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, enemiesTypesAllowed);

	return dist(generator);
}
