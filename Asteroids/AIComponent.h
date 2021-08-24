#pragma once

#include "Application.h"
#include "Player.h"

#include <vector>
#include <memory>


class AIComponent
{
public:
	AIComponent();
	~AIComponent() = default;

	void init(GameDataRef data);
	void update();

private:
	bool quadraticSolver(float a, float b, float c, float& solution1, float& solution2);
	inline float vecLength(const sf::Vector2f& vec) const;
	inline float vecAngle(const sf::Vector2f& vec) const;
	inline sf::Vector2f normalize(const sf::Vector2f& vec) const;
	inline float dotProduct(const sf::Vector2f& vec1, const sf::Vector2f& vec2) const;
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

	sf::Vector2f currentTrajectory, pPos, pVel, ePos, eVel, vecFromE;
	const sf::Vector2f screenCenter;

	bool specialWeaponUsable = true, centering = false, chasing = false;
	int specialWepCountdown;
	struct Target
	{
		std::shared_ptr<Entity> entity;
		sf::Vector2f interceptPos;
		float distance = 0.f, angle = 0.f, angleDelta = 0.f, interceptTime = 0.f;
	} t;
	
	std::shared_ptr<Player> m_player;
private:
	GameDataRef m_gameData;
#if _DEBUG
	inline void updateDebugInfo();
public:
	std::vector <std::array<sf::Vertex, 2>> lines;
	sf::CircleShape targetCircle;
	sf::CircleShape interceptPosCircle;
	sf::CircleShape trajectoryPosCircle;
#endif
};
