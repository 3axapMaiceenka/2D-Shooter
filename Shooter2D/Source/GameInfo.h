#pragma once

#include <SFML/Graphics.hpp>

class IO;

constexpr float GunImagesOffsetX = 2.0f;
constexpr float GunImagesOffsetY = 3.0f;
constexpr float DistanceBetweenGunImages = 5.0f;
constexpr std::size_t BorderSize = 2;

class GameInfo
{
public:
	GameInfo(IO* io_);

	void draw() const;

	void indicateCurrentGun(unsigned char indx) { border.createBorderAroundSprite(gunImages[indx]); }

private:
	class Border : public sf::Drawable
	{
	public:
		Border(std::size_t size, sf::Color borderColor);
		void createBorderAroundSprite(const sf::Sprite& sprite);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	private:
		std::vector<sf::VertexArray> border;
		std::size_t borderSize;
	};

private:
	Border border;
	std::vector<sf::Sprite> gunImages;
	IO* io;
};
