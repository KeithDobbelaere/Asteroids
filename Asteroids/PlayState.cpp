#include "PlayState.h"

#include <iostream>

#include "Defines.h"
#include "Player.h"
#include "Powerups.h"
#include "Bullets.h"
#include "Asteroid.h"
#include "PausedState.h"
#include "GameOverState.h"


PlayState::PlayState(AppDataRef data, GameDataRef gameData) :
	m_data(data), m_font(m_data->assets.getFont("default")), powerUpText(*m_font), m_gameData(gameData)
{
#if _DEBUG
	std::cout << "STATE_MACHINE: PlayState constructed!\n";
#endif
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Nearly_There.ogg");
	musicMan.getMusic().setLoop(true);
	m_window = &m_data->window;
	scoreText.setFont(*m_font);
	scoreText.setCharacterSize(40);
	scoreText.setPosition(0, 0);
	livesText.setFont(*m_font);
	livesText.setCharacterSize(40);
	specialText.setFont(*m_font);
	specialText.setCharacterSize(40);
	transitionText = sf::Text("Ready", *m_font, 80);
	transitionText.setStyle(sf::Text::Bold);
	const auto& bounds = transitionText.getLocalBounds();
	transitionText.setPosition(SCRN_WIDTH / 2 - bounds.width / 2, SCRN_HEIGHT / 2 - bounds.height / 2 - 50);
	transitionTextColor = transitionText.getFillColor();
#if _DEBUG
	debugText = sf::Text("**Debug**", *m_font, 16);
	debugText.setPosition(0, SCRN_HEIGHT - 200);
#endif
	auto& assets = m_data->assets;
	assets.loadTexture("ship", "images/spaceship.png");
	assets.loadTexture("background", "images/galaxy-4.png");
	assets.loadTexture("explosion1", "images/explosions/type_C.png");
	assets.loadTexture("rock_large", "images/rock.png");
	assets.loadTexture("fire_blue", "images/fire_blue.png");
	assets.loadTexture("rock_small", "images/rock_small.png");
	assets.loadTexture("explosion2", "images/explosions/type_B.png");
	assets.loadTexture("lives", "images/lives.png");
	assets.loadTexture("fire_red", "images/fire_red.png");
	assets.loadTexture("special", "images/special.png");
	assets.loadTexture("rapid_fire", "images/rapid_fire.png");


	assets.getTexture("ship").setSmooth(true);
	assets.getTexture("background").setSmooth(true);

	background = sf::Sprite(assets.getTexture("background"));
	
	explosionSprite = Animation(assets.getTexture("explosion1"), 0, 0, 256, 256, 48, 0.5f);
	rockSprite = Animation(assets.getTexture("rock_large"), 0, 0, 64, 64, 16, 0.2f);
	rockSmallSprite = Animation(assets.getTexture("rock_small"), 0, 0, 64, 64, 16, 0.2f);
	blueBulletSprite = Animation(assets.getTexture("fire_blue"), 0, 0, 32, 64, 16, 0.8f);
	playerSprite = Animation(assets.getTexture("ship"), 0, 0, 40, 40, 1, 0.f);
	playerGoSprite = Animation(assets.getTexture("ship"), 40, 0, 40, 40, 1, 0.0f);
	explosionShipSprite = Animation(assets.getTexture("explosion2"), 0, 0, 192, 192, 64, 0.5f);
	livesSprite = Animation(assets.getTexture("lives"), 0, 0, 64, 64, 1, 0.0f);
	redBulletSprite = Animation(assets.getTexture("fire_red"), 0, 0, 32, 64, 16, 0.8f);
	specialSprite = Animation(assets.getTexture("special"), 0, 0, 64, 64, 1, 0.0f);
	rapidFireSprite = Animation(assets.getTexture("rapid_fire"), 0, 0, 64, 64, 1, 0.0f);

	assets.loadSoundBuffer("Ship_Explosion", "Sounds/Ship_Explosion.wav");
	assets.loadSoundBuffer("Basic_Phaser", "Sounds/Basic_Phaser.ogg");
	assets.loadSoundBuffer("Rapid_Phaser", "Sounds/Rapid_Phaser.ogg");
	assets.loadSoundBuffer("Impact", "Sounds/Impact.wav");
	assets.loadSoundBuffer("Special_Weapon", "Sounds/Special_Weapon.ogg");
	assets.loadSoundBuffer("Power_Up1", "Sounds/Power_Up1.ogg");
	assets.loadSoundBuffer("Power_Up2", "Sounds/Power_Up2.ogg");
	assets.loadSoundBuffer("Power_Up3", "Sounds/Power_Up3.ogg");

	shipExplosionSound = assets.linkSoundRef("Ship_Explosion", 40.0f);
	basicPhaserSound = assets.linkSoundRef("Basic_Phaser", 25.0f);
	rapidPhaserSound = assets.linkSoundRef("Rapid_Phaser", 25.0f);
	impactExplosionSound = assets.linkSoundRef("Impact", 25.0f);
	specialWeaponSound = assets.linkSoundRef("Special_Weapon", 35.0f);
	livesUpSound = assets.linkSoundRef("Power_Up2", 30.0f);
	specialUpSound = assets.linkSoundRef("Power_Up1", 30.0f);
	rapidUpSound = assets.linkSoundRef("Power_Up3", 30.0f);
	assets.adjustSoundVolume(m_data->soundVolumeFactor);
}

