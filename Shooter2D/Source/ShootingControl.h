#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <string>

class ShootingControl
{
public:
	ShootingControl(const sf::Vector2f& position);
	~ShootingControl();

	void update(const sf::Vector2f& position);
	bool isDelayOver();

	const sf::Text* const getText() const { return pText; }

private:
	sf::Clock timer;
	sf::Font* pFont;
	sf::Text* pText;
	const sf::Int32 delay;
	const unsigned char fontSize;
};