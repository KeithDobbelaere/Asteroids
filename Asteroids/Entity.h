#pragma once

#include <SFML/Graphics.hpp>

#include "Animation.h"


enum class EntityType
{
	None,
	Player,
	Asteroid,
	Bullet,
	Explosion,
	RapidFire,
	AddLife,
	AddSpecial
};

class Entity
{
public:
	int lifetime;
	float x, y, dx, dy, R, angle, da;
	bool alive;
	EntityType type;
	Animation anim;

	Entity() :
		lifetime(0), dx(0.0f), dy(0.0f), da(0.0f), x(0.f), y(0.f),
		R(0.f), angle(0.f), type(EntityType::None), alive(true)
	{
	}
	virtual ~Entity() = default;

	void settings(Animation &a, int X, int Y, float Angle = 0.0f, float radius = 1)
	{
		anim = a;
		x = (float)X; y = (float)Y;
		angle = Angle;
		R = radius;
	}

	void update()
	{
		anim.update();
		updateImpl();
	}
	virtual void updateImpl() {}

	virtual bool collideWith(Entity* other) {
		return false;
	}

	void draw(sf::RenderWindow& window)
	{
		if (blink()) return;
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		window.draw(anim.sprite);

#if _DEBUG
		sf::CircleShape circle(R);
		circle.setFillColor(sf::Color(255, 0, 0, 70));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
		window.draw(circle);
#endif
	}

protected:
	inline bool isCollide(Entity* other)
	{
		return (other->x - x) * (other->x - x) +
			(other->y - y) * (other->y - y) <
			(R + other->R) * (R + other->R);
	}

	inline void wrapScreenPosition()
	{
		if (x > SCRN_WIDTH) x = 0; if (x < 0) x = SCRN_WIDTH;
		if (y > SCRN_HEIGHT) y = 0; if (y < 0) y = SCRN_HEIGHT;
	}

	inline bool notOnScreen() const
	{
		return (x > SCRN_WIDTH || x < 0 || y > SCRN_HEIGHT || y < 0);
	}

	virtual bool blink() const
	{
		return false;
	}
};

class Explosion : public Entity
{
public:
	Explosion()
	{
		type = EntityType::Explosion;
	}

	void updateImpl() override
	{
		if (anim.isEnd()) alive = false;
	}
};