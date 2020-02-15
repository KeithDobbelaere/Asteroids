#pragma once

#include "Defines.h"
#include "Entity.h"


class Bullet : public Entity
{
public:
	Bullet()
	{
		dx = 0.0f;
		dy = 0.0f;
		da = 0.0f;
		lifetime = 0;
	}


protected:
	inline float qSin(float a)
	{
		a -= (float)(a > PI) * TWO_PI;
		float res = 0.0f, pow = a, fact = 1.0f;
		for (int i = 0; i < 5; i++)
		{
			res += pow / fact;
			pow *= -1 * a * a;
			fact *= (2 * (i + 1)) * (2 * (i + 1) + 1);
		}
		return res;
	}

	inline float qCos(float a)
	{
		a = PI_2 - a;
		a += (float)(a < -PI) * TWO_PI;
		float res = 0.0f, pow = a, fact = 1.0f;
		for (int i = 0; i < 5; i++)
		{
			res += pow / fact;
			pow *= -1 * a * a;
			fact *= (2 * (i + 1)) * (2 * (i + 1) + 1);
		}
		return res;
	}
};

class StandardBullet : public Bullet
{
public:
	StandardBullet()
	{
		type = EntityType::Bullet;
	}

	void  update() override
	{
		dx = qCos(angle * DEGTORAD) * BULLET_SPEED;
		dy = qSin(angle * DEGTORAD) * BULLET_SPEED;
		x += dx;
		y += dy;

		if (x > SCRN_WIDTH || x < 0 || y > SCRN_HEIGHT || y < 0) alive = false;
	}
};

class SidewinderBullet : public Bullet
{
public:
	SidewinderBullet()
	{
		type = EntityType::Bullet;
	}

	void update() override
	{
		dx = qCos(angle * DEGTORAD) * BULLET_SPEED;
		dy = qSin(angle * DEGTORAD) * BULLET_SPEED;
		angle += rand() % 7 - 3;
		if (angle < 0.0f)
			angle += 360.0f;
		if (angle > 360.0f)
			angle -= 360.0f;
		x += dx;
		y += dy;

		if (x > SCRN_WIDTH || x < 0 || y > SCRN_HEIGHT || y < 0) alive = false;
	}
};
