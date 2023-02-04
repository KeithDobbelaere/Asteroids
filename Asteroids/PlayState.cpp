#include "PlayState.h"

#include "Defines.h"
#include "Player.h"
#include "Powerups.h"
#include "Bullets.h"
#include "Asteroid.h"
#include "PausedState.h"
#include "GameOverState.h"

#include <iostream>


PlayState::PlayState(AppDataPtr data, GameDataPtr gameData) :
	m_appData(data), m_font(m_appData->assets.getFont("default")), powerUpText(*m_font), m_gameData(gameData),
	transitionText("Ready", *m_font, 80), scoreText("", *m_font, 40), livesText("", *m_font, 40),
	specialText("", *m_font, 40)
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: PlayState constructed!\n";
#	endif
	auto& music = m_appData->music;
	music.setMaxVolume(m_appData->musicVolumeFactor * 100);
	music.play("Sounds/Nearly_There.ogg");
	music.getCurrent().setLoop(true);
	m_window = &m_appData->window;
	scoreText.setPosition(0, 0);
#	if _DEBUG
		debugText = sf::Text("**Debug**", *m_font, 16);
		debugText.setPosition(0, SCRN_HEIGHT - 200);
#	endif

	auto& assets = m_appData->assets;
	explosionSprite = Animation(assets.getTexture("explosion1"), { 0, 0, 256, 256 }, 48, 0.5f);
	rockSprite = Animation(assets.getTexture("rock_large"), { 0, 0, 64, 64 }, 16, 0.2f);
	rockSmallSprite = Animation(assets.getTexture("rock_small"), { 0, 0, 64, 64 }, 16, 0.2f);
	blueBulletSprite = Animation(assets.getTexture("fire_blue"), { 0, 0, 32, 64 }, 16, 0.8f);
	playerSprite = Animation(assets.getTexture("ship"), { 0, 0, 40, 40 }, 1, 0.f);
	playerGoSprite = Animation(assets.getTexture("ship"), { 40, 0, 40, 40 }, 1, 0.f);
	explosionShipSprite = Animation(assets.getTexture("explosion2"), { 0, 0, 192, 192 }, 64, 0.5f);
	livesSprite = Animation(assets.getTexture("lives"), { 0, 0, 64, 64 }, 1, 0.0f);
	redBulletSprite = Animation(assets.getTexture("fire_red"), { 0, 0, 32, 64 }, 16, 0.8f);
	specialSprite = Animation(assets.getTexture("special"), { 0, 0, 64, 64 }, 1, 0.0f);
	rapidFireSprite = Animation(assets.getTexture("rapid_fire"), { 0, 0, 64, 64 }, 1, 0.0f);

	shipExplosionSound = assets.linkSoundRef("ship_explosion", 40.0f);
	basicPhaserSound = assets.linkSoundRef("basic_phaser", 25.0f);
	rapidPhaserSound = assets.linkSoundRef("rapid_phaser", 25.0f);
	impactExplosionSound = assets.linkSoundRef("impact", 25.0f);
	specialWeaponSound = assets.linkSoundRef("special_weapon", 35.0f);
	livesUpSound = assets.linkSoundRef("power_up2", 30.0f);
	specialUpSound = assets.linkSoundRef("power_up1", 30.0f);
	rapidUpSound = assets.linkSoundRef("power_up3", 30.0f);
	assets.adjustSoundVolume(m_appData->soundVolumeFactor);
}

PlayState::~PlayState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: PlayState destroyed!\n";
#	endif
	if (m_gameData->p.use_count() != 0)
		m_gameData->p->livesRemaining = 0;
}


void PlayState::init()
{
	m_gameData->asteroidCount = 0;

	m_gameData->entities.clear();
	m_gameData->p = std::make_shared<Player>();
	m_gameData->p->settings(playerSprite, SCRN_WIDTH / 2, SCRN_HEIGHT / 2, 0, 20.0f);
	m_gameData->entities.push_back(m_gameData->p);

	for (int i = 0; i < 12; i++)
	{
		addEntity<Asteroid>(rockSprite, rand() % SCRN_WIDTH, rand() % SCRN_HEIGHT, (float)(rand() % 360), 25.0f);
		m_gameData->asteroidCount++;
	}

	m_gameData->maxAsteroids = 30;
	m_gameData->asteroidProbability = 200;
	m_gameData->score = 0;
	m_gameData->stage = 0;
	m_gameData->frameCounter = 0;

	m_appData->background = std::string("background") + std::to_string(rand() % 3);
	background = sf::Sprite(m_appData->assets.getTexture(m_appData->background.c_str()));

	transitionText.reset("Ready");
	fadeIn = 255;
}

