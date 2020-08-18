#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

class IO
{
public:
	IO(unsigned int width, unsigned int height, const std::string& title);

	void start();

	void setActiveContext(bool active = true)  { window.setActive(active); }
	void clearWindow()                         { window.clear(); }
	void display()                             { window.display(); }
	void draw(const sf::Sprite* const pSprite) { window.draw(*pSprite); }
	void drawGameBackground()                  { window.draw(gameBackground); };
	bool isPressed(unsigned int key)           { return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)); }
	
private:
	sf::RenderWindow window;
	sf::Texture gameBackgroundTexture;
	sf::Sprite  gameBackground;
};
