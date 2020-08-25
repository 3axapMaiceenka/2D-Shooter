#include "Rectangle.h"
#include "Border.h"

#include <SFML/System.hpp>

Rectangle::Rectangle(float left, float top, int width_, int height_, const sf::Color& color, std::size_t borderSize, const sf::Color& borderColor)
	: position(left, top),
	  pRectangle(new sf::VertexArray(sf::Quads, 4)),
	  pBorder(new Border(borderSize, borderColor)),
	  width(width_),
	  height(height_)
{
	setPosition(left, top);

	for (int i = 0; i < 4; i++)
	{
		pRectangle[0][i].color = color;
	}

	pBorder->createBorderAround(*pRectangle);
}

Rectangle::Rectangle(const sf::Vector2f& position_, const sf::Vector2i& size, const sf::Color& color, std::size_t borderSize, const sf::Color& borderColor)
	: Rectangle(position_.x, position_.y, size.x, size.y, color, borderSize, borderColor)
{ }

Rectangle::~Rectangle()
{
	delete pRectangle;
	delete pBorder;
}

void Rectangle::setPosition(float left, float top)
{
	pRectangle[0][0].position = sf::Vector2f(left, top);
	pRectangle[0][1].position = sf::Vector2f(left + width, top);
	pRectangle[0][2].position = sf::Vector2f(left + width, top + height);
	pRectangle[0][3].position = sf::Vector2f(left, top + height);
}

void Rectangle::resize(int width_, int height_)
{
	width = width_;
	height = height_;

	setPosition(position.x, position.y);
}

void Rectangle::setWidth(int width_)
{
	width = width_;

	pRectangle[0][1].position = sf::Vector2f(position.x + width, position.y);
	pRectangle[0][2].position = sf::Vector2f(position.x + width, position.y + height);
}

void Rectangle::setHeight(int height_)
{
	height = height_;

	pRectangle[0][2].position = sf::Vector2f(position.x + width, position.y + height);
	pRectangle[0][3].position = sf::Vector2f(position.x, position.y + height);
}														

void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*pRectangle, states);
	target.draw(*pBorder, states);
}
