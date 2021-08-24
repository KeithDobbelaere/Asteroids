#pragma once

#include "Entity.h"


class StandardBullet : public Entity
{
public:
	StandardBullet()
	{
		type = EntityType::Bullet;
		dx = 0.0f;
		dy = 0.0f;
		da = 0.0f;
		lifetime = 0;
	}

	void  updateImpl() override
	{
		if (angle < 0.0f)
			angle += 360.0f;
		if (angle >= 360.0f)
			angle -= 360.0f;
		dx = cos(angle * DEGTORAD) * BULLET_SPEED;
		dy = sin(angle * DEGTORAD) * BULLET_SPEED;
		x += dx;
		y += dy;

		if (notOnScreen()) alive = false;

		onUpdate();
	}
protected:
	virtual void onUpdate() {}
};

class SidewinderBullet : public StandardBullet
{
private:
	void onUpdate() override {
		angle += rand() % 7 - 3;
	}
};
