#include "Enemies.h"
#include "Shot.h"
#include "Game.h"
#include "IO.h"

Enemy::Enemy(const sf::Texture* const pTexture, const sf::Vector2f& pos, unsigned char health)
	: AnimatedObject(pTexture, pos),
	  hp(health),
	  alive(true),
	  dead(false)
{ }

Enemy::Enemy(const Enemy& rhs)
	: AnimatedObject(rhs),
	  hp(rhs.hp),
	  alive(rhs.alive),
	  dead(rhs.dead)
{ }

Enemy::Enemy(Enemy&& rhs) noexcept
	: AnimatedObject(std::move(rhs)),
	  hp(rhs.hp),
	  alive(rhs.alive),
	  dead(rhs.dead)
{
	rhs.hp = 0;
	rhs.alive = true;
	rhs.dead = false;
}

Enemy& Enemy::operator=(const Enemy& rhs)
{
	if (this != &rhs)
	{
		AnimatedObject::operator=(rhs);
		hp = rhs.hp;
		alive = rhs.alive;
		dead = rhs.dead;
	}

	return *this;
}

Enemy& Enemy::operator=(Enemy&& rhs) noexcept
{
	if (this != &rhs)
	{
		AnimatedObject::operator=(std::move(rhs));
		hp = rhs.hp;
		alive = rhs.alive;
		dead = rhs.dead;

		rhs.hp = 0;
		rhs.alive = true;
		rhs.dead = false;
	}

	return *this;
}

bool Enemy::hit(const Shot& shot)
{
	if (alive && sf::Rect<float>(position.x, position.y, static_cast<float>(size().x), static_cast<float>(size().y)).intersects(
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

void Enemy::changeFrame(float time)
{
	increaseFrame(time);

	if (currentFrame >= maxFrame())
	{
		if (alive)
		{
			currentFrame = 0.0f;
		}
		else
		{
			currentFrame--;
			dead = true;
		}
	}

	setTextureCoord();
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

int WeakEnemy::width()                 const { return 27; }
int WeakEnemy::height()                const { return 42; }
unsigned char WeakEnemy::moneyForHit() const { return 20; }
float WeakEnemy::speed()               const { return 0.04f; }
float WeakEnemy::frameChangeSpeed()    const { return 0.015f; }