void PlayState::cleanup()
{
}

void PlayState::pause()
{
}

void PlayState::resume()
{
	auto& music = m_appData->music;
	music.setMaxVolume(m_appData->musicVolumeFactor * 100);
	music.play("Sounds/Nearly_There.ogg");
	music.getCurrent().setLoop(true);
}

void PlayState::processInput()
{
	Player* p = m_gameData->p.get();

	m_appData->input.update(m_appData->window, m_appData->view);
	const auto& input = m_appData->input;
	const auto& controls = m_appData->controls;
	if (input.wasKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_appData->machine.addState(StatePtr(std::make_unique<PausedState>(m_appData, m_gameData)));
		return;
	}
	if (input.wasKeyPressed(controls.key(Control::Reverse1)) ||
		input.wasKeyPressed(controls.key(Control::Reverse2)))
	{
		p->angle += 180;
	}
	if (input.wasKeyPressed(controls.key(Control::Fire_Special1)) ||
		input.wasKeyPressed(controls.key(Control::Fire_Special2)))
	{
		if (m_gameData->p->getSpecialWeapons() > 0)
		{
			specialWeaponSound->play();
			for (int angle = 0; angle < 360; angle += 4)
			{
				addEntity<SidewinderBullet>(redBulletSprite, (int)p->x, (int)p->y, (float)angle, 10.0f);
			}
			m_gameData->p->fireSpecialWeapon();
		}
	}
	if (m_gameData->p->hasRapidFire())
	{
		if (input.isKeyDown(controls.key(Control::Fire_Weapon1)) ||
			input.isKeyDown(controls.key(Control::Fire_Weapon2)))
		{
			if (m_gameData->frameCounter % FIRE_RATE_DELAY == 0)
			{
				rapidPhaserSound->play();
				addEntity<SidewinderBullet>(redBulletSprite, (int)p->x, (int)p->y, p->angle, 10.0f);
			}
		}
	}
	else if (input.wasKeyPressed(controls.key(Control::Fire_Weapon1)) ||
		input.wasKeyPressed(controls.key(Control::Fire_Weapon2)))
	{
		basicPhaserSound->play();
		addEntity<StandardBullet>(blueBulletSprite, (int)p->x, (int)p->y, p->angle, 10.0f);
	}
	if (input.isKeyDown(controls.key(Control::Right1)) ||
		input.isKeyDown(controls.key(Control::Right2))) p->angle += 3;
	if (input.isKeyDown(controls.key(Control::Left1)) ||
		input.isKeyDown(controls.key(Control::Left2)))  p->angle -= 3;
	if (input.isKeyDown(controls.key(Control::Thrust1)) ||
		input.isKeyDown(controls.key(Control::Thrust2))) p->thrust = true;
	else p->thrust = false;
	if (p->angle > 360.0f)
		p->angle -= 360;
	if (p->angle < 0)
		p->angle += 360;
}

void PlayState::update(float dt)
{
	if (m_appData->restartRequired)
	{
		init();
		m_appData->restartRequired = false;
	}
	if (m_appData->backToTitle)
	{
		m_appData->machine.removeState();
		m_appData->backToTitle = false;
	}
	handleCollisions();
	addNewEntities();
	updateEntities();
	powerUpText.update();
	if (fadeIn > 0)
		fadeIn -= 5;
	m_gameData->frameCounter++;
}

void PlayState::draw(float dt)
{
	m_window->clear();
	m_window->setView(m_appData->view);

	drawScene();
	drawText();
	if (fadeIn > 0)
	{
		sf::RectangleShape shape(sf::Vector2f(SCRN_WIDTH, SCRN_HEIGHT));
		shape.setFillColor(sf::Color(255, 255, 255, fadeIn));
		m_window->draw(shape);
	}
	m_window->display();
}

void PlayState::handleCollisions()
{
	for (const auto& a : m_gameData->entities)
	{
		for (const auto& b : m_gameData->entities)
		{
			if (a->alive)
			{
				if (a->collideWith(b.get()))
				{
					b->alive = false;
					switch (b->type)
					{
						case EntityType::Bullet:
						{
							addEntity<Explosion>(explosionSprite, (int)a->x, (int)a->y);
							impactExplosionSound->play();
							for (int i = 0; i < 2; i++)
							{
								if (a->R == 15) continue;

								addEntity<Asteroid>(rockSmallSprite, (int)a->x, (int)a->y, (float)(rand() % 360), 15);
								m_gameData->asteroidCount++;
							}
							increaseScore(160);
							break;
						}
						case EntityType::Asteroid:
						{
							addEntity<Explosion>(explosionShipSprite, (int)a->x, (int)a->y);
							shipExplosionSound->play();
							m_gameData->p->reset();

							if (m_gameData->p->livesRemaining == 0)
								gameOver();
							break;
						}
						case EntityType::RapidFire:
							rapidUpSound->play();
							increaseScore(480);
							powerUpText.add("+480", (int)b->x, (int)b->y);
							break;
						case EntityType::AddLife:
							livesUpSound->play();
							increaseScore(800);
							powerUpText.add("+800", (int)b->x, (int)b->y);
							break;
						case EntityType::AddSpecial:
							specialUpSound->play();
							increaseScore(640);
							powerUpText.add("+640", (int)b->x, (int)b->y);
					}
				}
			}
		}
	}
}

