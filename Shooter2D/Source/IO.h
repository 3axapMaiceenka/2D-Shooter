#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <thread>
#include <vector>

class Menu;
class Game;

struct GameBackground
{
	GameBackground();
	~GameBackground();

	// These variables define screen size and limit moving objects displacement
	static constexpr float LeftBound  = 0.0f;
	static constexpr float RightBound = 1167.0f;
	static constexpr float UpperBound = 240.0f;
	static constexpr float LowerBound = 741.0f;

	const sf::Sprite* sprite() const { return pSprite; }

private:
	sf::Sprite*  pSprite;
};

enum class Scenes
{
	MAIN_SCENE,
	ON_PAUSE_SCENE,
	SETTINGS_SCENE,
	STATS_SCENE,
	SAVE_GAME_SCENE
};

#define CURRENT_SCENE_PTR(scenes, currentScene) scenes[static_cast<std::size_t>(currentScene)]
#define MAIN_SCENE_PTR(scenes)      CURRENT_SCENE_PTR(scenes, Scenes::MAIN_SCENE)			
#define ON_PAUSE_SCENE_PTR(scenes)	CURRENT_SCENE_PTR(scenes, Scenes::ON_PAUSE_SCENE)			
#define SETTINGS_SCENE_PTR(scenes)	CURRENT_SCENE_PTR(scenes, Scenes::SETTINGS_SCENE)			
#define STATS_SCENE_PTR(scenes)		CURRENT_SCENE_PTR(scenes, Scenes::STATS_SCENE)			
#define SAVE_GAME_SCENE(scenes)		CURRENT_SCENE_PTR(scenes, Scenes::SVAE_GAME_SCENE)			

class IO
{
public:
	IO(const std::string& title);
	~IO();

	void start();

	void clearWindow()                             { pWindow->clear(); }
	void display()                                 { pWindow->display(); }
	void draw(const sf::Drawable* const pDrawable) { pWindow->draw(*pDrawable); }
	void setActiveContext(bool active = true)      { pWindow->setActive(active); }
	void drawGameBackground()                      { pWindow->draw(*pGameBackground->sprite()); }
	bool isPressed(unsigned int key)               { return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)); }

private:
	void stopGame();
	void startGame();
	void pauseGame();
	void createMainScene();
	void createOnPauseScene();
	void killGameThread(std::thread&& gameThread);

private:
	sf::RenderWindow* pWindow; 
	GameBackground* pGameBackground;
	std::vector<Menu*> scenes;
	Game* pGame;
	std::thread gameThread;
	Scenes currentScene;
	bool gameOnPause;
};
