#include "Application.h"

#include "SplashState.h"
#include "Defines.h"

#include <iostream>


sf::Vector2f getLetterboxView(sf::View& view, const sf::Vector2u& windowSize, float& scaleFactor) {

	// Compares the aspect ratio of the window to the aspect ratio of the view,
	// and sets the view's viewport accordingly in order to archieve a letterbox effect.
	// A new view (with a new viewport set) is returned.

	float windowRatio = windowSize.x / (float)windowSize.y;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
		scaleFactor = SCRN_HEIGHT / (float)windowSize.y;
	}
	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
		scaleFactor = SCRN_WIDTH / (float)windowSize.x;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	sf::Vector2f offsets(windowSize.x * posX, windowSize.y * posY);
	return offsets;
}

Application::Application(int width, int height, const char * title) :
	data(std::make_shared<ApplicationData>(getLetterboxView))
{
	data->window.create(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
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
}

void Application::run()
{
	float newTime, frameTime, interpolation;
	float currentTime = m_clock.getElapsedTime().asSeconds();
	float accumulator = 0.0f;
	data->running = true;

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
			data->music.update(sf::seconds(dt));

			accumulator -= dt;
		}
		interpolation = accumulator / dt;
		activeState->draw(interpolation);
	}
}
