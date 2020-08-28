#include "Menu.h"
#include "Factory.h"

Menu::Menu(int width, int height, const sf::Vector2f& position, float indicatorRadius, const std::string& textureFilePath, const std::string& fontFilePath)
	: buttons(),
	  pRect(new TextureRectangle(position.x, position.y, width, height, TextureFactory::getInstance().loadFromFile(textureFilePath))),
	  pFont(FontFactory::getInstance().loadFromFile(fontFilePath)),
	  pButtonsColor(new sf::Color),
	  pButtonsTextColor(new sf::Color(sf::Color::White)),
	  pCurrButtonIndicator(new sf::CircleShape(indicatorRadius)),
	  indicator(0),
	  buttonWidth(DefaultButtonWidth),
	  buttonHeight(DefaultButtonHeight),
	  fontSize(ButtontFontSize)
{ }

Menu::~Menu()
{
	delete pRect;
	delete pButtonsColor;
	delete pButtonsTextColor;
	delete pCurrButtonIndicator;

	std::for_each(buttons.begin(), buttons.end(), [](Button* pButton) { delete pButton; });
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*pRect, states);

	std::for_each(buttons.cbegin(), buttons.cend(), [&target, &states](Button* pButton) 
	{ 
		target.draw(*pButton->pRectnagle, states); 
		target.draw(*pButton->pText, states);
	});

	if (!buttons.empty())
	{
		target.draw(*pCurrButtonIndicator, states);
	}
}

void Menu::down()
{
	if (++indicator >= buttons.size())
	{
		indicator = 0;
	}

	if (!buttons.empty())
	{
		moveIndicator();
	}
}

void Menu::up()
{
	indicator ? indicator-- : indicator = buttons.size() - 1;

	if (!buttons.empty())
	{
		moveIndicator();
	}
}

void Menu::onClick()
{
	buttons[indicator]->onClickFunction();
}

void Menu::setPosition(float x, float y)
{
	pRect->setPosition(x, y);
}

void Menu::setFontSize(unsigned char size)
{
	fontSize = size;
	
	for (auto& button : buttons)
	{
		button->pText->setCharacterSize(fontSize);
		setButtonTextPosition(*button);
	}
}

void Menu::setButtonSize(std::size_t width, std::size_t height)
{
	buttonWidth = width;
	buttonHeight = height;

	if (buttons.empty()) return;

	float distanceBetweenButtons, buttonRectX, buttonRectY;
	calcButtonsParams(distanceBetweenButtons, buttonRectX, buttonRectY);
	
	for (auto& button : buttons)
	{
		button->pRectnagle->setPosition(buttonRectX, buttonRectY);
		setButtonTextPosition(*button);

		buttonRectY += (distanceBetweenButtons + buttonHeight);
	}

	moveIndicator();
}

void Menu::initButton(const std::string& text, std::function<void(void)> function, std::size_t indx)
{
	Button* pButton = buttons[indx];

	pButton->pText->setString(text);
	pButton->onClickFunction = function;
	setButtonTextPosition(*pButton);
}

void Menu::setButtonTextPosition(Button& button)
{
	const auto& textSize = button.pText->getLocalBounds();
	const auto& buttonRectPos = button.pRectnagle->getPosition();

	button.pText->setPosition(buttonRectPos.x + static_cast<float>(buttonWidth - textSize.width) / 2.0f,
		                      buttonRectPos.y + static_cast<float>(buttonHeight - fontSize) / 2.0f);
}

void Menu::calcButtonsParams(float& distance, float& buttonsX, float& firstButtonY)
{
	distance = static_cast<float>(pRect->getHeight() - (buttons.size() * buttonHeight)) / static_cast<float>(buttons.size() + 1);
	buttonsX = static_cast<float>(pRect->getWidth() - buttonWidth) / 2.0f + pRect->getX();
	firstButtonY = distance + pRect->getY();
}

// T is either TextLine or Button
template <typename T>
void Menu::createMenuObjects(std::size_t count)
{
	auto prevSize = buttons.size();
	buttons.insert(buttons.cend(), count, nullptr);

	float distanceBetweenButtons, buttonRectX, buttonRectY;
	calcButtonsParams(distanceBetweenButtons, buttonRectX, buttonRectY);

	for (std::size_t i = 0; i < buttons.size(); i++, buttonRectY += (buttonHeight + distanceBetweenButtons))
	{
		Button*& pButton = buttons[i];

		if (i >= prevSize)
		{
			pButton = new T; // T is either TextLine or Button
			pButton->pRectnagle = new Rectangle(buttonRectX, buttonRectY, buttonWidth, buttonHeight, *pButtonsColor, ButtonBorderSize, sf::Color::White/*buttonBorderColor*/);
			pButton->pText = new sf::Text("", *pFont, fontSize);
			pButton->pText->setFillColor(*pButtonsTextColor);
		}
		else
		{
			pButton->pRectnagle->setPosition(buttonRectX, buttonRectY);
		}

		setButtonTextPosition(*pButton);
	}

	moveIndicator();
}

void Menu::createButtons(std::size_t buttonsNumber)
{
	createMenuObjects<Button>(buttonsNumber);
}

void Menu::setButtonsColor(const sf::Color& color)
{
	*pButtonsColor = color;
	std::for_each(buttons.begin(), buttons.end(), [&color](Button* pButton) { pButton->pRectnagle->setColor(color); });
}

void Menu::setButtonsTextColor(const sf::Color& color)
{
	*pButtonsTextColor = color;
	std::for_each(buttons.begin(), buttons.end(), [&color](Button* pButton) { pButton->pText->setFillColor(color); });
}

void Menu::moveIndicator()
{
	if (buttons.empty())
	{
		return;
	}

	const auto& curButtonPos = buttons[indicator]->pRectnagle->getPosition();
	float radius = pCurrButtonIndicator->getRadius();

	pCurrButtonIndicator->setPosition(curButtonPos.x - 3.0f * radius, curButtonPos.y + static_cast<float>(buttonHeight) * 0.5f - radius);
}

Menu::Button::~Button()
{
	delete pText;
	delete pRectnagle;
}

void Menu::TextLine::addSymbol(char c)
{
	std::string text = pText->getString();

	if (c == '\b')
	{
		text.erase(--text.end());
	}
	else
	{
		text += c;
	}

	pText->setString(text);

	while (pText->getLocalBounds().width >= pRectnagle->getWidth() && pText->getCharacterSize())
	{
		pText->setCharacterSize(pText->getCharacterSize() - 1);
	}
}

void Menu::createTextLines(std::size_t linesNumber)
{
	createMenuObjects<TextLine>(linesNumber);
}

void Menu::textEntered(char c)
{
	TextLine* pTextLine = dynamic_cast<TextLine*>(buttons[indicator]);
	if (pTextLine)
	{
		pTextLine->addSymbol(c);
	}
}