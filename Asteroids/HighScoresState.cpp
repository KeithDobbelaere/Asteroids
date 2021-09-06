#include "HighScoresState.h"

#include "ScoresManager.h"
#include "TitleState.h"

#include <iostream>
#include <algorithm>


HighScoresState::HighScoresState(AppDataPtr data, GameDataPtr gameData) :
	m_data(data), m_gameData(gameData), m_window(data->window), 
	m_font(m_data->assets.getFont("default")), m_titleFont(m_data->assets.getFont("arcadeBar"))
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: HighScoresState constructed!\n";
#	endif
	m_clickSound = m_data->assets.linkSoundRef("click");

	m_clearColor = sf::Color(1, 20, 51);
	m_textColor = sf::Color::Yellow;

	m_titleText = sf::Text("(High·Scores)", *m_titleFont, 66);
	m_titleText.setLetterSpacing(.9f);
	m_titleText.setFillColor(m_textColor);
	m_titleText.setPosition(SCRN_WIDTH / 2.f -
		m_titleText.getGlobalBounds().width / 2.f, 10);

	m_leftArrow = sf::Text("<", *m_font, 88);
	m_leftArrow.setFillColor(m_textColor);
	m_leftArrow.setPosition(20, SCRN_HEIGHT / 2.f -
		m_leftArrow.getGlobalBounds().height / 2.f);

	m_rightArrow = sf::Text(">", *m_font, 88);
	m_rightArrow.setFillColor(m_textColor);
	m_rightArrow.setPosition(SCRN_WIDTH - m_rightArrow.getGlobalBounds().width - 20,
		SCRN_HEIGHT / 2.f - m_rightArrow.getGlobalBounds().height / 2);

	m_backText = sf::Text("Back", *m_font, 56);
	m_backText.setFillColor(m_textColor);
	m_backText.setPosition(50, SCRN_HEIGHT - m_backText.getGlobalBounds().height - 50);

	m_displayedDifficulty = static_cast<int>(m_data->difficulty) - 1;
}

HighScoresState::~HighScoresState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: HighScoresState destroyed!\n";
#	endif
}


void HighScoresState::init()
{
	auto& music = m_data->music;
	music.setMaxVolume(m_data->musicVolumeFactor * 100);
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);

	refreshDifficulty(m_textColor);
	for (int i = 0; i < 3; i++) {
		initializeScoresText(360.f, m_textColor, i);
	}
	m_nameTexts[1][11].effect(true);
	m_scoreTexts[1][11].effect(true);

	p_newNameText = &m_nameTexts[m_displayedDifficulty][11];
	p_newScoreText = &m_scoreTexts[m_displayedDifficulty][11];
	if (m_data->newHighScore)
	{
		p_newNameText->setString(m_data->highScores.playerInitials);
		p_newScoreText->setString(std::to_string(m_gameData->score));
		p_newScoreText->setPosition(840.f - (p_newScoreText->getLocalBounds().width +
			p_newScoreText->getLocalBounds().left), p_newScoreText->getPosition().y);
		p_newNameText->effect(true);
		p_newScoreText->effect(true);
	}

	m_starField.init(m_data);
}

void HighScoresState::cleanup()
{
	if (m_data->newHighScore)
	{
	m_data->highScores.addScore(ScoresManager::Entry(m_data->highScores.playerInitials, m_gameData->score), m_data->difficulty);
	m_data->newHighScore = false;
	m_data->backToTitle = true;
	}
}

void HighScoresState::pause()
{
}

void HighScoresState::resume()
{
}

