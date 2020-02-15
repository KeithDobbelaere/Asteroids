#pragma once


constexpr int SCRN_WIDTH = 1200;
constexpr int SCRN_HEIGHT = 800;

constexpr float DEGTORAD = 0.0174532925f;
constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = PI * 2;
constexpr float PI_2 = PI / 2;

constexpr int TARGET_FRAME_RATE = 60;

constexpr int SPLASH_SCREEN_DELAY = 3;
constexpr int TITLE_SCREEN_DELAY = 24;

constexpr int BULLET_SPEED = 6;
constexpr int MAX_SPEED = 7;
constexpr int MAX_FIRE_RATE = 12;
constexpr int FIRE_RATE_DELAY = TARGET_FRAME_RATE / MAX_FIRE_RATE;
constexpr int SPAWN_DELAY_SECONDS = 4;
constexpr int RAPID_FIRE_SECONDS = 6;
constexpr int TRANSITION_DELAY = 4;
constexpr int STAGE_INTERVAL = 20000;
constexpr int INITIAL_LIVES = 7;

constexpr const char* REGISTRY_KEY = "Software\\2R Games\\Asteroids!";
constexpr const char* APP_DATA_PATH = "\\Asteroids!\\asteroids.bin";