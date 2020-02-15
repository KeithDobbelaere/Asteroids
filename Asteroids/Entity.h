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

	void settings(Animation &a, int X, int Y, float Angle = 0.0f, float radius = 1)
	{
		anim = a;
		x = (float)X; y = (float)Y;
		angle = Angle;
		R = radius;
	}

	virtual void update() {};

	void draw(sf::RenderWindow& app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);

#if _DEBUG
		sf::CircleShape circle(R);
		circle.setFillColor(sf::Color(255, 0, 0, 70));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
		app.draw(circle);
#endif
	}

	void draw(sf::RenderWindow& app, sf::Color color)
	{
		sf::Color curColor = anim.sprite.getColor();
		anim.sprite.setColor(color);
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);
		anim.sprite.setColor(curColor);
	}

	virtual ~Entity() {};
};