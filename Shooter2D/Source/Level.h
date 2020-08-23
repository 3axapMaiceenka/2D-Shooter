#pragma once

#include "Shot.h"
#include "Enemies.h"

#include <array>
#include <list>

class IO;

class Level
{
public:
	Level(IO* io_);

	void addShot(const sf::Vector2f& position, Player* pPlayer);
	bool update(float time);
	void draw() const;

private:
	void generateEnemies();
	void removeKilledEnemies();
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

	struct Wave
	{
		sf::Clock clock;
		const sf::Int32 generationDelayDec = 100;
		const sf::Int32 generationDelayLimit = 2000;
		sf::Int32 generationDelay = 5000;
		unsigned short wave = 0;
		unsigned short enemiesKilled = 0;
		unsigned short minEnemiesCount = 4;
		unsigned short currentEnemiesCount = 0;
		unsigned short waveOverallEnemiesCount = 8;
	};

private:
	std::unique_ptr<std::list<Enemy*>, decltype(&Level::enemiesDeleter)> pEnemies;
	std::unique_ptr<EnemiesFactory> pEnemiesFactory;
	std::unique_ptr<std::list<Shot>> pShots;
	std::unique_ptr<Wave> pWave;
	IO* io;
};
