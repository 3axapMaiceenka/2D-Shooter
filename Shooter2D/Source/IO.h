#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

class IO
{
public:
	IO(unsigned int width, unsigned int height, const std::string& title);
	~IO();

	void start();

	void setActiveContext(bool active = true)      { pWindow->setActive(active); }
	void clearWindow()                             { pWindow->clear(); }
	void display()                                 { pWindow->display(); }
	void draw(const sf::Drawable* const pDrawable) { pWindow->draw(*pDrawable); }
	void drawGameBackground()                      { pWindow->draw(*pGameBackground); }
	bool isPressed(unsigned int key)               { return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)); }
	
private:
	sf::RenderWindow* pWindow;
	sf::Texture*      pGameBackgroundTexture;
	sf::Sprite*       pGameBackground;
};
