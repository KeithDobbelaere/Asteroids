#include "SplashState.h"

#include "TitleState.h"

#include <iostream>


SplashState::SplashState(AppDataPtr data, GameDataPtr gameData) :
	m_appData(data), m_gameData(gameData), m_window(data->window), m_version()
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
	m_background.setTexture(m_appData->assets.getTexture("splash_screen"));
	m_background.setScale(sf::Vector2f(SCRN_WIDTH / m_background.getGlobalBounds().width,
		SCRN_HEIGHT / m_background.getGlobalBounds().height));
	char tempString[32];
	sprintf_s(tempString, sizeof(tempString), "Version: %d.%d.%d", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);
	m_version.setString(tempString);
	m_version.setFont(*m_appData->assets.getFont("arial"));
	m_version.setStyle(sf::Text::Bold);
	m_version.setPosition(SCRN_WIDTH - 220, SCRN_HEIGHT - 60);
}

void SplashState::cleanup()
{

}

void SplashState::processInput()
{
	m_appData->input.update(m_window, m_appData->view);
}

void SplashState::update(float dt)
{
	if (m_clock.getElapsedTime().asSeconds() > SPLASH_SCREEN_DELAY)
	{
		m_appData->machine.replaceState(StatePtr(std::make_unique<TitleState>(m_appData, m_gameData)));
	}
}

void SplashState::draw(float dt)
{
	m_window.clear();
	m_window.setView(m_appData->view);

	m_window.draw(m_background);
	m_window.draw(m_version);
	m_window.display();
}
