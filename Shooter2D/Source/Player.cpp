#include "Game.h"
#include "Player.h"
#include "IO.h"

#include <string>

Player::Player(IO* io_, const sf::Vector2f& pos, std::shared_ptr<MappingKeysToControls> pMappingKeysToControls)
	: AnimatedObject(Game::getTexture("Resources/Images/player1.png"), pos),
	  pShootingControl(std::make_unique<ShootingControl>(pos)),
	  pMapping(pMappingKeysToControls),
	  io(io_),
	  speedY(0.0f),
	  textureRow(0.0f),
	  shot(false)
{ 
	pSprite->setTextureRect(sf::IntRect(0, 0, width(), height()));
}

void Player::move(float time)
{
	update(time);

	if (speedX || speedY)
	{
		position.x += time * speedX;
		position.y += time * speedY;

		if (position.x < GameBackground::LeftBound) position.x = GameBackground::LeftBound;
		else if (position.x + width() > GameBackground::RightBound) position.x = GameBackground::RightBound- width();
		if (position.y < GameBackground::UpperBound) position.y = GameBackground::UpperBound;
		else if (position.y + height() > GameBackground::LowerBound) position.y = GameBackground::LowerBound - height();

		pSprite->setPosition(position);
		speedX = speedY = 0.0f;
	}
}

void Player::update(float time)
{
	if (io->isPressed(pMapping->mapping[PlayerControls::UP]))
	{
		speedY = -speed();
	}
	if (io->isPressed(pMapping->mapping[PlayerControls::DOWN]))
	{
		speedY = speed();
	}
	if (io->isPressed(pMapping->mapping[PlayerControls::LEFT]))
	{
		speedX = -speed();
	}
	if (io->isPressed(pMapping->mapping[PlayerControls::RIGHT]))
	{
		speedX = speed();
	}
	if (io->isPressed(pMapping->mapping[PlayerControls::SHOOT]))
	{
		shot = pShootingControl->isDelayOver();
	}

	changeFrame(time);
	pShootingControl->update(position);
}

bool Player::fire()
{
	if (shot)
	{
		shot = false;
		return true;
	}

	return false;
}

void Player::changeFrame(float time)
{
	if (speedX || speedY)
	{
		AnimatedObject::changeFrame(time);
	}
	else
	{
		currentFrame = 0.0f;
		pSprite->setTextureRect(sf::IntRect(0, static_cast<int>(textureRow * height()), width(), height()));
	}
}

int Player::width() const
{
	return 40;
}

int Player::height() const
{
	return 40;
}

float Player::speed() const
{
	return 0.06f;
}

float Player::frameChangeSpeed() const
{
	return 0.012f;
}

float Player::maxFrame() const
{
	return 6.0f;
}

void Player::draw() const
{
	io->draw(pSprite.get());
	io->draw(pShootingControl->getText());
}