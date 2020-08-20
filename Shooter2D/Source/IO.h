#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

struct GameBackground
{
	GameBackground();
	~GameBackground();

	// These variables define screen size and limit moving objects displacement
	static constexpr float LeftBound  = 0.0f;
	static constexpr float RightBound = 1167.0f;
	static constexpr float UpperBound = 193.0f;
	static constexpr float LowerBound = 741.0f;

	const sf::Sprite* sprite() const { return pSprite; }

private:
	sf::Texture* pTexture;
	sf::Sprite*  pSprite;
};

class IO
{
public:
	IO(const std::string& title);
	~IO();

	void start();

	void setActiveContext(bool active = true)      { pWindow->setActive(active); }
	void clearWindow()                             { pWindow->clear(); }
	void display()                                 { pWindow->display(); }
	void draw(const sf::Drawable* const pDrawable) { pWindow->draw(*pDrawable); }
	void drawGameBackground()                      { pWindow->draw(*pGameBackground->sprite()); }
	bool isPressed(unsigned int key)               { return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)); }

public:
	GameBackground* pGameBackground;

private:
	sf::RenderWindow* pWindow; 
};
