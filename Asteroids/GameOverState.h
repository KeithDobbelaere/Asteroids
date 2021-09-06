#pragma once

#include "GameState.h"

#include <SFML/Graphics.hpp>

#include "Application.h"
#include "TextEffects.h"


class GameOverState : public GameState
{
public:
	GameOverState(AppDataPtr data, GameDataPtr gameData);
	~GameOverState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;

private:

	void updateEntities();
	void drawScene();
	void drawText();

private:
	AppDataPtr m_data;
	GameDataPtr m_gameData;

	sf::RenderWindow* m_window;
	sf::Font* m_font;
	sf::Text scoreText, livesText, specialText, gameOverText;

	PulsingText pressKeyText;
	sf::Sprite background;
	Animation explosionSprite, rockSprite, rockSmallSprite, blueBulletSprite, playerSprite, playerGoSprite;
	Animation explosionShipSprite, livesSprite, redBulletSprite, specialSprite, rapidFireSprite;
};
