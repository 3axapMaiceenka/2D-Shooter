#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <vector>

class GameInfo;

const sf::Color GreenColor = sf::Color(3, 32, 16);
constexpr unsigned char GunsNumber = 5;

#define GUN_LABEL_OFFSET_X    20
#define GUN_LABEL_OFFSET_Y   -13
#define MONEY_LABEL_OFFSET_X -19
#define MONEY_LABEL_OFFSET_Y  15
#define SET_TEXT_POSITION(pText, position, offsetX, offsetY) pText->setPosition(position.x + (offsetX), position.y + (offsetY));

class ShootingControl
{
public:
	ShootingControl(const sf::Vector2f& position, std::shared_ptr<GameInfo> pGi);
	~ShootingControl();

	bool isDelayOver();
	void reload();
	bool setNewGun(unsigned char indx);
	void increaseMoney(unsigned int value);
	void update(const sf::Vector2f& position);

	const auto getGunLabel() const { return pGunLabel; }
	const auto getMoneyLabel() const { return pMoneyLabel;  }
	auto getCurrentGunDamage() const { return guns[currentGun].Damage; }

private:
	void initGuns();
	void initText(sf::Text* pText, const sf::Font* pFont, const sf::Color& color, const std::string& string, unsigned int characterSize);

private:
	struct Gun
	{
		Gun(sf::Int32 reloadingTime, sf::Int32 delay, unsigned int price_, unsigned char numberOfShots, unsigned char damage);

		const sf::Int32 ReloadingTime;
		const sf::Int32 Delay;
		unsigned int price;
		const unsigned char NumberOfShots;
		const unsigned char Damage;
		unsigned char shots;
	};

private:
	std::shared_ptr<GameInfo> pGameInfo;
	std::vector<Gun> guns;
	sf::Clock timer;
	sf::Font* pFont;
	sf::Text* pGunLabel;
	sf::Text* pMoneyLabel;
	unsigned int playerMoney;
	unsigned char currentGun;
	const unsigned char fontSize;
};