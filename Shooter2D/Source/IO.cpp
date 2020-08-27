#include "IO.h"
#include "Game.h"
#include "Factory.h"

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
	bool onPause = false; // temp

	while (pWindow->isOpen())
	{
		if (game.isGameFinished())
		{
			stop(std::move(gameThread));
			return;
		}

		sf::Event event;

		while (pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				game.finishGame();
				stop(std::move(gameThread));
				return;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape) 
				{
					onPause = !onPause; // temp
					game.setPause(onPause);
				}
			}
		}
	}
}

void IO::stop(std::thread&& gameThread)
{
	gameThread.join();
	pWindow->setActive(true);
	pWindow->close();
}

GameBackground::GameBackground()
	: pSprite(new sf::Sprite(*TextureFactory::getInstance().loadFromFile("Resources/Images/background.png")))
{ }

GameBackground::~GameBackground()
{
	delete pSprite;
}
