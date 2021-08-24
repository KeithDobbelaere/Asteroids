#pragma once
#include <SFML/Graphics.hpp>

#include "MenuState.h"
#include "Application.h"
#include "StarField.h"

#include <vector>


class TitleState : public MenuState
{
public:
	TitleState(AppDataRef data, GameDataRef gameData);
	~TitleState();


	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void updateImpl() override;

private:
	void onEscapePressed() override;
	void drawBackground() override;

private:
	GameDataRef m_gameData;
	sf::Clock m_clock;
	StarField m_starField;
};
