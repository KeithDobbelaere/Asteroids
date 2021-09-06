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
	sf::Image icon;
	icon.loadFromFile("Images/Icon.png");

	auto& window = data->window;
	window.create(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setVerticalSyncEnabled(true); //vastly reduces resource consumption
	window.setKeyRepeatEnabled(false);

	auto& assets = data->assets;
	assets.loadFont("default", "Fonts/BarcadeNoBar.ttf");
	assets.loadFont("arcadeItal", "Fonts/BarcadeBoldItalic.otf");
	assets.loadFont("arcadeBar", "Fonts/Barcade.otf");

	assets.loadTexture("splash_screen", "images/splash_screen.jpg");
	assets.loadTexture("haze", "images/haze.png");
	assets.loadTexture("star_field", "images/starfield.png");
	assets.loadTexture("slider", "images/range_slider.png").setSmooth(true);
	assets.loadTexture("ship", "images/spaceship.png").setSmooth(true);
	assets.loadTexture("background0", "images/galaxy-4.png").setSmooth(true);
	assets.loadTexture("background1", "images/galaxy-1.png").setSmooth(true);
	assets.loadTexture("background2", "images/galaxy-2.png").setSmooth(true);
	assets.loadTexture("explosion1", "images/explosions/type_C.png");
	assets.loadTexture("rock_large", "images/rock.png");
	assets.loadTexture("fire_blue", "images/fire_blue.png");
	assets.loadTexture("rock_small", "images/rock_small.png");
	assets.loadTexture("explosion2", "images/explosions/type_B.png");
	assets.loadTexture("lives", "images/lives.png");
	assets.loadTexture("fire_red", "images/fire_red.png");
	assets.loadTexture("special", "images/special.png");
	assets.loadTexture("rapid_fire", "images/rapid_fire.png");

	assets.loadSoundBuffer("click", "sounds/click_sound.ogg");
	assets.loadSoundBuffer("ship_explosion", "Sounds/Ship_Explosion.wav");
	assets.loadSoundBuffer("basic_phaser", "Sounds/Basic_Phaser.ogg");
	assets.loadSoundBuffer("rapid_phaser", "Sounds/Rapid_Phaser.ogg");
	assets.loadSoundBuffer("impact", "Sounds/Impact.wav");
	assets.loadSoundBuffer("special_weapon", "Sounds/Special_Weapon.ogg");
	assets.loadSoundBuffer("power_up1", "Sounds/Power_Up1.ogg");
	assets.loadSoundBuffer("power_up2", "Sounds/Power_Up2.ogg");
	assets.loadSoundBuffer("power_up3", "Sounds/Power_Up3.ogg");

	data->machine.addState(StatePtr(std::make_unique<SplashState>(data, gameData)));
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
