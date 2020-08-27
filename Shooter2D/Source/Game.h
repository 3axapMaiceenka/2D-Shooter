#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <condition_variable>

enum class PlayerControls;
class IO;
class Player;
class Level;
class GameInfo;

struct MappingKeysToControls
{
	MappingKeysToControls();
	std::unordered_map<PlayerControls, unsigned int> mapping;
};

class Game
{
public:
	Game(IO* io_);

	void finishGame();
	std::thread start();
	bool isGameFinished();
	void setPause(bool onPause = true);
	void setNeedToSave(bool toSave = true);

private:
	void gameLoop(std::shared_ptr<Player> pPlayer, std::shared_ptr<Level> pLevel, std::shared_ptr<GameInfo> pGameInfo);
	void checkForPause(sf::Clock& clock);
	void saveGame(std::shared_ptr<GameInfo> pGameInfo);

private:
	std::shared_ptr<MappingKeysToControls> pMapping;
	std::condition_variable state;
	std::mutex mutex;
	IO* io;
	bool gameOver;
	bool pause;
	bool finished;
	bool shouldSave;
	bool decisionToSave;
};
