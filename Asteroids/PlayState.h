#pragma once

#include "GameState.h"

#include <SFML/Graphics.hpp>

#include <memory>

#include "AssetManager.h"
#include "Application.h"
#include "TextEffects.h"


class PlayState : public GameState
{
public:
	PlayState(AppDataRef data, GameDataRef gameData);
	~PlayState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;

protected:

	void handleCollisions();
	void addNewEntities();
	void updateEntities();
	void drawScene();
	void drawText();
	bool isCollide(Entity* a, Entity* b);
	void increaseScore(int amount);
	virtual void gameOver();

protected:
	template<class EntityType>
	std::shared_ptr<EntityType> addEntity(Animation &a, int x, int y, float angle = 0.0f, float radius = 1);
	template<class EntityType>
	void addPowerup(Animation &a);

protected:
	int fadeIn;
	AppDataRef m_data;
	GameDataRef m_gameData;

	sf::RenderWindow* m_window;
	sf::Font* m_font;
	sf::Text scoreText, livesText, specialText, transitionText;
#if _DEBUG
	sf::Text debugText;
#endif
	sf::Color transitionTextColor;
	sf::Sprite background;
	Animation explosionSprite, rockSprite, rockSmallSprite, blueBulletSprite, playerSprite, playerGoSprite;
	Animation explosionShipSprite, livesSprite, redBulletSprite, specialSprite, rapidFireSprite;

	SoundRef shipExplosionSound, basicPhaserSound, rapidPhaserSound, impactExplosionSound, specialWeaponSound;
	SoundRef livesUpSound, specialUpSound, rapidUpSound;

	PowerUpText powerUpText;
};


template<class EntityType>
inline std::shared_ptr<EntityType> PlayState::addEntity(Animation & a, int x, int y, float angle, float radius)
{
	auto e = std::make_shared<EntityType>();
	e->settings(a, x, y, angle, radius);
	m_gameData->entities.push_back(e);

	return e;
}

template<class EntityType>
inline void PlayState::addPowerup(Animation& a)
{
	(void)addEntity<EntityType>(a, rand() % (SCRN_WIDTH / 2) + SCRN_WIDTH / 4, rand() % (SCRN_HEIGHT / 2) + SCRN_HEIGHT / 4, (float)(rand() % 360), 28.0f);
}