PlayState::~PlayState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: PlayState destroyed!\n";
#endif
	m_gameData->livesRemaining = 0;
}


void PlayState::init()
{
	m_gameData->asteroidCount = 0;

	m_gameData->entities.clear();
	m_gameData->p = addEntity<Player>(playerSprite, SCRN_WIDTH / 2, SCRN_HEIGHT / 2, 0, 20.0f);

	for (int i = 0; i < 12; i++)
	{
		(void)addEntity<Asteroid>(rockSprite, rand() % SCRN_WIDTH, rand() % SCRN_HEIGHT, (float)(rand() % 360), 25.0f);
		m_gameData->asteroidCount++;
	}

	m_gameData->maxAsteroids = 30;
	m_gameData->asteroidProbability = 200;
	m_gameData->score = 0;
	m_gameData->stage = 0;
	m_gameData->livesRemaining = INITIAL_LIVES;
	m_gameData->frameCounter = 0;
	m_gameData->specialWeapons = 1;
	m_gameData->invincible = true;
	m_gameData->rapidFirePerk = false;
	m_gameData->transition = true;
	m_gameData->countdown = SPAWN_DELAY_SECONDS * TARGET_FRAME_RATE;

	transitionText.setString("Ready");
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
	auto& musicMan = m_data->musicManager;
	musicMan.setMaxVolume(m_data->musicVolumeFactor * 100);
	musicMan.play("Sounds/Nearly_There.ogg");
	musicMan.getMusic().setLoop(true);
}

void PlayState::processInput()
{
	Player* p = m_gameData->p.get();

	m_data->input.update(m_data->window);
	const auto& input = m_data->input;
#if _DEBUG
	if (input.wasKeyPressed(sf::Keyboard::F8))
	{
		scoreText.setCharacterSize(scoreText.getCharacterSize() - 2);
		livesText.setCharacterSize(livesText.getCharacterSize() - 2);
		specialText.setCharacterSize(specialText.getCharacterSize() - 2);
	}
	if (input.wasKeyPressed(sf::Keyboard::F9))
	{
		scoreText.setCharacterSize(scoreText.getCharacterSize() + 2);
		livesText.setCharacterSize(livesText.getCharacterSize() + 2);
		specialText.setCharacterSize(specialText.getCharacterSize() + 2);
	}
#endif
	if (input.wasKeyPressed(sf::Keyboard::Escape))
	{
		m_data->machine.addState(StateRef(std::make_unique<PausedState>(m_data, m_gameData)), false);
		return;
	}
	if (input.wasKeyPressed(sf::Keyboard::S) ||
		input.wasKeyPressed(sf::Keyboard::Down))
	{
		p->angle += 180;
	}
	if (input.wasKeyPressed(sf::Keyboard::LShift) ||
		input.wasKeyPressed(sf::Keyboard::RShift))
	{
		if (m_gameData->specialWeapons > 0)
		{
			specialWeaponSound->play();
			for (int angle = 0; angle < 360; angle += 4)
			{
				(void)addEntity<SidewinderBullet>(redBulletSprite, (int)p->x, (int)p->y, (float)angle, 10.0f);
			}
			m_gameData->specialWeapons--;
		}
	}
	if (m_gameData->rapidFirePerk)
	{
		if (input.isKeyDown(sf::Keyboard::Space))
		{
			if (m_gameData->frameCounter % FIRE_RATE_DELAY == 0)
			{
				rapidPhaserSound->play();
				(void)addEntity<SidewinderBullet>(redBulletSprite, (int)p->x, (int)p->y, p->angle, 10.0f);
			}
		}
	}
	else if (input.wasKeyPressed(sf::Keyboard::Space))
	{
		basicPhaserSound->play();
		(void)addEntity<StandardBullet>(blueBulletSprite, (int)p->x, (int)p->y, p->angle, 10.0f);
	}
	if (input.isKeyDown(sf::Keyboard::Right) ||
		input.isKeyDown(sf::Keyboard::D)) p->angle += 3;
	if (input.isKeyDown(sf::Keyboard::Left) ||
		input.isKeyDown(sf::Keyboard::A))  p->angle -= 3;
	if (input.isKeyDown(sf::Keyboard::Up) ||
		input.isKeyDown(sf::Keyboard::W)) p->thrust = true;
	else p->thrust = false;
	if (p->angle > 360.0f)
		p->angle -= 360;
	if (p->angle < 0)
		p->angle += 360;
}

