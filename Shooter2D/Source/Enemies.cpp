#include "Enemies.h"
#include "Shot.h"
#include "Game.h"
#include "IO.h"

#include <iostream>
#include <random>

unsigned char EnemiesFactory::enemiesTypesAllowed = 0;

Enemy::Enemy(const sf::Texture* const pTexture, const sf::Vector2f& pos, unsigned char health)
	: AnimatedObject(pTexture, pos),
	  hp(health),
	  alive(true)
{ }

bool Enemy::hit(const Shot& shot)
{
	if (sf::Rect<float>(position.x, position.y, static_cast<float>(size().x), static_cast<float>(size().y)).intersects(
		{ shot.getX(), shot.getY(), static_cast<float>(shot.size().x), static_cast<float>(shot.size().y) } ))
	{
		if (!(--hp))
		{
			alive = false;
		}

		return true;
	}

	return false;
}

void Enemy::move(float time)
{
	changeFrame(time);

	position.x += speedX * time;
	pSprite->setPosition(position);
}

WeakEnemy::WeakEnemy(const sf::Vector2f& pos)
	: Enemy(Game::getTexture("Resources/Images/enemie1.png"), pos, 3)
{ 
	pSprite->setTextureRect(sf::IntRect(0, 0, width(), height()));
	speedX = speed();
}

float WeakEnemy::maxFrame() const
{
	if (alive) return 4.0f;
	return 10.0f;
}

Enemy* EnemiesFactory::createEnemy()
{
	Enemy* pEnemy = nullptr;
	auto enemyType = getRandomEnemyType();

	switch (enemyType)
	{
		case 0:
		{
			pEnemy = new WeakEnemy( { GameBackground::LeftBound, getRandomCoordinate() } );
		} break;

		case 1:
		{
			static_assert("EnemiesFactory::createEnemy(). Case 1. Not implemented yet");
			//pEnemy = new DiagonalMovingEnemy( { GameBackground::LeftBound, getRandomCoordinate() } );
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

void EnemiesFactory::increseAllowedEnemiesTypes()
{
	if (++enemiesTypesAllowed > MaxEnemiesTypes)
	{
		enemiesTypesAllowed = MaxEnemiesTypes;
	}
}

float EnemiesFactory::getRandomCoordinate()
{
	std::random_device rd;
    std::mt19937 generator(rd());
	std::uniform_real_distribution<float> dist(GameBackground::UpperBound, GameBackground::LowerBound);

	return dist(generator);
}

unsigned char EnemiesFactory::getRandomEnemyType()
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, enemiesTypesAllowed);

	return dist(generator);
}

int WeakEnemy::width()                 const { return 27; }
int WeakEnemy::height()                const { return 42; }
unsigned char WeakEnemy::moneyForHit() const { return 20; }
float WeakEnemy::speed()               const { return 0.04f; }
float WeakEnemy::frameChangeSpeed()    const { return 0.015f; }