void PlayState::addNewEntities()
{
	int random = rand();
	if (random % (m_gameData->asteroidProbability) == 0 && m_gameData->asteroidCount < m_gameData->maxAsteroids)
	{
		addEntity<Asteroid>(rockSprite, 0, rand() % SCRN_HEIGHT, (float)(rand() % 360), 25);
		m_gameData->asteroidCount++;
	}
	if (random % 1800 == 0)
	{
		addPowerup<RapidFire>(rapidFireSprite);
	}
	else if (random % 2000 == 0)
	{
		addPowerup<AddSpecial>(specialSprite);
	}
	else if (random % 2500 == 0)
	{
		addPowerup<AddLife>(livesSprite);
	}
}

void PlayState::updateEntities()
{
	Player* p = m_gameData->p.get();
	if (p->thrust)  p->anim = playerGoSprite;
	else   p->anim = playerSprite;

	auto& entities = m_gameData->entities;
	for (auto it = entities.begin(); it != entities.end();)
	{
		Entity* e = (*it).get();

		e->update();

		if (e->alive == false)
		{
			if (e->type == EntityType::Asteroid) m_gameData->asteroidCount--;
			it = entities.erase(it);
		}
		else it++;
	}
}

void PlayState::drawScene()
{
	m_window->draw(background);
	for (auto rit = m_gameData->entities.crbegin(); rit != m_gameData->entities.crend(); ++rit)
	{
		auto e = (*rit).get();
		e->draw(*m_window);
	}
}

void PlayState::drawText()
{
	Player* p = m_gameData->p.get();
	scoreText.setString("Score: " + std::to_string(m_gameData->score));
	livesText.setString("Lives: " + std::to_string(p->getLivesRemaining()));
	sf::FloatRect bounds = livesText.getLocalBounds();
	livesText.setPosition(SCRN_WIDTH - bounds.width - 6, 0);
	specialText.setString("Special Weapons: " + std::to_string(p->getSpecialWeapons()));
	sf::FloatRect bounds2 = specialText.getLocalBounds();
	specialText.setPosition(SCRN_WIDTH - bounds2.width - 6, bounds.height + 8);

#	if _DEBUG
		debugText.setString("**Debug**\n\nasteroidCount: " +
			std::to_string(m_gameData->asteroidCount) + '/' +
			std::to_string(m_gameData->maxAsteroids) + "\ninvincible: " +
			std::to_string(p->isInvincible()) + " | secs: " +
			std::to_string(p->getCountdown() / TARGET_FRAME_RATE) + "\nstage: " +
			std::to_string(m_gameData->stage) + "\nspecialWeapons: " +
			std::to_string(p->getSpecialWeapons()) + "\nrapidFirePerk: " +
			std::to_string(p->hasRapidFire()) + "\nframeCounter: " +
			std::to_string(m_gameData->frameCounter) + "\nentities.size(): " +
			std::to_string(m_gameData->entities.size()));
		m_window->draw(debugText);
#	endif

	m_window->draw(scoreText);
	m_window->draw(livesText);
	m_window->draw(specialText);

	if (transitionText.isAlive())
	{
		transitionText.update();
		m_window->draw(transitionText);
	}
	m_window->draw(powerUpText);
}

void PlayState::increaseScore(int amount)
{
	m_gameData->score += amount;
	if (m_gameData->score % (STAGE_INTERVAL * static_cast<int>(m_appData->difficulty)) == 0)
	{
		m_gameData->stage++;
		m_gameData->maxAsteroids += 6;
		m_gameData->asteroidProbability = std::max(200 - m_gameData->stage * 36, 20);
		transitionText.reset("Stage " + std::to_string(m_gameData->stage + 1));
		m_gameData->p->setInvincible(TRANSITION_DELAY);
	}
}

void PlayState::gameOver()
{
	m_appData->machine.addState(StatePtr(std::make_unique<GameOverState>(m_appData, m_gameData)));
}
