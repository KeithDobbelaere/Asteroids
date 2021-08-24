#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Application.h"


class SplashState : public GameState
{
public:
	SplashState(AppDataRef data, GameDataRef gameData);
	~SplashState();
	void init() override;
	void cleanup() override;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;

private:
	AppDataRef m_data;
	GameDataRef m_gameData;
	sf::Clock m_clock;
	sf::Sprite m_background;
	sf::RenderWindow& m_window;
};
