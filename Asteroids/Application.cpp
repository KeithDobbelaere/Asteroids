#include "Application.h"

#include <iostream>

#include "SplashState.h"


Application::Application(int width, int height, const char * title)
{
	data->window.create(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close);
	sf::Image icon;
	icon.loadFromFile("Images/Icon.png");
	data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	data->window.setVerticalSyncEnabled(true); //vastly reduces resource consumption
	data->window.setKeyRepeatEnabled(false);
	data->machine.addState(StateRef(std::make_unique<SplashState>(data, gameData)));

	srand((uint32_t)time(0));

	auto& assets = data->assets;
	assets.loadFont("default", "Fonts/BarcadeNoBar.ttf");
	assets.loadFont("arcadeItal", "Fonts/BarcadeBoldItalic.otf");
	assets.loadFont("arcadeBar", "Fonts/Barcade.otf");
	data->running = true;
	Run();
}

void Application::Run()
{
	float newTime, frameTime, interpolation;
	float currentTime = m_clock.getElapsedTime().asSeconds();
	float accumulator = 0.0f;

	while (data->window.isOpen() && data->running)
	{
		auto& machine = data->machine;
		machine.processStateChanges();

		newTime = m_clock.getElapsedTime().asSeconds();
		frameTime = newTime - currentTime;
		if (frameTime > 0.25f)
			frameTime = 0.25f;

		currentTime = newTime;
		accumulator += frameTime;

		auto& activeState = machine.getActiveState();
		while (accumulator >= dt)
		{
			activeState->processInput();
			activeState->update(dt);
			data->musicManager.update(sf::seconds(dt));

			accumulator -= dt;
		}
		interpolation = accumulator / dt;
		activeState->draw(interpolation);
	}
}
