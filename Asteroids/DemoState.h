#pragma once

#include "PlayState.h"

#include "Application.h"
#include "TextEffects.h"
#include "AIComponent.h"


class DemoState : public PlayState
{
public:
	DemoState(AppDataRef data, GameDataRef gameData);
	~DemoState();

	void init() override;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;

private:
	void drawScene();
	void drawText();
	void gameOver() override;

	int fadeOut;
	Difficulty oldDifficulty;
	PulsingText pressKeyText;
	PulsingText demoModeText;

	AIComponent ai;
};