void HighScoresState::processInput()
{
	m_data->input.update(m_data->window, m_data->view);

	const auto& input = m_data->input;
	if (input.wasKeyPressed(sf::Keyboard::Key::Escape) || m_data->input.wasKeyPressed(sf::Keyboard::Key::Enter) ||
		input.wasItemClicked(m_backText, sf::Mouse::Button::Left, m_window))
	{
		m_clickSound->play();
		if (sorted)
			m_data->machine.removeState();
	}	
	if (input.wasKeyPressed(sf::Keyboard::Key::Left) ||
		input.wasItemClicked(m_leftArrow, sf::Mouse::Button::Left, m_window))
	{
		m_clickSound->play();
		slideLeft();
	}
	if (input.wasKeyPressed(sf::Keyboard::Key::Right) ||
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
			p_newNameText->move(0.f, -swapStep);
			p_newScoreText->move(0.f, -swapStep);
			m_nameTexts[m_displayedDifficulty][m_swapIndex].move(0.f, swapStep);
			m_scoreTexts[m_displayedDifficulty][m_swapIndex].move(0.f, swapStep);
			sf::Vector2f position0 = p_newScoreText->getPosition();
			if (float difference = position0.y - dest1.y; difference < 0.1f)
			{
				swapping = false;
				p_newNameText->setPosition(360.f, dest1.y);
				p_newScoreText->setPosition(position0.x, dest1.y);
				sf::Vector2f position1 = m_scoreTexts[m_displayedDifficulty][m_swapIndex].getPosition();
				m_nameTexts[m_displayedDifficulty][m_swapIndex].setPosition(360.f, dest2.y);
				m_scoreTexts[m_displayedDifficulty][m_swapIndex].setPosition(position1.x, dest2.y);
				if (--m_swapIndex < 0)
					sorted = true;
			}
		}
		else if (int a = std::stoi(p_newScoreText->getString().toAnsiString()),
			b = std::stoi(m_scoreTexts[m_displayedDifficulty][m_swapIndex].getString().toAnsiString());
			a > b)
			swapEntries(m_swapIndex);
		else
			sorted = true;
	}
	if (slide)
	{
		auto& fadeColor = m_textColor;
		fadeColor.a = alpha;
		m_difficultyText.setFillColor(fadeColor);
		for (int i = 0; i < 12; i++)
		{
			m_nameTexts[m_displayedDifficulty][i].move(direction, 0);
			m_nameTexts[m_displayedDifficulty][i].setOutlineColor(fadeColor);

			m_scoreTexts[m_displayedDifficulty][i].move(direction, 0);
			m_scoreTexts[m_displayedDifficulty][i].setOutlineColor(fadeColor);
		}
		alpha += fadeInOut;
		if (fadeInOut == -slideStep && alpha <= 0)
		{
			if (direction > 0) {
				m_displayedDifficulty++;
			} else {
				--m_displayedDifficulty;
			}
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
	if (m_data->newHighScore)
	{
		p_newNameText->update(alpha);
		p_newScoreText->update(alpha);
	}
}

void HighScoresState::draw(float dt)
{
	m_window.clear(m_clearColor);
	m_window.setView(m_data->view);

	m_window.draw(m_starField);

	m_window.draw(m_titleText);
	m_window.draw(m_difficultyText);
	if (m_displayedDifficulty != 2)
		m_window.draw(m_leftArrow);
	if (m_displayedDifficulty != 0)
		m_window.draw(m_rightArrow);
	for (int i = 0; i < 12; i++)
	{
		m_window.draw(m_nameTexts[m_displayedDifficulty][i]);
		m_window.draw(m_scoreTexts[m_displayedDifficulty][i]);
	}
	m_window.draw(m_backText);
	m_window.display();
}

void HighScoresState::refreshDifficulty(const sf::Color& color)
{
	std::string difficultyStr;
	switch (m_displayedDifficulty)
	{
	case 0:
		difficultyStr = "Hard"; break;
	case 1:
		difficultyStr = "Medium"; break;
	case 2:
		difficultyStr = "Easy"; break;
	}
	m_difficultyText = sf::Text(difficultyStr, *m_font, 66);
	m_difficultyText.setFillColor(color);
	m_difficultyText.setPosition(SCRN_WIDTH / 2.f -
		m_difficultyText.getGlobalBounds().width / 2, 90);
}

void HighScoresState::refreshScores(float xPos, const sf::Color& color)
{
	for (int i = 0; i < 12; i++)
	{
		ScoresManager::Entry entry = m_data->highScores.getScore(i, m_displayedDifficulty);
		auto& nameText = m_nameTexts[m_displayedDifficulty][i];
		nameText.setPosition(xPos, nameText.getPosition().y);
		nameText.setOutlineColor(color);

		auto& scoreText = m_scoreTexts[m_displayedDifficulty][i];
		scoreText.setPosition(xPos + 480.f - (scoreText.getLocalBounds().width +
			scoreText.getLocalBounds().left), scoreText.getPosition().y);
		scoreText.setOutlineColor(color);
	}

}

void HighScoresState::initializeScoresText(float xPos, const sf::Color& color, int difficulty)
{
	float yPosition = 160.f;
	for (int i = 0; i < 12; i++)
	{
		ScoresManager::Entry entry = m_data->highScores.getScore(i, difficulty);
		std::string nameStr = std::string(entry.initials);
		std::string scoreStr = std::to_string(entry.score);
		auto& nameText = m_nameTexts[difficulty][i];
		nameText = { nameStr, *m_font, 56 };
		nameText.setPosition(xPos, yPosition);
		nameText.setFillColor(sf::Color::Transparent);
		nameText.setOutlineColor(color);
		nameText.setOutlineThickness(1.f);

		auto& scoreText = m_scoreTexts[difficulty][i];
		scoreText = { scoreStr, *m_font, 56 };
		scoreText.setPosition(xPos + 480.f - (scoreText.getLocalBounds().width +
			scoreText.getLocalBounds().left), yPosition);
		scoreText.setFillColor(sf::Color::Transparent);
		scoreText.setOutlineColor(color);
		scoreText.setOutlineThickness(1.f);
		yPosition += m_lineSpacing;
	}
}

void HighScoresState::swapEntries(int index)
{
	swapping = true;
	dest1 = m_scoreTexts[m_displayedDifficulty][index].getPosition();
	dest2 = p_newScoreText->getPosition();
}

void HighScoresState::slideLeft()
{
	if (!slide && sorted)
	{
		if (m_displayedDifficulty < 2)
		{
			slide = true;
			direction = (float)slideStep;
		}
	}
}

void HighScoresState::slideRight()
{
	if (!slide && sorted)
	{
		if (m_displayedDifficulty > 0)
		{
			slide = true;
			direction = (float)-slideStep;
		}
	}
}
