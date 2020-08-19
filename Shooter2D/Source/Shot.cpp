#include "Game.h"
#include "Shot.h"

Shot::Shot(const sf::Vector2f& pos, Player* pPlayer_)
	: MovingObject(Game::getTexture("Resources/Images/shot.png"), { pos.x - width(), pos.y + 10.0f }),
	  pPlayer(pPlayer_)
{ 
	speedX = speed();
}

Shot::Shot(const Shot& rhs)
	: MovingObject(rhs),
	  pPlayer(rhs.pPlayer)
{ }

Shot::Shot(Shot&& rhs) noexcept
	: MovingObject(std::move(rhs)),
	  pPlayer(std::move(rhs.pPlayer))
{
	rhs.pPlayer = nullptr;
}

Shot& Shot::operator=(const Shot& rhs)
{
	if (this != &rhs)
	{
		MovingObject::operator=(rhs);
		pPlayer = rhs.pPlayer;
	}

	return *this;
}

Shot& Shot::operator=(Shot&& rhs) noexcept
{
	if (this != &rhs)
	{
		MovingObject::operator=(std::move(rhs));
		pPlayer = std::move(rhs.pPlayer);
		rhs.pPlayer = nullptr;
	}

	return *this;
}

void Shot::move(float time)
{
	position.x += time * speed();

	pSprite->setPosition(position);
}

int Shot::width() const
{
	return 10;
}

int Shot::height() const
{
	return 5;
}

float Shot::speed() const
{
	return -1.0f;
}




