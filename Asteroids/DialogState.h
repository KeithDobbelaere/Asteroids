#pragma once

#include <SFML/Graphics.hpp>
#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Application.h"
#include "MenuState.h"
#include "StarField.h"


class DialogState : public MenuState
{
public:
	DialogState(AppDataPtr data, GameDataPtr gameData);
	~DialogState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void updateImpl() override;

private:
	void onEscapePressed() override;
	void drawBackground() override;

private:
	GameDataPtr m_gameData;
	StarField m_starField;
	SoundRef m_effectsSound;
};
