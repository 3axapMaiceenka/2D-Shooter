#pragma once

#include "AnimatedObject.h"

#include <SFML/System/Clock.hpp>

class Shot;

class Enemy : public AnimatedObject
{
public:
	Enemy(const sf::Texture* const pTexture, const sf::Vector2f& pos, unsigned char health);

	virtual unsigned char moneyForHit() const = 0;
	virtual void move(float time) override;
	virtual bool hit(const Shot& shot);

	bool isAlive() const { return alive; }

protected:
	unsigned char hp;
	bool alive;
};

class WeakEnemy : public Enemy
{
public:
	WeakEnemy(const sf::Vector2f& pos);

	virtual int width() const override;;
	virtual int height() const override;
	virtual unsigned char moneyForHit() const override;

protected:
	virtual float speed() const override;
	virtual float maxFrame() const override;
	virtual float frameChangeSpeed() const override;
};

/*class DiagonalMovingEnemy : public Enemy
{
public:
	DiagonalMovingEnemy(const sf::Vector2f& pos);

	virtual int width() const override;;
	virtual int height() const override;
	virtual void move(float time) override;
	virtual unsigned char moneyForHit() const override;

protected:
	virtual float speed() const override;
	virtual float maxFrame() const override;
	virtual float frameChangeSpeed() const override;

private:
	sf::Clock timer;
};*/

class EnemiesFactory
{
public:
	static Enemy* createEnemy();
	static void increseAllowedEnemiesTypes();

private:
	static float getRandomCoordinate();
	static unsigned char getRandomEnemyType();

private:
	static const unsigned char MaxEnemiesTypes = 3;
	static unsigned char enemiesTypesAllowed;
};

