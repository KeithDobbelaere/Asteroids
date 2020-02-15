#include "DemoState.h"

#include <SFML/Graphics.hpp>

#include <iostream>

#include "Defines.h"
#include "Player.h"
#include "Bullets.h"


DemoState::DemoState(AppDataRef data, GameDataRef gameData) :
	PlayState(data, gameData)
{
#if _DEBUG
	std::cout << "STATE_MACHINE: DemoState constructed!\n";
#endif
	pressKeyText.text = sf::Text("Press any key to play!", *m_font);
	pressKeyText.text.setStyle(sf::Text::Bold);
	const auto& bounds1 = pressKeyText.text.getLocalBounds();
	pressKeyText.text.setPosition(SCRN_WIDTH / 2 - bounds1.width / 2, SCRN_HEIGHT - 50);
	demoModeText.text = sf::Text("-- DEMO MODE --", *m_font);
	demoModeText.text.setStyle(sf::Text::Bold);
	const auto& bounds2 = demoModeText.text.getLocalBounds();
	demoModeText.text.setPosition(SCRN_WIDTH / 2 - bounds2.width / 2, 10);
	oldDifficulty = m_data->difficulty;
	m_data->difficulty = 1;
	fadeOut = 0;
}

DemoState::~DemoState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: DemoState destroyed!\n";
#endif
	m_data->difficulty = oldDifficulty;
}


void DemoState::init()
{
	PlayState::init();
	ai.init(m_gameData);
	ai.delay.restart();
}

void DemoState::processInput()
{
	Player* p = m_gameData->p.get();

	m_data->input.update(m_data->window);

	if (m_data->input.anyKeyPressed())
	{
		m_data->machine.removeState();
		return;
	}

	if (m_gameData->livesRemaining > 0)
		ai.update();
	else
		ai.input = AIComponent::KeyPressed::NONE;

	if (ai.input & AIComponent::KeyPressed::DOWN)
	{
		p->angle += 180;
	}
	if (ai.input & AIComponent::KeyPressed::SHIFT)
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
		if (ai.input & AIComponent::KeyPressed::SPACE)
		{
			if (m_gameData->frameCounter % FIRE_RATE_DELAY == 0)
			{
				rapidPhaserSound->play();
				(void)addEntity<SidewinderBullet>(redBulletSprite, (int)p->x, (int)p->y, p->angle, 10.0f);
			}
		}
	}
	else
	{
		if (ai.input & AIComponent::KeyPressed::SPACE)
		{
			basicPhaserSound->play();
			(void)addEntity<StandardBullet>(blueBulletSprite, (int)p->x, (int)p->y, p->angle, 10.0f);
		}
	}
	if (ai.input & AIComponent::KeyPressed::RIGHT) p->angle += 3;
	if (ai.input & AIComponent::KeyPressed::LEFT)  p->angle -= 3;
	if (ai.input & AIComponent::KeyPressed::UP) p->thrust = true;
	else p->thrust = false;
	if (p->angle > 360.0f)
		p->angle -= 360;
	if (p->angle < 0)
		p->angle += 360;
}

void DemoState::update(float dt)
{
	PlayState::update(dt);
	if (m_gameData->livesRemaining == 0)
		fadeOut += 2;

	if (fadeOut >= 255)
	{
		m_data->machine.removeState();
	}
	pressKeyText.update(0.05f);
	demoModeText.update(0.05f);
}

void DemoState::draw(float dt)
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
	if (fadeOut > 0)
	{
		sf::RectangleShape shape(sf::Vector2f(SCRN_WIDTH, SCRN_HEIGHT));
		shape.setFillColor(sf::Color(0, 0, 0, fadeOut));
		m_window->draw(shape);
	}
	m_window->display();
}

void DemoState::drawScene()
{
	PlayState::drawScene();

#if _DEBUG
	for (auto line : ai.lines)
	{
		m_window->draw(&line[0], 2, sf::Lines);
	}
	m_window->draw(ai.targetCircle);
	m_window->draw(ai.interceptPosCircle);
	m_window->draw(ai.trajectoryPosCircle);
#endif
}

void DemoState::drawText()
{
	PlayState::drawText();

	m_window->draw(pressKeyText.text);
	m_window->draw(demoModeText.text);
}

void DemoState::gameOver()
{
	m_gameData->countdown = 2000;
}