void PlayState::update(float dt)
{
	if (m_data->restartRequired)
	{
		init();
		m_data->restartRequired = false;
	}
	if (m_data->backToTitle)
	{
		m_data->machine.removeState();
		m_data->backToTitle = false;
	}
	handleCollisions();
	addNewEntities();
	updateEntities();
	if (--m_gameData->countdown <= 0)
	{
		m_gameData->invincible = false;
		m_gameData->rapidFirePerk = false;
	}
	powerUpText.update();
	if (fadeIn > 0)
		fadeIn -= 5;
	m_gameData->frameCounter++;
}

void PlayState::draw(float dt)
{
	m_window->clear();
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
	Player* p = m_gameData->p.get();
	for (const auto& a : m_gameData->entities)
	{
		for (const auto& b : m_gameData->entities)
		{
			if (a->alive && b->alive)
			{
				if (a->type == EntityType::Asteroid && b->type == EntityType::Bullet)
				{
					if (isCollide(a.get(), b.get()))
					{
						a->alive = false;
						b->alive = false;

						Entity* e = addEntity<Entity>(explosionSprite, (int)a->x, (int)a->y).get();
						e->type = EntityType::Explosion;
						impactExplosionSound->play();
						for (int i = 0; i < 2; i++)
						{
							if (a->R == 15) continue;

							(void)addEntity<Asteroid>(rockSmallSprite, (int)a->x, (int)a->y, (float)(rand() % 360), 15);
							m_gameData->asteroidCount++;
						}
						increaseScore(160);
					}
				}
			}

			if (!m_gameData->invincible && a->type == EntityType::Player)
			{
				if (b->alive && b->type == EntityType::Asteroid)
				{
					if (isCollide(a.get(), b.get()))
					{
						b->alive = false;

						Entity* e = addEntity<Entity>(explosionShipSprite, (int)a->x, (int)a->y).get();
						e->type = EntityType::Explosion;

						p->settings(playerSprite, SCRN_WIDTH / 2, SCRN_HEIGHT / 2, 0, 20);
						p->dx = 0; p->dy = 0;

						shipExplosionSound->play();

						if (--m_gameData->livesRemaining == 0)
						{
							gameOver();
						}
						else
						{
							m_gameData->countdown = SPAWN_DELAY_SECONDS * TARGET_FRAME_RATE;
						}

						m_gameData->invincible = true;
					}
				}
				if (b->type == EntityType::RapidFire)
				{
					if (isCollide(a.get(), b.get()))
					{
						rapidUpSound->play();
						b->alive = false;
						m_gameData->invincible = true;
						m_gameData->rapidFirePerk = true;
						m_gameData->countdown = RAPID_FIRE_SECONDS * TARGET_FRAME_RATE;
						increaseScore(480);
						powerUpText.add("+480", (int)b->x, (int)b->y);
					}
				}
				if (b->type == EntityType::AddLife)
				{
					if (isCollide(a.get(), b.get()))
					{
						livesUpSound->play();
						b->alive = false;
						m_gameData->livesRemaining++;
						increaseScore(800);
						powerUpText.add("+800", (int)b->x, (int)b->y);
					}
				}
				if (b->type == EntityType::AddSpecial)
				{
					if (isCollide(a.get(), b.get()))
					{
						specialUpSound->play();
						b->alive = false;
						m_gameData->specialWeapons++;
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
		(void)addEntity<Asteroid>(rockSprite, 0, rand() % SCRN_HEIGHT, (float)(rand() % 360), 25);
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

		if (e->type == EntityType::Explosion)
		{
			if (e->anim.isEnd()) e->alive = false;
		}

		e->update();
		e->anim.update();

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
	bool blink = m_gameData->frameCounter % 30 > 10;
	for (auto rit = m_gameData->entities.crbegin(); rit != m_gameData->entities.crend(); ++rit)
	{
		auto e = (*rit).get();
		if (e->type == EntityType::Player)
		{
			if (blink || !m_gameData->invincible)
				e->draw(*m_window);
		}
		else
		{
			e->draw(*m_window);
		}
	}
}

void PlayState::drawText()
{
	scoreText.setString("Score: " + std::to_string(m_gameData->score));
	livesText.setString("Lives: " + std::to_string(m_gameData->livesRemaining));
	sf::FloatRect bounds = livesText.getLocalBounds();
	livesText.setPosition(SCRN_WIDTH - bounds.width - 6, 0);
	specialText.setString("Special Weapons: " + std::to_string(m_gameData->specialWeapons));
	sf::FloatRect bounds2 = specialText.getLocalBounds();
	specialText.setPosition(SCRN_WIDTH - bounds2.width - 6, bounds.height + 8);

#ifdef _DEBUG
	debugText.setString("**Debug**\n\nasteroidCount: " +
		std::to_string(m_gameData->asteroidCount) + '/' +
		std::to_string(m_gameData->maxAsteroids) + "\ninvincible: " +
		std::to_string(m_gameData->invincible) + " | secs: " +
		std::to_string(m_gameData->countdown / TARGET_FRAME_RATE) + "\nstage: " +
		std::to_string(m_gameData->stage) + "\nspecialWeapons: " +
		std::to_string(m_gameData->specialWeapons) + "\nrapidFirePerk: " +
		std::to_string(m_gameData->rapidFirePerk) + "\nframeCounter: " +
		std::to_string(m_gameData->frameCounter) + "\nentities.size(): " +
		std::to_string(m_gameData->entities.size()));
	m_window->draw(debugText);
#endif

	m_window->draw(scoreText);
	m_window->draw(livesText);
	m_window->draw(specialText);

	if (m_gameData->transition)
	{
	transitionTextColor.a = (uint8_t)(255 * (float)m_gameData->countdown / (float)(TRANSITION_DELAY * TARGET_FRAME_RATE));
	transitionText.setFillColor(transitionTextColor);
	m_window->draw(transitionText);
	if (m_gameData->countdown == 0)
		m_gameData->transition = false;
	}
	m_window->draw(powerUpText);
}

bool PlayState::isCollide(Entity * a, Entity * b)
{
	return (b->x - a->x)*(b->x - a->x) +
		(b->y - a->y)*(b->y - a->y) <
		(a->R + b->R)*(a->R + b->R);
}

void PlayState::increaseScore(int amount)
{
	m_gameData->score += amount;
	if (m_gameData->score % (STAGE_INTERVAL * m_data->difficulty) == 0)
	{
		m_gameData->stage++;
		m_gameData->maxAsteroids += 6;
		m_gameData->asteroidProbability = std::max(200 - m_gameData->stage * 36, 20);
		transitionText.setString("Stage " + std::to_string(m_gameData->stage + 1));
		m_gameData->transition = true;
		m_gameData->invincible = true;
		m_gameData->countdown = TRANSITION_DELAY * TARGET_FRAME_RATE;
	}
}

void PlayState::gameOver()
{
	m_data->machine.addState(StateRef(std::make_unique<GameOverState>(m_data, m_gameData)), false);
}
