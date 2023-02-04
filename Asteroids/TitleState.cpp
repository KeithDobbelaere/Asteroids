#include "TitleState.h"

#include "PlayState.h"
#include "DemoState.h"
#include "OptionsState.h"
#include "HighScoresState.h"

#include <iostream>


TitleState::TitleState(AppDataPtr data, GameDataPtr gameData) :
	MenuState(data, gameData), m_gameData(gameData)
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: TitleState constructed!\n";
#	endif
}

TitleState::~TitleState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: TitleState destroyed!\n";
#	endif
}

void TitleState::init()
{
	auto& music = m_appData->music;
	music.setMaxVolume(m_appData->musicVolumeFactor * 100);
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);

	setDefaultColor(sf::Color::White);
	setHighlightColor(sf::Color(155, 100, 255));

	setTitle("(Asteroids!)", 200.0f, m_appData->assets.getFont("arcadeItal"), sf::Text::Style::Italic,
		sf::Color(55, 155, 255), sf::Color(255, 255, 75), 130, .9f);
	setTopItemPos(SCRN_HEIGHT / 2);
	setDefaultLineSpacing(10);
	setDefaultAttribs(sf::Text::Bold, 80);
	addMenuItem("Start");
	addMenuItem("Options");
	addMenuItem("High Scores");

	m_starField.init(m_appData);

}

void TitleState::cleanup()
{
}

void TitleState::pause()
{
}

void TitleState::resume()
{
	auto& music = m_appData->music;
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);
	m_clock.restart();
}

void TitleState::onEscapePressed()
{
	m_appData->running = false;
}

void TitleState::drawBackground()
{
	m_appData->window.draw(m_starField);
}

void TitleState::updateImpl()
{
	if (m_clock.getElapsedTime().asSeconds() > TITLE_SCREEN_DELAY)
	{
		m_appData->machine.addState(StatePtr(std::make_unique<DemoState>(m_appData, m_gameData)));
	}
	if (m_itemSelected != -1)
	{
		switch (m_itemSelected)
		{
			case 0:
				m_appData->machine.addState(StatePtr(std::make_unique<PlayState>(m_appData, m_gameData)));
				break;
			case 1:
				m_appData->machine.addState(StatePtr(std::make_unique<OptionsState>(m_appData, m_gameData)));
				break;
			case 2:
				m_appData->machine.addState(StatePtr(std::make_unique<HighScoresState>(m_appData, m_gameData)));
				break;
			default:
				std::cerr << "Error selecting menu item!" << '\n';
		}
		m_itemSelected = -1;
	}
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
}
