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

	void updateImpl() override
	{
		x += dx;
		y += dy;

		wrapScreenPosition();
	}

	bool collideWith(Entity* other) override
	{
		if (other->alive) {
			if (other->type == EntityType::Bullet)
				if (isCollide(other))
				{
					alive = false;
					return true;
				}
		}
		return false;
	}
};
