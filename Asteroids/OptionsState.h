#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Application.h"
#include "MenuState.h"
#include "StarField.h"


class OptionsState : public MenuState
{
public:
	OptionsState(AppDataPtr data, GameDataPtr gameData); 
	~OptionsState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void updateImpl() override;
	
private:
	Difficulty m_oldDifficultySetting, m_newDifficultySetting;
	void setDifficultyText();
	void onEscapePressed() override;
	void drawBackground() override;

private:
	sf::Text m_restartReqText;
	GameDataPtr m_gameData;
	StarField m_starField;
	SoundRef m_effectsSound;
};
