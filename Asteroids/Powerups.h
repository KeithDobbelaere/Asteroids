#pragma once

#include "Defines.h"
#include "Entity.h"

class Powerup : public Entity
{
public:
	Powerup()
	{
		const float min = -2.0f;
		const float range = 4.0f;
		dx = min + (float)rand() / (float)(RAND_MAX / range);
		dy = min + (float)rand() / (float)(RAND_MAX / range);
		da = (float)(rand() % 10 - 5);
		lifetime = 1000;
	}

	void update() override
	{
		angle += da;
		x += dx;
		y += dy;

		if (x > SCRN_WIDTH || x < 0 || y > SCRN_HEIGHT || y < 0 || --lifetime == 0)
			alive = false;
	}
};

class RapidFire : public Powerup
{
public:
	RapidFire()
	{
		type = EntityType::RapidFire;
	}
};

class AddLife : public Powerup
{
public:
	AddLife()
	{
		type = EntityType::AddLife;
	}
};

class AddSpecial : public Powerup
{
public:
	AddSpecial()
	{
		type = EntityType::AddSpecial;
	}
};