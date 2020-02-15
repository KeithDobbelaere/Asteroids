#pragma once
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Application.h"
#include "MenuState.h"
#include "StarField.h"


class OptionsState : public MenuState
{
public:
	OptionsState(AppDataRef data, GameDataRef gameData); 
	~OptionsState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void updateImpl() override;
	
private:
	int m_oldDifficultySetting, m_newDifficultySetting;
	void setDifficultyText();
	void onEscapePressed() override;
	void drawBackground() override;

private:
	sf::Text m_restartReqText;
	GameDataRef m_gameData;
	StarField m_starField;
};
