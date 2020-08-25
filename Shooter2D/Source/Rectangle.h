#pragma once

#include <SFML/Graphics.hpp>

class Border;

class Rectangle : public sf::Drawable
{
public:
	Rectangle(float left, float top, int width, int height, const sf::Color& color, std::size_t borderSize = 0, const sf::Color& borderColor = sf::Color::White);
	Rectangle(const sf::Vector2f& position_, const sf::Vector2i& size, const sf::Color& color, std::size_t borderSize = 0, const sf::Color& borderColor = sf::Color::White);
	~Rectangle();
	
	void setPosition(float left, float top);
	void setWidth(int width);
	void setHeight(int height);
	void resize(int width, int height);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Vector2f position;
	sf::VertexArray* pRectangle;
	Border* pBorder;
	int width;
	int height;
};
