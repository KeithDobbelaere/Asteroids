#include "ScoreEntryState.h"

#include <iostream>

#include "PausedState.h"
#include "HighScoresState.h"

ScoreEntryState::ScoreEntryState(AppDataRef data, GameDataRef gameData) :
	m_data(data), m_gameData(gameData), m_window(data->window),
	m_font(m_data->assets.getFont("default")),
	m_titleFont(m_data->assets.getFont("arcadeBar")),
	m_initials("---")
{
#if _DEBUG
	std::cout << "STATE_MACHINE: ScoreEntryState constructed!\n";
#endif
	m_cursorPosition = m_lastCursorPos = 0;
	m_frameCounter = 0;
}

ScoreEntryState::~ScoreEntryState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: ScoreEntryState destroyed!\n";
#endif
}

void ScoreEntryState::init()
{
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Stardust_Memories.ogg");
	musicMan.getMusic().setLoop(true);

	const auto& windowSize = m_window.getSize();
	m_titleText = sf::Text(L"(New\u00a0High\u00a0Score!)", *m_titleFont, 90);
	m_titleText.setFillColor(sf::Color::Yellow);
	m_titleText.setPosition(windowSize.x / 2 -
		m_titleText.getGlobalBounds().width / 2, 100);

	m_instructionText = sf::Text("Enter initials:", *m_font, 60);
	m_instructionText.setFillColor(sf::Color::Yellow);
	m_instructionText.setPosition(windowSize.x / 2 -
		m_instructionText.getGlobalBounds().width / 2, 340);

	m_initialsText = sf::Text(m_initials, *m_font, m_characterSize);
	m_initialsText.setFillColor(sf::Color::Yellow);
	m_initialsText.setPosition(windowSize.x / 2 -
		m_initialsText.getGlobalBounds().width / 2, 400);

	m_cursorScreenPos = m_initialsText.findCharacterPos(m_cursorPosition);

	m_cursorText = sf::Text(m_initials[m_cursorPosition], *m_font, m_characterSize);
	m_cursorText.setPosition(m_cursorScreenPos);
	m_cursorText.setStyle(sf::Text::Style::Underlined);

	m_starField.init(m_data);
}

void ScoreEntryState::cleanup()
{

}

void ScoreEntryState::processInput()
{
	m_data->input.update(m_window);

	if (m_data->input.wasKeyPressed(sf::Keyboard::Key::Left))
	{
		m_cursorPosition = std::max(0, m_cursorPosition - 1);
	}
	else if (m_data->input.wasKeyPressed(sf::Keyboard::Right))
	{
		m_cursorPosition = std::min(3, m_cursorPosition + 1);
	}
	else if (m_data->input.wasKeyPressed(sf::Keyboard::Key::Enter))
	{
		strncpy_s(m_data->playerInitials, m_initials, strlen(m_initials));
		m_data->newHighScore = true;
		m_data->machine.addState(StateRef(std::make_unique<HighScoresState>(m_data, m_gameData)), true);
	}
	else if (m_data->input.wasKeyPressed(sf::Keyboard::Key::BackSpace))
	{
		if (m_cursorPosition > 0)
		{
			m_cursorPosition--;
			char* p = &m_initials[m_cursorPosition];
			memmove(p, p + 1, strlen(p));
			m_initials[3] = '\0';
			if (strlen(m_initials) < 3)
				m_initials[2] = '-';
		}
	}
	else if (uint8_t charTyped = m_data->input.getCharTyped(); charTyped >= 'A' && charTyped <= 'Z')
	{
		if (m_cursorPosition < 3)
		{
			char* p = &m_initials[m_cursorPosition];
			memmove(p + 1, p, strlen(p + 1));
			m_initials[3] = '\0';
			m_initials[m_cursorPosition] = charTyped;
			m_cursorPosition++;
		}
	}
}

void ScoreEntryState::update(float dt)
{
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
	if (m_lastCursorPos != m_cursorPosition)
	{
		m_initialsText.setString(m_initials);
		m_initialsText.setPosition(m_window.getSize().x / 2 -
			m_initialsText.getGlobalBounds().width / 2, 400);

		m_cursorScreenPos = m_initialsText.findCharacterPos(m_cursorPosition);
		m_cursorText.setPosition(m_cursorScreenPos);
		m_cursorText.setString(m_initials[m_cursorPosition] == '\0' ? ' ' : m_initials[m_cursorPosition]);

		m_lastCursorPos = m_cursorPosition;
	}
	bool blink = m_frameCounter % 60 > 30;
	m_cursorText.setFillColor(blink ? sf::Color::White : sf::Color::Transparent);
	m_frameCounter++;
}

void ScoreEntryState::draw(float dt)
{
	m_window.clear();
	m_window.draw(m_starField);
	m_window.draw(m_titleText);
	m_window.draw(m_instructionText);
	m_window.draw(m_cursorText);
	m_window.draw(m_initialsText);
	m_window.display();
}
