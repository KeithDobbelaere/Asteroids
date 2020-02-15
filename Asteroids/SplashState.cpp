#include "SplashState.h"

#include <iostream>

#include "TitleState.h"


SplashState::SplashState(AppDataRef data, GameDataRef gameData) :
	m_data(data), m_gameData(gameData), m_window(data->window)
{
#if _DEBUG
	std::cout << "STATE_MACHINE: SplashState constructed!\n";
#endif
}

SplashState::~SplashState()
{
#if _DEBUG
	std::cout << "STATE_MACHINE: SplashState destroyed!\n";
#endif
}


void SplashState::init()
{
	m_data->assets.loadTexture("Splash_State_Background", "images/splash_screen.jpg");
	m_background.setTexture(m_data->assets.getTexture("Splash_State_Background"));
	m_background.setScale(sf::Vector2f(m_window.getSize().x / m_background.getGlobalBounds().width,
		m_window.getSize().y / m_background.getGlobalBounds().height));
}

void SplashState::cleanup()
{

}

void SplashState::processInput()
{
	m_data->input.update(m_window);
}

void SplashState::update(float dt)
{
	if (m_clock.getElapsedTime().asSeconds() > SPLASH_SCREEN_DELAY)
	{
		m_data->machine.addState(StateRef(std::make_unique<TitleState>(m_data, m_gameData)), true);
	}
}

void SplashState::draw(float dt)
{
	m_window.clear();
	m_window.draw(m_background);
	m_window.display();
}
