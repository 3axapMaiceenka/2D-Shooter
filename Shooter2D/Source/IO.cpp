#include "IO.h"
#include "Game.h"

#include <SFML/Graphics.hpp>

IO::IO(const std::string& title)
	: pWindow(new sf::RenderWindow(sf::VideoMode(static_cast<unsigned>(GameBackground::RightBound), static_cast<unsigned>(GameBackground::LowerBound)), title)),
	  pGameBackground(new GameBackground)
{ }

IO::~IO()
{
	delete pWindow;
	delete pGameBackground;
}

void IO::start()
{
	Game game(this);
	auto gameThread = game.start();
	
	while (pWindow->isOpen())
	{
		sf::Event event;

		while (pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				game.finishGame();

				gameThread.join();
				setActiveContext(true);

				pWindow->close();
			}
		}
	}
}

GameBackground::GameBackground()
	: pTexture(new sf::Texture),
	  pSprite(new sf::Sprite)
{
	pTexture->loadFromFile("Resources/Images/background.png");
	pSprite->setTexture(*pTexture);
}

GameBackground::~GameBackground()
{
	delete pTexture;
	delete pSprite;
}
