#include "TitleState.h"

#include <iostream>

#include "PlayState.h"
#include "DemoState.h"
#include "OptionsState.h"
#include "HighScoresState.h"


TitleState::TitleState(AppDataRef data, GameDataRef gameData) :
	MenuState(data, gameData), m_gameData(gameData)
{
#if _DEBUG
	std::cout << "STATE_MACHINE: TitleState constructed!\n";
#endif
}

TitleState::~TitleState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: TitleState destroyed!\n";
#endif
}

void TitleState::init()
{
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Stardust_Memories.ogg");
	musicMan.getMusic().setLoop(true);

	setDefaultColor(sf::Color::White);
	setHighlightColor(sf::Color(155, 100, 255));
	setTitle("(Asteroids!)", 200.0f, m_data->assets.getFont("arcadeItal"), sf::Text::Style::Italic,
		sf::Color(55, 155, 255), sf::Color(255, 255, 75), 130, .9f);
	setTopItemPos(m_window.getSize().y / 2);
	setLineSpacing(60);
	setItemAttribs(sf::Text::Bold, 80);
	addMenuItem("Start");
	addMenuItem("Options");
	addMenuItem("High Scores");

	m_starField.init(m_data);

}

void TitleState::cleanup()
{
}

void TitleState::pause()
{
}

void TitleState::resume()
{
	m_data->musicManager.play("Sounds/Stardust_Memories.ogg");
	m_data->musicManager.getMusic().setLoop(true);
	m_clock.restart();
}

void TitleState::onEscapePressed()
{
	m_data->running = false;
}

void TitleState::drawBackground()
{
	m_data->window.draw(m_starField);
}

void TitleState::updateImpl()
{
	if (m_clock.getElapsedTime().asSeconds() > TITLE_SCREEN_DELAY)
	{
		m_data->machine.addState(StateRef(std::make_unique<DemoState>(m_data, m_gameData)), false);
	}
	if (m_itemSelected != -1)
	{
		switch (m_itemSelected)
		{
			case 0:
				m_data->machine.addState(StateRef(std::make_unique<PlayState>(m_data, m_gameData)), false);
				break;
			case 1:
				m_data->machine.addState(StateRef(std::make_unique<OptionsState>(m_data, m_gameData)), false);
				break;
			case 2:
				m_data->machine.addState(StateRef(std::make_unique<HighScoresState>(m_data, m_gameData)), false);
				break;
			default:
				std::cerr << "Error selecting menu item!" << '\n';
		}
		m_itemSelected = -1;
	}
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
}
