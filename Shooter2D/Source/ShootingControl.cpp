#include "ShootingControl.h"

ShootingControl::ShootingControl(const sf::Vector2f& position)
	: timer(),
	  pFont(new sf::Font),
	  pText(new sf::Text),
	  delay(1000),
	  fontSize(13)
{
	pFont->loadFromFile("Resources/Fonts/arial.ttf");

	pText->setFont(*pFont);
	pText->setCharacterSize(fontSize);
	pText->setFillColor(sf::Color::Red);
	pText->setPosition(position.x + fontSize, position.y - fontSize);
	pText->setString("0%");

	timer.restart();
}

ShootingControl::~ShootingControl()
{
	delete pFont;
	delete pText;
}

bool ShootingControl::isDelayOver()
{
	if (timer.getElapsedTime().asMilliseconds() >= delay)
	{
		pText->setFillColor(sf::Color::Red);
		pText->setString("0%");

		timer.restart();

		return true;
	}

	return false;
}

void ShootingControl::update(const sf::Vector2f& position)
{
	pText->setPosition(position.x + fontSize, position.y - fontSize);

	auto time = timer.getElapsedTime().asMilliseconds();
	int reload = time >= delay ? 100 : static_cast<int>((static_cast<float>(time) * 100.0f / static_cast<float>(delay)));

	if (reload == 100)
	{
		pText->setFillColor(sf::Color(3, 32, 16));
	}

	pText->setString(std::to_string(reload) + "%");
}