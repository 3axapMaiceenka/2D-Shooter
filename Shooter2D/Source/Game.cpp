#include "Game.h"
#include "Level.h"
#include "IO.h"
#include "Player.h"
#include "GameInfo.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <unordered_map>

Game::Game(IO* io_)
	: pMapping(std::make_shared<MappingKeysToControls>()),
	  state(),
	  mutex(),
	  io(io_),
	  activeContext(false),
	  gameOver(false), pause(false), finished(false), shouldSave(false), decisionToSave(false)
{ }

std::thread Game::start()
{
	auto pGameInfo = std::make_shared<GameInfo>(io);
	auto pPlayer = std::make_shared<Player>(io, pGameInfo, sf::Vector2f(500.0f, 500.0f), pMapping);
	auto pLevel = std::make_shared<Level>(io, pGameInfo);

	io->setActiveContext(false);
	std::thread game(&Game::gameLoop, this, pPlayer, pLevel, pGameInfo);
	
	return game;
}

void Game::gameLoop(std::shared_ptr<Player> pPlayer, std::shared_ptr<Level> pLevel, std::shared_ptr<GameInfo> pGameInfo)
{
	io->setActiveContext(true);
	activeContext = true;

	sf::Clock clock;
	gameOver = false;
	pause = false;

	while (!gameOver)
	{
		float elapsedTime = static_cast<float>(clock.restart().asMilliseconds()); 

		checkForPause(clock); // if the game was on pause, the clock will be restarted in that function
		if (finished)
		{
			break;
		}

		pPlayer->move(elapsedTime);

		if (pPlayer->fire())
		{
			pLevel->addShot(pPlayer->getShotPosition(), pPlayer.get(), pPlayer->damage());
		}

		if (!pLevel->update(elapsedTime))
		{
			break;
		}

		io->clearWindow();
		io->drawGameBackground();

		pGameInfo->draw();
		pPlayer->draw();
		pLevel->draw();

		io->display();
	}

	{
		std::scoped_lock lock(mutex);

		io->setActiveContext(false);
		activeContext = false;
		finished = true;
	}

	saveGame(pGameInfo);
}

void Game::checkForPause(sf::Clock& clock)
{
	std::unique_lock lock(mutex);

	if (pause)
	{	
		io->setActiveContext(false);
		activeContext = false;
		state.wait(lock, [this]() { return !pause || finished; } );

		if (finished)
		{
			gameOver = true;
		}
		else
		{
			clock.restart();
			io->setActiveContext(true);
			activeContext = true;
		}
	}
}

void Game::saveGame(std::shared_ptr<GameInfo> pGameInfo)
{
	std::unique_lock lock(mutex);

	if (!decisionToSave)
	{
		state.wait(lock, [this]() { return decisionToSave || shouldSave; });

		if (shouldSave)
		{
			pGameInfo->saveToFile();
		}
	}
}

void Game::setNeedToSave(bool toSave /* = true */)
{
	std::scoped_lock lock(mutex);

	shouldSave = toSave;
	decisionToSave = true;

	state.notify_one();
}

void Game::setPause(bool onPause /* = true */)
{
	std::scoped_lock lock(mutex);

	pause = onPause;
	if (!pause)
	{
		state.notify_one();
	}
}

void Game::finishGame()
{
	std::scoped_lock lock(mutex);

	pause = true;
	finished = true;
	decisionToSave = true;

	state.notify_one();
}

bool Game::isGameFinished()
{
	std::scoped_lock lock(mutex);

	return finished;
}

MappingKeysToControls::MappingKeysToControls()
	: mapping()
{
	mapping.emplace(PlayerControls::UP,     static_cast<unsigned int>(sf::Keyboard::Key::W));
	mapping.emplace(PlayerControls::DOWN,   static_cast<unsigned int>(sf::Keyboard::Key::S));
	mapping.emplace(PlayerControls::LEFT,   static_cast<unsigned int>(sf::Keyboard::Key::A));
	mapping.emplace(PlayerControls::RIGHT,  static_cast<unsigned int>(sf::Keyboard::Key::D));
	mapping.emplace(PlayerControls::SHOOT,  static_cast<unsigned int>(sf::Keyboard::Key::LShift));
	mapping.emplace(PlayerControls::RELOAD, static_cast<unsigned int>(sf::Keyboard::Key::R));
	mapping.emplace(PlayerControls::GUN1,   static_cast<unsigned int>(sf::Keyboard::Key::Num1));
	mapping.emplace(PlayerControls::GUN2,   static_cast<unsigned int>(sf::Keyboard::Key::Num2));
	mapping.emplace(PlayerControls::GUN3,   static_cast<unsigned int>(sf::Keyboard::Key::Num3));
	mapping.emplace(PlayerControls::GUN4,   static_cast<unsigned int>(sf::Keyboard::Key::Num4));
	mapping.emplace(PlayerControls::GUN5,   static_cast<unsigned int>(sf::Keyboard::Key::Num5));
}
