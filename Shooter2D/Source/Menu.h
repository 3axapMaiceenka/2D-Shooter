#pragma once

#include "Rectangle.h"
#include "Border.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <utility>
#include <string>

const sf::Color buttonBorderColor{ sf::Color::White };
constexpr std::size_t DefaultButtonWidth = 100;
constexpr std::size_t DefaultButtonHeight = 50;
constexpr unsigned char ButtontFontSize = 30;
constexpr unsigned char ButtonBorderSize = 1;

class Menu : public sf::Drawable
{
public:
	Menu(int width, int height, const sf::Vector2f& position, float indicatorRadius, const std::string& textureFilePath, const std::string& fontFilePath);
	~Menu();

	void up();
	void down();
	void onClick();
	void textEntered(char c);
	void setPosition(float x, float y);
	void setFontSize(unsigned char size);
	void setButtonsColor(const sf::Color& color);
	void createButtons(std::size_t buttonsNumber);
	void createTextLines(std::size_t linesNumber);
	void setButtonsTextColor(const sf::Color& color);
	void initButton(const std::string& text, std::function<void(void)> function, std::size_t indx);
	void setButtonSize(std::size_t width = DefaultButtonWidth, std::size_t height = DefaultButtonHeight);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void calcButtonsParams(float& distance, float& buttonsX, float& firstButtonY);
	void moveIndicator();

	template <typename T>
	void createMenuObjects(std::size_t count);

private:
	struct Button
	{
		sf::Text* pText;
		Rectangle* pRectnagle;
		std::function<void(void)> onClickFunction;

		virtual ~Button();
	};

	struct TextLine : Button
	{
		void addSymbol(char c);
	};

private:
	void setButtonTextPosition(Button& buttton);

private:
	std::vector<Button*> buttons;
	TextureRectangle* pRect;
	sf::Font* pFont;
	sf::Color* pButtonsColor;
	sf::Color* pButtonsTextColor;
	sf::CircleShape* pCurrButtonIndicator;
	std::size_t indicator;
	std::size_t buttonWidth;
	std::size_t buttonHeight;
	unsigned char fontSize;
};