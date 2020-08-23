#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <condition_variable>

enum class PlayerControls;
class IO;
class Player;
class Level;

struct MappingKeysToControls
{
	MappingKeysToControls();
	std::unordered_map<PlayerControls, unsigned int> mapping;
};

class Game
{
public:
	Game(IO* io_);
	~Game();

	void finishGame();
	std::thread start();
	bool isGameFinished();
	void setPause(bool onPause = true);

	static const sf::Texture* const getTexture(const std::string& filePath) { return pTextureFactory->getTexture(filePath); }

private:
	void gameLoop(std::shared_ptr<Player> pPlayer, std::shared_ptr<Level> pLevel);
	void checkForPause(sf::Clock& clock);

private:
	struct TextureFactory
	{
		~TextureFactory();
		sf::Texture* getTexture(const std::string& filePath);

	private:
		std::unordered_map<std::string, sf::Texture*> textureCash;
	};

private:
	static std::unique_ptr<TextureFactory> pTextureFactory;
	std::shared_ptr<MappingKeysToControls> pMapping;
	std::condition_variable state;
	std::mutex mutex;
	IO* io;
	bool gameOver;
	bool pause;
	bool finished;
};
