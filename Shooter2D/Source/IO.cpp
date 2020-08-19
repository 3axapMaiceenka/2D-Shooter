#include "IO.h"
#include "Game.h"

#include <SFML/Graphics.hpp>

IO::IO(unsigned int width, unsigned int height, const std::string& title)
	: pWindow(new sf::RenderWindow(sf::VideoMode(width, height), title)),
	  pGameBackgroundTexture(new sf::Texture),
	  pGameBackground(new sf::Sprite)
{
	pGameBackgroundTexture->loadFromFile("Resources/Images/background.png");
	pGameBackground->setTexture(*pGameBackgroundTexture);
}

IO::~IO()
{
	delete pWindow;
	delete pGameBackgroundTexture;
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
