#pragma once

#include "MovingObject.h"

class Player;

class Shot : public MovingObject
{
public:
	Shot(const sf::Vector2f& pos, Player* pPlayer_);
	Shot(const Shot& rhs);
	Shot(Shot&& rhs) noexcept;
	Shot& operator=(const Shot& rhs);
	Shot& operator=(Shot&& rhs) noexcept;

	virtual int width() const override;
	virtual int height() const override;
	virtual void move(float time) override;

	const Player* getPlayer() const { return pPlayer; }

protected:
	virtual float speed() const override;

private:
	Player* pPlayer;
};
