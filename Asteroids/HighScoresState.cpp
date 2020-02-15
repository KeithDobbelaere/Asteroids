#include "HighScoresState.h"

#include <iostream>

#include "ScoresManager.h"
#include "TitleState.h"

HighScoresState::HighScoresState(AppDataRef data, GameDataRef gameData) :
	m_data(data), m_gameData(gameData), m_window(data->window), 
	m_font(m_data->assets.getFont("default")), m_titleFont(m_data->assets.getFont("arcadeBar"))
{
#if _DEBUG
	std::cout << "STATE_MACHINE: HighScoresState constructed!\n";
#endif
	m_data->assets.loadSoundBuffer("click", "sounds/click_sound.ogg");
	m_clickSound = m_data->assets.linkSoundRef("click");

	m_clearColor = sf::Color(1, 20, 51);
	m_textColor = sf::Color::Yellow;

	const auto& windowSize = m_window.getSize();
	m_titleText = sf::Text("(High·Scores)", *m_titleFont, 66);
	m_titleText.setLetterSpacing(.9f);
	m_titleText.setFillColor(m_textColor);
	m_titleText.setPosition(windowSize.x / 2 -
		m_titleText.getGlobalBounds().width / 2, 10);

	m_leftArrow = sf::Text("<", *m_font, 88);
	m_leftArrow.setFillColor(m_textColor);
	m_leftArrow.setPosition(20, windowSize.y / 2 -
		m_leftArrow.getGlobalBounds().height / 2);

	m_rightArrow = sf::Text(">", *m_font, 88);
	m_rightArrow.setFillColor(m_textColor);
	m_rightArrow.setPosition(windowSize.x - m_rightArrow.getGlobalBounds().width - 20,
		windowSize.y / 2 - m_rightArrow.getGlobalBounds().height / 2);

	m_backText = sf::Text("Back", *m_font, 56);
	m_backText.setFillColor(m_textColor);
	m_backText.setPosition(50, windowSize.y - m_backText.getGlobalBounds().height - 50);

	m_difficulty = m_data->difficulty;
}

HighScoresState::~HighScoresState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: HighScoresState destroyed!\n";
#endif
}


void HighScoresState::init()
{
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Stardust_Memories.ogg");
	musicMan.getMusic().setLoop(true);

	refreshDifficulty(m_textColor);
	refreshScores(360.f, m_textColor);

	if (m_data->newHighScore)
	{
		m_nameTexts.back().setString(m_data->playerInitials);
		auto& scoreText = m_scoreTexts.back();
		scoreText.setString(std::to_string(m_gameData->score));
		scoreText.setPosition(840.f - (scoreText.getLocalBounds().width +
			scoreText.getLocalBounds().left), scoreText.getPosition().y);
	}

	m_starField.init(m_data);
}

void HighScoresState::cleanup()
{
}

void HighScoresState::pause()
{
}

void HighScoresState::resume()
{
}

void HighScoresState::processInput()
{
	m_data->input.update(m_data->window);

	const auto& input = m_data->input;
	if (input.wasKeyPressed(sf::Keyboard::Escape) || m_data->input.wasKeyPressed(sf::Keyboard::Enter) ||
		input.wasItemClicked(m_backText, sf::Mouse::Button::Left, m_window))
	{
		m_clickSound->play();
		if (!m_data->newHighScore)
			m_data->machine.removeState();
	}	
	if (input.wasKeyPressed(sf::Keyboard::Left) ||
		input.wasItemClicked(m_leftArrow, sf::Mouse::Button::Left, m_window))
	{
		m_clickSound->play();
		slideLeft();
	}
	if (input.wasKeyPressed(sf::Keyboard::Right) ||
		input.wasItemClicked(m_rightArrow, sf::Mouse::Button::Left, m_window))
	{
		m_clickSound->play();
		slideRight();
	}
	if (input.isItemHovered(m_backText, m_window))
		m_backText.setFillColor(sf::Color::White);
	else
		m_backText.setFillColor(m_textColor);
	if (input.isItemHovered(m_leftArrow, m_window))
		m_leftArrow.setFillColor(sf::Color::White);
	else
		m_leftArrow.setFillColor(m_textColor);
	if (input.isItemHovered(m_rightArrow, m_window))
		m_rightArrow.setFillColor(sf::Color::White);
	else
		m_rightArrow.setFillColor(m_textColor);
}

