﻿#include "OptionsState.h"

#include "CustomControlsState.h"

#include <iostream>
#include <string>


OptionsState::OptionsState(AppDataPtr data, GameDataPtr gameData) :
	MenuState(data, gameData), m_gameData(gameData)
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: OptionsState constructed!\n";
#	endif

	auto& assets = m_appData->assets;
	m_effectsSound = assets.linkSoundRef("basic_phaser", 25.0f);
	m_oldDifficultySetting = m_newDifficultySetting = m_appData->difficulty;
}

OptionsState::~OptionsState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: OptionsState destroyed!\n";
#	endif
}

void OptionsState::init()
{
	auto& music = m_appData->music;
	music.setMaxVolume(m_appData->musicVolumeFactor * 100);
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);

	setDefaultColor(sf::Color::Yellow);
	setHighlightColor(sf::Color::White);
	setTitle("(Options)", 60.0f, m_appData->assets.getFont("arcadeBar"), sf::Text::Style::Regular, sf::Color::Yellow, sf::Color::Transparent, 84, .9f);
	setTopItemPos(200);
	setDefaultLineSpacing(7);
	setDefaultAttribs(sf::Text::Bold | sf::Text::Italic, 62, 54);
	addMenuItem("Difficulty:");
	addSubItem("Hard", true);
	addSubItem("Medium", true);
	addSubItem("Easy", true);
	addMenuItem("Default Controls");
	setDefaultAttribs(sf::Text::Bold | sf::Text::Italic, 62, 30);
	addSubItem(L"\u25b2/W = Thrust    \u25bc/S = Reverse Direction", false);
	addSubItem(L"\u25c4/A = Rotate Left   \u25ba/D = Rotate Right", false);
	addSubItem("SPACE = Fire Weapon    SHIFT = Fire Special Weapon", false);
	setDefaultAttribs(sf::Text::Bold | sf::Text::Italic, 62, 54);
	addMenuItem("Customize Controls");
	addMenuItem("Music Volume");
	addSlider(m_appData->musicVolumeFactor);
	addMenuItem("Sound Volume");
	addSlider(m_appData->soundVolumeFactor);
	addMenuItem("Back");
	
	m_restartReqText = sf::Text("*Restart Required", *m_appData->assets.getFont("default"));
	m_restartReqText.setPosition(20.f, (float)SCRN_HEIGHT - 50.f);
	m_restartReqText.setFillColor(sf::Color::Transparent);

	setDifficultyText();

	m_starField.init(m_appData);
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
				m_newDifficultySetting = static_cast<Difficulty>(1 + m_subItemSelected);
				setDifficultyText();
				m_menuItems[0].close();
			}
			break;
		case 1:
			break;
		case 2:
			m_appData->machine.addState(StatePtr(std::make_unique<CustomControlsState>(m_appData, m_gameData)));
			break;
		case 3:
			m_appData->music.setMaxVolume(m_appData->musicVolumeFactor * 100);
			break;
		case 4:
			if (m_subItemSelected != -1)
			{
				m_appData->assets.adjustSoundVolume(m_appData->soundVolumeFactor);
				m_effectsSound->play();
			}
			break;
		case 5:
			if (m_oldDifficultySetting != m_newDifficultySetting)
			{
				m_appData->difficulty = m_newDifficultySetting;
				m_appData->restartRequired = true;
			}
			m_appData->machine.removeState();
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
	case Difficulty::Hard:
		diffString = "Difficulty: Hard";
		break;
	case Difficulty::Medium:
		diffString = "Difficulty: Medium";
		break;
	case Difficulty::Easy:
		diffString = "Difficulty: Easy";
		break;
	}
	bool restartRequired = (m_newDifficultySetting != m_oldDifficultySetting);
	if (restartRequired && m_gameData->p.use_count() != 0 && m_gameData->p->getLivesRemaining() != 0)
	{
		diffString += '*';
		m_restartReqText.setFillColor(sf::Color::Yellow);
	}
	else
		m_restartReqText.setFillColor(sf::Color::Transparent);
	m_menuItems[0].setString(diffString);
	MenuItem::centerItem(m_menuItems[0]);
}

void OptionsState::onEscapePressed()
{
	m_appData->machine.removeState();
}

void OptionsState::drawBackground()
{
	m_appData->window.draw(m_starField);
	m_appData->window.draw(m_restartReqText);
}
