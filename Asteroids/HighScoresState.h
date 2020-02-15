#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>

#include "GameState.h"
#include "Application.h"
#include "StarField.h"


class HighScoresState : public GameState
{
public:
	HighScoresState(AppDataRef data, GameDataRef gameData);
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
	void refreshScores(float posX, const sf::Color& color);
	void swapEntries(int indexA, int indexB);
	void slideLeft();
	void slideRight();

	float m_lineSpacing = 50.f, direction;
	int m_difficulty, alpha = 255, slideStep = 5, fadeInOut = -slideStep;
	int index1 = 11, index2 = 10;
	sf::Vector2f dest1, dest2;
	bool swapping = false, sorted = false, slide = false;
	sf::Color m_clearColor, m_textColor;
	sf::Text m_titleText, m_difficultyText, m_leftArrow, m_rightArrow, m_backText;
	std::vector<sf::Text> m_nameTexts;
	std::vector<sf::Text> m_scoreTexts;
	sf::Clock m_timer;
	AppDataRef m_data;
	GameDataRef m_gameData;
	sf::Font* m_font, *m_titleFont;
	sf::Texture m_backgroundTexture;
	StarField m_starField;
	SoundRef m_clickSound;
	sf::RenderWindow& m_window;
};
