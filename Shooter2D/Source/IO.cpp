#include "IO.h"
#include "Menu.h"
#include "Game.h"
#include "Factory.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <utility>

IO::IO(const std::string& title)
	: pWindow(new sf::RenderWindow(sf::VideoMode(static_cast<unsigned>(GameBackground::RightBound), static_cast<unsigned>(GameBackground::LowerBound)), title)),
	  pGameBackground(new GameBackground),
	  scenes(5, nullptr),
	  pGame(nullptr),
	  gameThread(),
	  currentScene(Scenes::MAIN_SCENE),
	  gameOnPause(false)
{ 
	createMainScene();
}

IO::~IO()
{
	delete pWindow;
	delete pGameBackground;
	
	std::for_each(scenes.begin(), scenes.end(), [](Menu* pScene) { delete pScene; });
}

void IO::createMainScene()
{
	MAIN_SCENE_PTR(scenes) = new Menu(GameBackground::RightBound, GameBackground::LowerBound, { 0.0f, 0.0f }, 10.0f,
		"Resources/Images/menuBackground.png", "Resources/Fonts/arial.ttf");

	MAIN_SCENE_PTR(scenes)->setButtonSize(300, 50);
	MAIN_SCENE_PTR(scenes)->setButtonsColor(sf::Color::Black);
	MAIN_SCENE_PTR(scenes)->setButtonsTextColor(sf::Color::White);
	MAIN_SCENE_PTR(scenes)->setFontSize(30);
						 
	MAIN_SCENE_PTR(scenes)->createButtons(4);
	MAIN_SCENE_PTR(scenes)->initButton("One Player Game", std::bind(&IO::startGame, this), 0);
	MAIN_SCENE_PTR(scenes)->initButton("Two Players Game", []() {}, 1);
	MAIN_SCENE_PTR(scenes)->initButton("Stats", []() {}, 2);
	MAIN_SCENE_PTR(scenes)->initButton("Settings", []() {}, 3);
}

void IO::createOnPauseScene()
{
	if (!ON_PAUSE_SCENE_PTR(scenes))
	{
		ON_PAUSE_SCENE_PTR(scenes) = new Menu(GameBackground::RightBound, GameBackground::LowerBound, { 0.0f, 0.0f }, 10.0f,
			"Resources/Images/menuBackground.png", "Resources/Fonts/arial.ttf");

		ON_PAUSE_SCENE_PTR(scenes)->setButtonSize(300, 50);
		ON_PAUSE_SCENE_PTR(scenes)->setButtonsColor(sf::Color::Black);
		ON_PAUSE_SCENE_PTR(scenes)->setButtonsTextColor(sf::Color::White);
		ON_PAUSE_SCENE_PTR(scenes)->setFontSize(30);

		ON_PAUSE_SCENE_PTR(scenes)->createButtons(3);
		ON_PAUSE_SCENE_PTR(scenes)->initButton("Continue", std::bind(&IO::pauseGame, this), 0);
		ON_PAUSE_SCENE_PTR(scenes)->initButton("Main menu", std::bind(&IO::stopGame, this), 1);
		ON_PAUSE_SCENE_PTR(scenes)->initButton("Seetings", []() {}, 2);
	}
}

void IO::start()
{
	while (pWindow->isOpen())
	{
		sf::Event event;

		while (pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (pGame) stopGame();
				return;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (pGame && event.key.code == sf::Keyboard::Key::Escape)
				{
					pauseGame();
				}
				else
				{
					if (event.key.code == sf::Keyboard::Key::Down)       CURRENT_SCENE_PTR(scenes, currentScene)->down();
					else if (event.key.code == sf::Keyboard::Key::Up)    CURRENT_SCENE_PTR(scenes, currentScene)->up();
					else if (event.key.code == sf::Keyboard::Key::Enter) CURRENT_SCENE_PTR(scenes, currentScene)->onClick();
				}
			}
		}

		if (!pGame || gameOnPause)
		{
			pWindow->clear();
			pWindow->draw(*CURRENT_SCENE_PTR(scenes, currentScene));
			pWindow->display();
		}
		else
		{
			if (pGame->isGameFinished())
			{
				killGameThread(std::move(gameThread));
			}
		}
	}
}

void IO::killGameThread(std::thread&& gameThread)
{
	gameThread.join();
	delete pGame;
	pGame = nullptr;
	pWindow->setActive(true);
}

void IO::startGame()
{
	pGame = new Game(this);
	gameThread = pGame->start();
}

GameBackground::GameBackground()
	: pSprite(new sf::Sprite(*TextureFactory::getInstance().loadFromFile("Resources/Images/background.png")))
{ }

GameBackground::~GameBackground()
{
	delete pSprite;
}

void IO::pauseGame()
{
	gameOnPause = !gameOnPause;

	if (!gameOnPause) pWindow->setActive(false);

	pGame->setPause(gameOnPause);

	if (gameOnPause)
	{
		currentScene = Scenes::ON_PAUSE_SCENE;
		createOnPauseScene();
		setActiveContext(true);
	}
}

void IO::stopGame()
{
	pGame->finishGame();
	killGameThread(std::move(gameThread));
}