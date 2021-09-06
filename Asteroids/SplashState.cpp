#include "SplashState.h"

#include "TitleState.h"

#include <iostream>


SplashState::SplashState(AppDataPtr data, GameDataPtr gameData) :
	m_data(data), m_gameData(gameData), m_window(data->window)
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: SplashState constructed!\n";
#	endif
}

SplashState::~SplashState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: SplashState destroyed!\n";
#	endif
}


void SplashState::init()
{
	m_background.setTexture(m_data->assets.getTexture("splash_screen"));
	m_background.setScale(sf::Vector2f(SCRN_WIDTH / m_background.getGlobalBounds().width,
		SCRN_HEIGHT / m_background.getGlobalBounds().height));
}

void SplashState::cleanup()
{

}

void SplashState::processInput()
{
	m_data->input.update(m_window, m_data->view);
}

void SplashState::update(float dt)
{
	if (m_clock.getElapsedTime().asSeconds() > SPLASH_SCREEN_DELAY)
	{
		m_data->machine.addState(StatePtr(std::make_unique<TitleState>(m_data, m_gameData)), true);
	}
}

void SplashState::draw(float dt)
{
	m_window.clear();
	m_window.setView(m_data->view);

	m_window.draw(m_background);
	m_window.display();
}
