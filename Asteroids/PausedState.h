#pragma once

#include <vector>

#include "MenuState.h"
#include "Application.h"
#include "StarField.h"


class PausedState : public MenuState
{
public:
	PausedState(AppDataRef data, GameDataRef gameData);
	~PausedState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void updateImpl() override;

	
private:
	void onEscapePressed() override;
	void drawBackground() override;

	GameDataRef m_gameData;
	StarField m_starField;
};