void HighScoresState::update(float dt)
{
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
	if (!sorted)
	{
		if (swapping)
		{
			const float swapStep = 1.8f;
			m_nameTexts[index1].move(0.f, -swapStep);
			m_scoreTexts[index1].move(0.f, -swapStep);
			m_nameTexts[index2].move(0.f, swapStep);
			m_scoreTexts[index2].move(0.f, swapStep);
			sf::Vector2f position0 = m_scoreTexts[index1].getPosition();
			if (float difference = position0.y - dest1.y; difference < 0.1f)
			{
				swapping = false;
				m_nameTexts[index1].setPosition(360.f, dest1.y);
				m_scoreTexts[index1].setPosition(position0.x, dest1.y);
				sf::Vector2f position1 = m_scoreTexts[index2].getPosition();
				m_nameTexts[index2].setPosition(360.f, dest2.y);
				m_scoreTexts[index2].setPosition(position1.x, dest2.y);
				if (--index2 < 0)
					sorted = true;
			}
		}
		else if (int a = std::stoi(m_scoreTexts[index1].getString().toAnsiString()),
			b = std::stoi(m_scoreTexts[index2].getString().toAnsiString());
			a > b)
			swapEntries(index1, index2);
		else
			sorted = true;
	}
	else if (m_data->newHighScore)
	{
		m_data->highScores.addScore(ScoresManager::Entry(m_data->playerInitials, m_gameData->score), m_data->difficulty);
		m_data->newHighScore = false;
		m_data->backToTitle = true;
	}
	if (slide)
	{
		sf::Color fadeColor(m_textColor.r, m_textColor.g, m_textColor.b, alpha);
		m_difficultyText.setFillColor(fadeColor);
		for (int i = 0; i < 12; i++)
		{
			m_nameTexts[i].move(direction, 0);
			m_nameTexts[i].setOutlineColor(fadeColor);

			m_scoreTexts[i].move(direction, 0);
			m_scoreTexts[i].setOutlineColor(fadeColor);
		}
		alpha += fadeInOut;
		if (fadeInOut == -slideStep && alpha <= 0)
		{
			refreshDifficulty(fadeColor);
			refreshScores(direction < 0 ? 615.f : 105.f, fadeColor);
			fadeInOut = slideStep;
		}
		if (fadeInOut == slideStep && alpha >= 255)
		{
			refreshDifficulty(m_textColor);
			refreshScores(360.f, m_textColor);
			fadeInOut = -slideStep;
			slide = false;
		}
	}
}

void HighScoresState::draw(float dt)
{
	m_window.clear(m_clearColor);

	m_window.draw(m_starField);

	m_window.draw(m_titleText);
	m_window.draw(m_difficultyText);
	m_window.draw(m_leftArrow);
	m_window.draw(m_rightArrow);
	for (int i = 0; i < 12; i++)
	{
		m_window.draw(m_nameTexts[i]);
		m_window.draw(m_scoreTexts[i]);
	}
	m_window.draw(m_backText);
	m_window.display();
}

void HighScoresState::refreshDifficulty(const sf::Color& color)
{
	std::string difficultyStr;
	switch (m_difficulty)
	{
	case 1:
		difficultyStr = "Hard"; break;
	case 2:
		difficultyStr = "Medium"; break;
	case 3:
		difficultyStr = "Easy"; break;
	}
	m_difficultyText = sf::Text(difficultyStr, *m_font, 66);
	m_difficultyText.setFillColor(color);
	m_difficultyText.setPosition(m_data->window.getSize().x / 2 -
		m_difficultyText.getGlobalBounds().width / 2, 90);
}

void HighScoresState::refreshScores(float xPos, const sf::Color& color)
{
	m_nameTexts.clear();
	m_scoreTexts.clear();
	float yPosition = 160.f;
	for (int i = 0; i < 12; i++)
	{
		ScoresManager::Entry entry = m_data->highScores.getScore(i, m_difficulty);
		std::string nameStr = std::string(entry.initials);
		std::string scoreStr = std::to_string(entry.score);
		m_nameTexts.emplace_back(nameStr, *m_font, 56);
		auto& nameText = m_nameTexts.back();
		nameText.setPosition(xPos, yPosition);
		nameText.setFillColor(sf::Color::Transparent);
		nameText.setOutlineColor(color);
		nameText.setOutlineThickness(1.f);

		m_scoreTexts.emplace_back(scoreStr, *m_font, 56);
		auto& scoreText = m_scoreTexts.back();
		scoreText.setPosition(xPos + 480.f - (m_scoreTexts.back().getLocalBounds().width +
			scoreText.getLocalBounds().left), yPosition);
		scoreText.setFillColor(sf::Color::Transparent);
		scoreText.setOutlineColor(color);
		scoreText.setOutlineThickness(1.f);
		yPosition += m_lineSpacing;
	}
}

void HighScoresState::swapEntries(int indexA, int indexB)
{
	swapping = true;
	dest1 = m_scoreTexts[indexB].getPosition();
	dest2 = m_scoreTexts[indexA].getPosition();
}

void HighScoresState::slideLeft()
{
	if (!slide && sorted)
	{
		if (++m_difficulty < 4)
		{
			slide = true;
			direction = (float)slideStep;
		}
		else
			m_difficulty = 3;
	}
}

void HighScoresState::slideRight()
{
	if (!slide && sorted)
	{
		if (--m_difficulty > 0)
		{
			slide = true;
			direction = (float)-slideStep;
		}
		else
			m_difficulty = 1;
	}
}
