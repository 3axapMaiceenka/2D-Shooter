#include "ShootingControl.h"
#include "GameInfo.h"
#include "Game.h"
#include "IO.h"

GameInfo::GameInfo(IO* io_)
	: border(BorderSize, sf::Color::Green),
	  gunImages(),
	  io(io_)
{ 
	float offset = 0.0f;

	for (int i = 0; i < GunsNumber; i++)
	{
		gunImages.emplace_back(sf::Sprite(*Game::getTexture("Resources/Images/gun" + std::to_string(i + 1) + ".png")));
		gunImages[i].setPosition(offset + GunImagesOffsetX, GunImagesOffsetY);
		offset += gunImages[i].getGlobalBounds().width + DistanceBetweenGunImages;
	}

	border.createBorderAroundSprite(gunImages.front());
}

void GameInfo::draw() const
{
	std::for_each(gunImages.begin(), gunImages.end(), [this](const sf::Sprite& sprite) { io->draw(&sprite); });
	io->draw(&border);
}

void GameInfo::Border::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto vao = border.cbegin(); vao != border.cend(); ++vao)
	{
		target.draw(*vao, states);
	}
}

GameInfo::Border::Border(std::size_t size, sf::Color borderColor)
	: border(),
	  borderSize(size)
{ 
	for (std::size_t i = 0; i < borderSize; i++)
	{
		border.emplace_back(sf::VertexArray(sf::LineStrip, 5));

		for (std::size_t j = 0; j < 5; j++)
		{
			border[i][j].color = borderColor;
		}
	}
}

void GameInfo::Border::createBorderAroundSprite(const sf::Sprite& sprite)
{
	auto rectangle = sprite.getGlobalBounds();

	for (std::size_t i = 0; i < borderSize; i++)
	{
		border[i][0].position = sf::Vector2f(rectangle.left + 1 + i,                   rectangle.top + 1 + i);
		border[i][1].position = sf::Vector2f(rectangle.left + rectangle.width + 1 + i, rectangle.top + 1 + i);
		border[i][2].position = sf::Vector2f(rectangle.left + rectangle.width + 1 + i, rectangle.top + rectangle.height + 1 + i);
		border[i][3].position = sf::Vector2f(rectangle.left + 1 + i,                   rectangle.top + rectangle.height + 1 + i);
		border[i][4].position = sf::Vector2f(rectangle.left + 1 + i,                   rectangle.top + 1 + i);
	}
}