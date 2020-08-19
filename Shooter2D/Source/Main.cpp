#include "IO.h"

#include "ShootingControl.h"
#include "Shot.h"
#include "Player.h"
#include "Game.h"
#include "IO.h"
#include <iostream>

int main()
{
	IO io(1167, 741, "Game");
	io.start();

#if 0
	std::cout << "Shot " << sizeof(Shot) << "\n";
	std::cout << "Game " << sizeof(Game) << "\n";
	std::cout << "PLayer " << sizeof(Player) << "\n";
	std::cout << "IO " << sizeof(IO) << "\n";
	std::cout << "ShootingControl " << sizeof(ShootingControl) << "\n";
	std::cout << "Sprite " << sizeof(sf::Sprite) << "\n";
	std::cout << "Texture " << sizeof(sf::Texture) << "\n";
	std::cout << "RenderWindwo " << sizeof(sf::RenderWindow) << "\n";
	system("pause");
#endif

	return 0;
}