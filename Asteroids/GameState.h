#pragma once


class GameState
{
public:
	virtual ~GameState() = default;
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() {};
	virtual void resume() {};

	virtual void processInput() = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;
};
