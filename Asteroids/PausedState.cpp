#include "PausedState.h"

#include <iostream>

#include "TitleState.h"
#include "OptionsState.h"

PausedState::PausedState(AppDataRef data, GameDataRef gameData) :
	m_gameData(gameData), MenuState(data, gameData)
{
#if _DEBUG
	std::cout << "STATE_MACHINE: PausedState constructed!\n";
#endif
}

PausedState::~PausedState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: PausedState destroyed!\n";
#endif
}

void PausedState::init()
{
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Stardust_Memories.ogg");
	musicMan.getMusic().setLoop(true);

	m_starField.init(m_data);
	setDefaultColor(sf::Color::Yellow);
	setHighlightColor(sf::Color::White);
	setTitle("(Paused)", 160.0f, m_data->assets.getFont("arcadeBar"), sf::Text::Style::Regular, sf::Color::Yellow, sf::Color::Transparent, 84, .9f);
	setTopItemPos(300);
	setLineSpacing(50);
	setItemAttribs(sf::Text::Bold | sf::Text::Italic, 62, 54);
	addMenuItem("Exit");
	addMenuItem("Resume");
	addMenuItem("Options");
	addMenuItem("Restart");
	addMenuItem("Quit to Main Menu");
}

void PausedState::cleanup()
{
}

void PausedState::pause()
{
}

void PausedState::resume()
{
}

void PausedState::onEscapePressed()
{
	m_data->machine.removeState();
}

void PausedState::drawBackground()
{
	m_data->window.draw(m_starField);
}

void PausedState::updateImpl()
{
	if (m_itemSelected != -1)
	{
		switch (m_itemSelected)
		{
		case 0:
			m_data->running = false;
			break;
		case 1:
			m_data->machine.removeState();
			break;
		case 2:
			m_data->machine.addState(StateRef(std::make_unique<OptionsState>(m_data, m_gameData)), false);
			break; 
		case 3:
			m_data->machine.removeState();
			m_data->restartRequired = true;
			break;
		case 4:
			//m_data->machine.addState(StateRef(std::make_unique<TitleState>(m_data, m_gameData)), true, true);
			m_data->machine.removeState();
			m_data->backToTitle = true;
			break;
		default:
			std::cerr << "Error selecting menu item!\n";
		}
		m_itemSelected = -1;
	}
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
}
