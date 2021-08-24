#include "GameOverState.h"

#include "Defines.h"
#include "Player.h"
#include "Powerups.h"
#include "Bullets.h"
#include "Asteroid.h"
#include "PausedState.h"
#include "ScoreEntryState.h"
#include "AssetManager.h"

#include <iostream>


GameOverState::GameOverState(AppDataRef data, GameDataRef gameData) :
	m_data(data), m_font(m_data->assets.getFont("default")), m_gameData(gameData)
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: GameOverState constructed!\n";
#	endif
	m_window = &m_data->window;
	scoreText.setFont(*m_font);
	scoreText.setPosition(0, 0);
	livesText.setFont(*m_font);
	specialText.setFont(*m_font);
	gameOverText = sf::Text("Game Over", *m_font, 100);
	gameOverText.setStyle(sf::Text::Bold);
	{
		sf::FloatRect bounds = gameOverText.getLocalBounds();
		gameOverText.setPosition(SCRN_WIDTH / 2 - bounds.width / 2, SCRN_HEIGHT / 2 - bounds.height / 2 - 50);
	}
	pressKeyText = sf::Text("Press ENTER to continue", *m_font);
	pressKeyText.setStyle(sf::Text::Bold);
	{
		sf::FloatRect bounds = pressKeyText.getLocalBounds();
		pressKeyText.setPosition(SCRN_WIDTH / 2 - bounds.width / 2, SCRN_HEIGHT - 50);
	}
	auto& assets = m_data->assets;
	assets.getTexture("ship").setSmooth(true);
	assets.getTexture(m_data->background.c_str()).setSmooth(true);

	background = sf::Sprite(assets.getTexture(m_data->background.c_str()));

	explosionSprite = Animation(assets.getTexture("explosion1"), { 0, 0, 256, 256 }, 48, 0.5f);
	rockSprite = Animation(assets.getTexture("rock_large"), { 0, 0, 64, 64 }, 16, 0.2f);
	rockSmallSprite = Animation(assets.getTexture("rock_small"), { 0, 0, 64, 64 }, 16, 0.2f);
	blueBulletSprite = Animation(assets.getTexture("fire_blue"), { 0, 0, 32, 64 }, 16, 0.8f);
	playerSprite = Animation(assets.getTexture("ship"), { 0, 0, 40, 40 }, 1, 0.f);
	playerGoSprite = Animation(assets.getTexture("ship"), { 40, 0, 40, 40 }, 1, 0.f);
	explosionShipSprite = Animation(assets.getTexture("explosion2"), { 0, 0, 192, 192 }, 64, 0.5f);
	livesSprite = Animation(assets.getTexture("lives"), { 0, 0, 64, 64 }, 1, 0.f);
	redBulletSprite = Animation(assets.getTexture("fire_red"), { 0, 0, 32, 64 }, 16, 0.8f);
	specialSprite = Animation(assets.getTexture("special"), { 0, 0, 64, 64 }, 1, 0.f);
	rapidFireSprite = Animation(assets.getTexture("rapid_fire"), { 0, 0, 64, 64 }, 1, 0.f);

}

GameOverState::~GameOverState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: GameOverState destroyed!\n";
#	endif
}


void GameOverState::init()
{
	auto& music = m_data->music;
	music.setMaxVolume(m_data->musicVolumeFactor * 100);
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);
	m_gameData->p->setGodMode();
}

void GameOverState::cleanup()
{
	m_gameData->p->godMode = false;
}

void GameOverState::pause()
{
}

void GameOverState::resume()
{
}

void GameOverState::processInput()
{
	Player* p = m_gameData->p.get();
	m_data->input.update(m_data->window, m_data->view);

	const auto& input = m_data->input;
	if (input.wasKeyPressed(sf::Keyboard::Key::Enter))
	{
		if (m_gameData->score > m_data->highScores.getLowest(m_data->difficulty).score)
		{
			m_data->machine.addState(StateRef(std::make_unique<ScoreEntryState>(m_data, m_gameData)), true);
		}
		else
		{
			m_data->restartRequired = true;
		}
	}
	if (input.wasKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_data->machine.addState(StateRef(std::make_unique<PausedState>(m_data, m_gameData)), false);
		return;
	}
	if (input.wasKeyPressed(sf::Keyboard::Key::S) ||
		input.wasKeyPressed(sf::Keyboard::Key::Down))
	{
		p->angle += 180;
	}
	if (input.isKeyDown(sf::Keyboard::Key::Right) ||
		input.isKeyDown(sf::Keyboard::Key::D)) p->angle += 3;
	if (input.isKeyDown(sf::Keyboard::Key::Left) ||
		input.isKeyDown(sf::Keyboard::Key::A))  p->angle -= 3;
	if (input.isKeyDown(sf::Keyboard::Key::Up) ||
		input.isKeyDown(sf::Keyboard::Key::W)) p->thrust = true;
	else p->thrust = false;
	if (p->angle > 360.0f)
		p->angle -= 360;
	if (p->angle < 0)
		p->angle += 360;
}

void GameOverState::update(float dt)
{
	if (m_data->restartRequired || m_data->backToTitle)
		m_data->machine.removeState();

	updateEntities();

	pressKeyText.update(0.05f);
	m_gameData->frameCounter++;
}

void GameOverState::draw(float dt)
{
	m_window->clear();
	m_window->setView(m_data->view);
	drawScene();
	drawText();
	m_window->display();
}

void GameOverState::updateEntities()
{
	Player* p = m_gameData->p.get();
	if (p->thrust)  p->anim = playerGoSprite;
	else   p->anim = playerSprite;

	auto& entities = m_gameData->entities;
	for (auto it = entities.begin(); it != entities.end();)
	{
		Entity* e = (*it).get();

		if (e->type == EntityType::Explosion)
		{
			if (e->anim.isEnd()) e->alive = false;
		}

		e->update();

		if (e->alive == false)
		{
			if (e->type == EntityType::Asteroid) m_gameData->asteroidCount--;
			it = entities.erase(it);
		}
		else it++;
	}
}

void GameOverState::drawScene()
{
	m_window->draw(background);
	for (auto rit = m_gameData->entities.crbegin(); rit != m_gameData->entities.crend(); ++rit)
	{
		auto e = (*rit).get();
		e->draw(*m_window);
	}
}

void GameOverState::drawText()
{
	scoreText.setString("Score: " + std::to_string(m_gameData->score));
	livesText.setString("Lives: " + std::to_string(m_gameData->p->getLivesRemaining()));
	sf::FloatRect bounds = livesText.getLocalBounds();
	livesText.setPosition(SCRN_WIDTH - bounds.width - 6, 0);
	specialText.setString("Special Weapons: " + std::to_string(m_gameData->p->getSpecialWeapons()));
	sf::FloatRect bounds2 = specialText.getLocalBounds();
	specialText.setPosition(SCRN_WIDTH - bounds2.width - 6, bounds.height + 8);

	m_window->draw(scoreText);
	m_window->draw(livesText);
	m_window->draw(specialText);

	m_window->draw(gameOverText);
	m_window->draw(pressKeyText);
}