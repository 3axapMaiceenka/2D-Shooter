#include "IO.h"
#include "Game.h"

#include <SFML/Graphics.hpp>

IO::IO(unsigned int width, unsigned int height, const std::string& title)
	: window(sf::VideoMode(width, height), title),
	  gameBackgroundTexture(),
	  gameBackground()
{
	gameBackgroundTexture.loadFromFile("Resources/Images/background.png");
	gameBackground.setTexture(gameBackgroundTexture);
}

void IO::start()
{
	Game game(this);
	auto gameThread = game.start();
	
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				game.finishGame();

				gameThread.join();
				setActiveContext(true);

				window.close();
			}
		}
	}
}
