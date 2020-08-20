#pragma once

#include "Shot.h"

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

private:
	static void enemiesDeleter(std::list<Enemy*>* pEnemies);

private:
	std::unique_ptr<std::list<Enemy*>, decltype(&Level::enemiesDeleter)> pEnemies;
	std::unique_ptr<std::list<Shot>> pShots;
	IO* io;
};
