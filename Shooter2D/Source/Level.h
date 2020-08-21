#pragma once

#include "Shot.h"
#include "Enemies.h"

#include <array>
#include <list>

class IO;
class Enemy;

class Level
{
public:
	Level(IO* io_);

	void addShot(const sf::Vector2f& position, Player* pPlayer);
	bool update(float time);
	void draw() const;

private:
	void updateShots(float time);
	bool updateEnemies(float time);

	static void enemiesDeleter(std::list<Enemy*>* pEnemies);

private:
	class EnemiesFactory
	{
	public:
		EnemiesFactory();

		Enemy* createEnemy();
		void increseAllowedEnemiesTypes();

	private:
		float getRandomYCoordinate(unsigned char enemyType);
		unsigned char getRandomEnemyType();

	private:
		static constexpr unsigned char MaxEnemiesTypes = 3;
		std::array<std::unique_ptr<Enemy>, MaxEnemiesTypes + 1> enemiesSamples;
		unsigned char enemiesTypesAllowed;
	};

private:
	std::unique_ptr<std::list<Enemy*>, decltype(&Level::enemiesDeleter)> pEnemies;
	std::unique_ptr<EnemiesFactory> pEnemiesFactory;
	std::unique_ptr<std::list<Shot>> pShots;
	IO* io;
};
