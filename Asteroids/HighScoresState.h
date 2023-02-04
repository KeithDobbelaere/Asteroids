#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Application.h"
#include "StarField.h"
#include "TextEffects.h"

#include <array>


class HighScoresState : public GameState
{
public:
	HighScoresState(AppDataPtr data, GameDataPtr gameData);
	~HighScoresState();


	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;

private:
	void refreshDifficulty(const sf::Color& color);
	void refreshScores(float xPos, const sf::Color& color);
	void initializeScoresText(float xPos, const sf::Color& color, int difficulty);
	void swapEntries(int index);
	void slideLeft();
	void slideRight();

	float m_lineSpacing = 50.f, direction;
	int m_displayedDifficulty, alpha = 255, slideStep = 5, fadeInOut = -slideStep;
	int m_swapIndex = 10;
	sf::Vector2f dest1, dest2;
	bool swapping = false, sorted = false, slide = false;
	sf::Color m_clearColor, m_textColor;
	sf::Text m_titleText, m_difficultyText, m_leftArrow, m_rightArrow, m_backText;
	std::array<std::array<RainbowShiftText, 12>, 3> m_nameTexts;
	std::array<std::array<RainbowShiftText, 12>, 3> m_scoreTexts;
	RainbowShiftText* p_newNameText = nullptr;
	RainbowShiftText* p_newScoreText = nullptr;
	sf::Clock m_timer;
	AppDataPtr m_appData;
	GameDataPtr m_gameData;
	sf::Font* m_font, *m_titleFont;
	sf::Texture m_backgroundTexture;
	StarField m_starField;
	SoundRef m_clickSound;
	sf::RenderWindow& m_window;
};
