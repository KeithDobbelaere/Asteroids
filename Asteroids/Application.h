#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <SFMLExt/Fader.hpp>

#include "Player.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "StateMachine.h"
#include "ScoresManager.h"
#include "Controls.h"

#include <list>
#include <memory>


struct GameData
{
	std::list<std::shared_ptr<Entity>> entities;
	std::shared_ptr<Player> p;
	int asteroidCount;
	int asteroidProbability;
	int maxAsteroids;
	int score, stage;
	unsigned int frameCounter;
};

enum class Dialog {
	Accept,
	Reject,
	Cancel
};

struct ApplicationData
{
	ApplicationData(sf::Vector2f(*callback)(sf::View&, const sf::Vector2u&, float&)) :
		input(callback), view({ SCRN_WIDTH / 2, SCRN_HEIGHT / 2 }, { SCRN_WIDTH, SCRN_HEIGHT }) {
		
	}
	StateMachine machine {};
	sfext::MusicManager music;
	AssetManager assets;
	sf::RenderWindow window;
	sf::View view;
	InputManager input;
	bool running = false, restartRequired = false, newHighScore = false, backToTitle = false;
	float soundVolumeFactor = 1.0f, musicVolumeFactor = 0.3f;
	Difficulty difficulty = Difficulty::Medium;
	ScoresManager highScores;
	std::string background;
	Controls controls;
	Dialog dialog = Dialog::Cancel;
};

using AppDataPtr = std::shared_ptr<ApplicationData>;
using GameDataPtr = std::shared_ptr<GameData>;

class Application
{
public:
	Application(int width, int height, const char* title);
	~Application();

	void run();

private:
	const float dt = 1.0f / 60.0f;
	sf::Clock m_clock;
	AppDataPtr m_appData;
	GameDataPtr m_gameData = std::make_shared<GameData>();
};
