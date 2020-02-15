#pragma once

#include "Defines.h"
#include "Entity.h"

class Asteroid : public Entity
{
public:
	Asteroid()
	{
		const float min = -4.0f;
		const float range = 8.0f;
		dx = min + (float)rand() / (float)(RAND_MAX / range);
		if (fabs(0.0f - dx) < 0.001) dx = -1.0f;
		dy = min + (float)rand() / (float)(RAND_MAX / range);

		type = EntityType::Asteroid;
	}

	void update() override
	{
		x += dx;
		y += dy;

		if (x > SCRN_WIDTH) x = 0;  if (x < 0) x = SCRN_WIDTH;
		if (y > SCRN_HEIGHT) y = 0;  if (y < 0) y = SCRN_HEIGHT;
	}
};
