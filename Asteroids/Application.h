#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <SFMLExt/Fader.hpp>

#include <list>
#include <memory>

#include "Player.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "StateMachine.h"
#include "ScoresManager.h"


struct GameData
{
	std::list<std::shared_ptr<Entity>> entities;

	std::shared_ptr<Player> p;

	int asteroidCount, asteroidProbability, maxAsteroids, score, stage;
	int livesRemaining, specialWeapons;
	int countdown;
	unsigned int frameCounter;

	bool invincible, rapidFirePerk, transition;
};

struct ApplicationData
{
	StateMachine machine = {};
	sf::RenderWindow window;
	AssetManager assets;
	InputManager input = {};
	bool running = false, restartRequired = false, newHighScore = false, backToTitle = false;
	float soundVolumeFactor = 1.0f, musicVolumeFactor = 0.3f;
	int difficulty = 2;
	sfext::MusicManager musicManager;
	ScoresManager highScores;
	char playerInitials[4] = "";
};

using AppDataRef = std::shared_ptr<ApplicationData>;
using GameDataRef = std::shared_ptr<GameData>;

class Application
{
public:
	Application(int width, int height, const char* title);
	~Application() = default;

private:
	void Run();

	const float dt = 1.0f / 60.0f;
	sf::Clock m_clock;
	AppDataRef data = std::make_shared<ApplicationData>();
	GameDataRef gameData = std::make_shared<GameData>();
};
