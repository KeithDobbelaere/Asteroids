#include "OptionsState.h"

#include <iostream>
#include <string>


OptionsState::OptionsState(AppDataRef data, GameDataRef gameData) :
	MenuState(data, gameData), m_gameData(gameData)
{
#if _DEBUG
	std::cout << "STATE_MACHINE: OptionsState constructed!\n";
#endif
	m_oldDifficultySetting = m_newDifficultySetting = m_data->difficulty;
}

OptionsState::~OptionsState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: OptionsState destroyed!\n";
#endif
}

void OptionsState::init()
{
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Stardust_Memories.ogg");
	musicMan.getMusic().setLoop(true);

	setDefaultColor(sf::Color::Yellow);
	setHighlightColor(sf::Color::White);
	setTitle("(Options)", 60.0f, m_data->assets.getFont("arcadeBar"), sf::Text::Style::Regular, sf::Color::Yellow, sf::Color::Transparent, 84, .9f);
	setTopItemPos(200);
	setLineSpacing(50);
	setItemAttribs(sf::Text::Bold | sf::Text::Italic, 62, 54);
	addMenuItem("Difficulty:");
	addSubItem("Hard", true);
	addSubItem("Medium", true);
	addSubItem("Easy", true);
	addMenuItem("Controls");
	setItemAttribs(sf::Text::Bold | sf::Text::Italic, 62, 30);
	addSubItem(L"\u25b2/W = Thrust    \u25bc/S = Reverse Direction", false);
	addSubItem(L"\u25c4/A = Rotate Left   \u25ba/D = Rotate Right", false);
	addSubItem("SPACE = Fire Weapon    SHIFT = Fire Special Weapon", false);
	setItemAttribs(sf::Text::Bold | sf::Text::Italic, 62, 54);
	addMenuItem("Music Volume");
	addSlider(m_data->musicVolumeFactor);
	addMenuItem("Sound Volume");
	addSlider(m_data->soundVolumeFactor);
	addMenuItem("Back");
	
	m_restartReqText = sf::Text("*Restart Required", *m_data->assets.getFont("default"));
	m_restartReqText.setPosition(20.f, (float)SCRN_HEIGHT - 50.f);
	m_restartReqText.setFillColor(sf::Color::Transparent);

	setDifficultyText();

	m_starField.init(m_data);
}

void OptionsState::cleanup()
{
}

void OptionsState::pause()
{
}

void OptionsState::resume()
{
}

void OptionsState::updateImpl()
{
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
	if (m_itemSelected != -1)
	{
		switch (m_itemSelected)
		{
		case 0:
			if (m_subItemSelected != -1)
			{
				m_newDifficultySetting = 1 + m_subItemSelected;
				setDifficultyText();
				m_menuItems[0].subMenuOpened = false;
			}
			break;
		case 1:
			break;
		case 2:
			m_data->musicManager.setMaxVolume(m_data->musicVolumeFactor * 100);
			break;
		case 3:
			if (m_subItemSelected != -1)
				m_data->assets.adjustSoundVolume(m_data->soundVolumeFactor);
			break;
		case 4:
			if (m_oldDifficultySetting != m_newDifficultySetting)
			{
				m_data->difficulty = m_newDifficultySetting;
				m_data->restartRequired = true;
			}
			m_data->machine.removeState();
			break;
		default:
			std::cerr << "Error selecting menu item!" << '\n';
		}
		m_itemSelected = -1;
		m_subItemSelected = -1;
	}
}

void OptionsState::setDifficultyText()
{
	std::string diffString;
	switch (m_newDifficultySetting)
	{
	case 1:
		diffString = "Difficulty: Hard";
		break;
	case 2:
		diffString = "Difficulty: Medium";
		break;
	case 3:
		diffString = "Difficulty: Easy";
		break;
	}
	bool restartRequired = (m_newDifficultySetting != m_oldDifficultySetting);
	if (restartRequired && m_gameData->livesRemaining != 0)
	{
		diffString += '*';
		m_restartReqText.setFillColor(sf::Color::Yellow);
	}
	else
		m_restartReqText.setFillColor(sf::Color::Transparent);
	m_menuItems[0].text.setString(diffString);
	centerItem(m_menuItems[0].text);
}

void OptionsState::onEscapePressed()
{
	m_data->machine.removeState();
}

void OptionsState::drawBackground()
{
	m_data->window.draw(m_starField);
	m_data->window.draw(m_restartReqText);
}
