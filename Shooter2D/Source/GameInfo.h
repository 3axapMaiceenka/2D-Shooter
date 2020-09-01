#pragma once

#include "IO.h"
#include "Border.h"
#include "Rectangle.h"

#include <SFML/Graphics.hpp>

class IO;

constexpr float GunImagesOffsetX = 2.0f;
constexpr float GunImagesOffsetY = 3.0f;
constexpr float DistanceBetweenGunImages = 5.0f;
constexpr std::size_t BorderSize = 2;
constexpr unsigned int FontSize = 20;
constexpr int LabelsBoxWidth = 200;
constexpr int LabelsBoxHeight = 75;
constexpr float LabelsBoxX = GameBackground::RightBound - static_cast<float>(LabelsBoxWidth + BorderSize + 1);
constexpr int ProgressBarHeight = 3;
constexpr float ProgressBarX = LabelsBoxX + BorderSize;
constexpr float ProgressBarY = LabelsBoxHeight + GunImagesOffsetY - BorderSize - ProgressBarHeight - 3.0f;
constexpr int ProgressBarMaxWidth = LabelsBoxWidth - (BorderSize * 2);

class GameInfo
{
public:
	GameInfo(IO* io_);
	~GameInfo();

	void incWave();
	void draw() const;
	void saveToFile() const;
	void incEnemiesKilled(float ratio);

	void indicateCurrentGun(unsigned char indx) { pBorder->createBorderAround(pGunImages[0][indx]); }

private:
	void initTextBox();
	void initLabel(sf::Text* pLabel, const sf::Color& color, float x, float y);

private:
	struct TextBox
	{
		sf::Text* pWaveLabel = nullptr;
		sf::Text* pEnemiesKilledLabel = nullptr;

		~TextBox();
	};

private:
	Rectangle* pLabelsBox;
	Rectangle* pProgressBar;
	TextBox* pTextBox;
	Border* pBorder;
	std::vector<sf::Sprite>* pGunImages;
	IO* io;
	unsigned short enemiesKilled;
	unsigned short wave;
};
