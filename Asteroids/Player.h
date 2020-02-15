#pragma once

#include "Defines.h"
#include "Entity.h"

class Player : public Entity
{
public:
	bool thrust;

	Player()
	{
		type = EntityType::Player;
		thrust = false;
	}

	void update() override
	{
		if (thrust)
		{
			dx += (float)cos(angle*DEGTORAD)*0.2f;
			dy += (float)sin(angle*DEGTORAD)*0.2f;
		}
		else
		{
			dx *= 0.99f;
			dy *= 0.99f;
		}

		float speed = sqrt(dx * dx + dy * dy);
		if (speed > MAX_SPEED)
		{
			dx *= MAX_SPEED / speed;
			dy *= MAX_SPEED / speed;
		}

		x += dx;
		y += dy;

		if (x > SCRN_WIDTH) x = 0; if (x < 0) x = SCRN_WIDTH;
		if (y > SCRN_HEIGHT) y = 0; if (y < 0) y = SCRN_HEIGHT;
	}
};