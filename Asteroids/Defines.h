#pragma once

constexpr int SCRN_WIDTH = 1200;
constexpr int SCRN_HEIGHT = 800;

constexpr float DEGTORAD = 0.0174532925f;
constexpr long double PI = 3.14159265358979323846L;
constexpr double TWO_PI = PI * 2;
constexpr double PI_2 = PI / 2;

constexpr int TARGET_FRAME_RATE = 60;

constexpr int SPLASH_SCREEN_DELAY = 3;
constexpr int TITLE_SCREEN_DELAY = 24;

constexpr int BULLET_SPEED = 6;
constexpr int MAX_SPEED = 7;
constexpr int MAX_FIRE_RATE = 12;
constexpr int FIRE_RATE_DELAY = TARGET_FRAME_RATE / MAX_FIRE_RATE;
constexpr int AI_FIRE_DELAY_MS = 120;
constexpr int SPAWN_DELAY_SECONDS = 4;
constexpr int RAPID_FIRE_SECONDS = 6;
constexpr int TRANSITION_DELAY = 4;
constexpr int STAGE_INTERVAL = 20000;
constexpr int INITIAL_LIVES = 7;

constexpr float DECELERATION = .99f;
static_assert(DECELERATION > 0.f && DECELERATION < 1.f);
constexpr float SPEED_CUTOFF = 1e-9f;
static constexpr float getStopDistanceMultiplier() {
	float x = 0;
	float v = 1.f;
	do {
		x += v;
		v *= DECELERATION;
	} while (v > SPEED_CUTOFF);
	return x;
}
constexpr float STOP_DISTANCE_MULTIPLIER = getStopDistanceMultiplier();

// alternate method for calculation of STOP_DISTANCE_MULTIPLIER, which isnt
// constexpr, but was interesting to figure out anyway:
/*
#include <cmath>
static const double v0 = 1.0;

static const double e = 2.718282;

double static getStopDistanceMultiplier(double c) {
	double k = 1.0 - c;
	double t = -log(SPEED_CUTOFF/v0) / k;
	return ((pow(e, -k*t) + v0) / k);
}
static const float STOP_DISTANCE_MULTIPLIER2 = getStopDistanceMultiplier(DECELERATION);
*/

constexpr const char* REGISTRY_KEY = "Software\\2R Games\\Asteroids!";
constexpr const char* APP_DATA_PATH = "\\Asteroids!\\Asteroids.bin";
constexpr const int APP_DATA_FILE_SIZE = 336;
