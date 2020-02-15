#pragma once

#include <vector>
#include <memory>

#include "Application.h"
#include "Player.h"

class AIComponent
{
public:
	AIComponent();
	~AIComponent() = default;

	void init(GameDataRef data);
	void update();

private:
	bool quadraticSolver(float a, float b, float c, float& solution1, float& solution2);
	float vecLength(const sf::Vector2f& vec) const;
	float vecAngle(const sf::Vector2f& vec) const;
	sf::Vector2f normalize(const sf::Vector2f& vec) const;
	float dotProduct(const sf::Vector2f& vec1, const sf::Vector2f& vec2) const;
	void findTarget();
	void findInterceptPoint();
	void trackTarget();
	void fireBullet();
	void moveToCenter();

public:
	enum KeyPressed : uint8_t
	{
		NONE	= 0,
		UP		= 1<<0,
		DOWN    = 1<<1,
		LEFT    = 1<<2,
		RIGHT   = 1<<3,
		SPACE	= 1<<4,
		SHIFT   = 1<<5
	};
	uint8_t input;
	sf::Clock delay;
#if _DEBUG
	std::vector <std::array<sf::Vertex, 2>> lines;
	sf::CircleShape targetCircle;
	sf::CircleShape interceptPosCircle;
	sf::CircleShape trajectoryPosCircle;
#endif
	sf::Vector2f currentTrajectory, pPos, pVel, ePos, eVel, vecFromE, interceptPos;

	bool specialWeaponUsable = true;
	int specialWepCntDn;
	struct Target
	{
		bool centering = false;
		bool chasing = false;
		std::shared_ptr<Entity> entity;
		sf::Vector2f interceptPos;
		float distance = 0.f, angle = 0.f, angleDelta = 0.f;
	};
	Target t;
	std::shared_ptr<Player> m_player;

private:
	const sf::Int32 FIRE_DELAY_MS = 120;
	GameDataRef m_gameData;
};

