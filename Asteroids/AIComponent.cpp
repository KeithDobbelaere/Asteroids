#include "AIComponent.h"

#include <array>
#include <iostream>
#include <cmath>
#include <algorithm>


AIComponent::AIComponent()
{
	input = KeyPressed::NONE;
	specialWepCntDn = 0;
}

void AIComponent::init(GameDataRef data)
{
	m_gameData = data;
	m_player = data->p;
}

void AIComponent::update()
{
	input = KeyPressed::NONE;

	findTarget();
	trackTarget();

#if _DEBUG
	lines.clear();
	std::array<sf::Vertex, 2> lineVertices;
	float x, y, dx, dy, da, scale = 10.0f;
	for (auto& entity : m_gameData->entities)
	{
		da = entity->da;
		if (da == 0.0f)
		{
			x = entity->x;
			y = entity->y;
			dx = entity->dx;
			dy = entity->dy;
			lineVertices[0] = sf::Vertex(sf::Vector2f(x, y));
			lineVertices[1] = sf::Vertex(sf::Vector2f(scale * dx + x, scale * dy + y));
			lines.emplace_back(lineVertices);
		}
	}
	targetCircle = sf::CircleShape(t.entity->R);
	targetCircle.setPosition(t.entity->x - t.entity->R, t.entity->y - t.entity->R);
	targetCircle.setFillColor(sf::Color::Transparent);
	targetCircle.setOutlineThickness(1);
#endif
}

//Method adapted from code written by Mike Oberberger.
bool AIComponent::quadraticSolver(float a, float b, float c, float & solution1, float & solution2)
{
	if (a == 0.0f)
	{
		if (b == 0.0f)
		{
			solution1 = solution2 = NAN;
			return false;
		}
		else
		{
			solution1 = solution2 = -c / b;
			return true;
		}
	}

	float tmp = b * b - 4 * a * c;
	if (tmp < 0.0f)
	{
		solution1 = solution2 = NAN;
		return false;
	}

	tmp = sqrt(tmp);
	float _2a = 2 * a;
	solution1 = (-b + tmp) / _2a;
	solution2 = (-b - tmp) / _2a;
	return true;
}

float AIComponent::vecLength(const sf::Vector2f& vec) const
{
	return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
}

float AIComponent::vecAngle(const sf::Vector2f& vec) const
{
	return std::atan2(-vec.y, -vec.x) / PI * 180 + 180;
}

sf::Vector2f AIComponent::normalize(const sf::Vector2f & vec) const
{
	return vec / vecLength(vec);
}

float AIComponent::dotProduct(const sf::Vector2f & vec1, const sf::Vector2f & vec2) const
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

void AIComponent::findTarget()
{
	//Count down frames after firing special weapon to keep it from being fired repeatedly
	if (!specialWeaponUsable)
		if (--specialWepCntDn == 0) specialWeaponUsable = true;

	pPos = { m_player->x, m_player->y };
	float distanceToE, shortestDistToE = FLT_MAX;
	//Loop through all entities
	for (const auto& entity : m_gameData->entities)
	{
		//Ignore all entities besides asteroids and power-ups
		if (entity->type != EntityType::Bullet && entity->type != EntityType::Explosion &&
			entity->type != EntityType::Player)
		{
			ePos = { entity->x, entity->y };
			vecFromE = pPos - ePos;
			distanceToE = vecLength(vecFromE);
			
			if (entity->type == EntityType::Asteroid)
			{
				//If the player is not invincible, and an asteroid is within twice the player's radius,
				//fire special weapon, then wait ten frames to make it available again
				if (!m_gameData->invincible && specialWeaponUsable && distanceToE < m_gameData->p->R * 2 + entity->R)
				{
					specialWeaponUsable = false;
					specialWepCntDn = 10;
					input |= KeyPressed::SHIFT;
				}
			}
			if (distanceToE < shortestDistToE)
			{
				//If the closest entity is an asteroid, or if it's a power-up and player isn't invincible,
				//make that entity the target.
				if (entity->type == EntityType::Asteroid || !m_gameData->invincible)
				{
					t.entity = entity;
					shortestDistToE = distanceToE;
				}
			}
		}
	}
	findInterceptPoint();
}

void AIComponent::findInterceptPoint()
{
	//Target entities will be either an asteroid, or a power-up at this point.
	//Concept for some of the code in this method taken from a very informative
	//article by Mike Oberberger, thank you.
	//https://www.codeproject.com/Articles/990452/Interception-of-Two-Moving-Objects-in-D-Space

	pVel = { m_player->dx, m_player->dy };
	ePos = { t.entity->x, t.entity->y };
	eVel = { t.entity->dx, t.entity->dy };
	vecFromE = pPos - ePos;
	float distanceToE, eSpeed, interceptTime;
	distanceToE = vecLength(vecFromE);
	eSpeed = vecLength(eVel);
	//If target entity is close to a stand-still, no need to calculate an intercept point 
	if (eSpeed < 0.2f)
	{
		interceptPos = ePos;
		interceptTime = distanceToE / BULLET_SPEED;
	}
	else
	{
		float a;
		if (t.entity->type == EntityType::Asteroid)
		{
			//If target is an asteroid, set quadratic coefficient 'a' for the bullet's speed.
			a = BULLET_SPEED * BULLET_SPEED - eSpeed * eSpeed;
		}
		else
		{
			//If target is a power-up, set coefficient 'a' to player's max speed, with padding 
			//to avoid overshooting target.  This produces an approximation that works.
			a = (MAX_SPEED + .5f) * (MAX_SPEED + .5f) - eSpeed * eSpeed;
		}
		//Set remaining coefficients
		float b = 2.0f * dotProduct(vecFromE, eVel);
		float c = -distanceToE * distanceToE;
		float t1 = 0.0f, t2 = 0.0f;
		if (!quadraticSolver(a, b, c, t1, t2))
		{
			//Should never reach this code
			std::cout << "No fire Solution!" << '\n';
			return;
		}

		// Both values for t are negative, so the interception would have to have occurred in the past
		if (t1 < 0.0f && t2 < 0.0f)
		{
			std::cout << "No fire Solution!" << '\n';
			return;
		}


		if (t1 > 0.0f && t2 > 0.0f)
			interceptTime = std::min(t1, t2); //Both positive, take the smaller one
		else
			interceptTime = std::max(t1, t2); //One has to be negative, so take the larger one

		interceptPos = ePos + eVel * interceptTime;
	}
	t.chasing = false;
	//If entity is a power-up, chase it!
	if (t.entity->type != EntityType::Asteroid && t.entity->alive)
	{
		t.chasing = true;
		currentTrajectory = pPos + pVel * interceptTime;
		t.angle = vecAngle(interceptPos - currentTrajectory);
	}
	//Otherwise, point at it
	else
	{
		t.angle = vecAngle(interceptPos - pPos);
	}
	float pAngle = m_player->angle;
	t.angleDelta = t.angle - pAngle;
	if (t.angleDelta < 0.0f) t.angleDelta += 360.0f; //Keep angles between 0 and 360
	t.distance = distanceToE;
	t.interceptPos = interceptPos;
#if _DEBUG
	const float R = 5.0;
	interceptPosCircle = sf::CircleShape(R);
	interceptPosCircle.setPosition(t.interceptPos - sf::Vector2f(R, R));
	interceptPosCircle.setFillColor(sf::Color::Transparent);
	interceptPosCircle.setOutlineColor(sf::Color::Yellow);
	interceptPosCircle.setOutlineThickness(2);
	trajectoryPosCircle = sf::CircleShape(R);
	trajectoryPosCircle.setPosition(currentTrajectory - sf::Vector2f(R, R));
	trajectoryPosCircle.setFillColor(sf::Color::Transparent);
	trajectoryPosCircle.setOutlineColor(sf::Color::Red);
	trajectoryPosCircle.setOutlineThickness(2);
#endif
}

void AIComponent::trackTarget()
{
	if (!t.chasing)
	{
		//Re-position player to center of screen if too close to perimeter
		if ((!t.entity->alive || t.distance > 80) && (m_player->x < SCRN_WIDTH / 2 - 150 || m_player->x > SCRN_WIDTH / 2 + 150 ||
			m_player->y < SCRN_HEIGHT / 2 - 100 || m_player->y > SCRN_HEIGHT / 2 + 100))
			moveToCenter();
		else
			t.centering = false;
	}
	if (t.angleDelta >= 90 && t.angleDelta <= 270)
	{
		input |= KeyPressed::DOWN; //flip to point backwards
		t.angleDelta -= 180;
		if (t.angleDelta < 0.0f) t.angleDelta += 360.0f;
	}
	if (t.angleDelta > 270 && t.angleDelta < 358)
	{
		input |= KeyPressed::LEFT;
	}
	else if (t.angleDelta < 90 && t.angleDelta > 2)
	{
		input |= KeyPressed::RIGHT;
	}
	float difference = m_player->angle - t.angle;
	//If player is pointed at target:
	if (difference > -4 && difference < 4)
	{
		float pSpeed = vecLength(sf::Vector2f(m_player->dx, m_player->dy));
		//If target is a power-up:
		if (t.entity->alive && (t.entity->type == EntityType::AddLife || t.entity->type == EntityType::AddSpecial ||
			t.entity->type == EntityType::RapidFire))
		{
			//Slow down as player gets closer to power-up:
			if (t.distance > 160 * (pSpeed / MAX_SPEED))
				input |= KeyPressed::UP;
		}
		//If currently re-centering on the screen:
		else if (t.centering)
		{
			//Slow down as player gets nearer to center:
			if (t.distance > 400 * (pSpeed / MAX_SPEED))
				input |= KeyPressed::UP;
		}
		//If target is an asteroid:
		else if (t.entity->alive)
			fireBullet();
	}
	//Fire weapon and abandon re-centering if Rapid Fire power-up is active or if asteroid
	//is dangerously close
	if (m_gameData->rapidFirePerk || (t.entity->alive && !t.chasing && t.distance < 80.0f))
	{
		t.centering = false;
		fireBullet();
	}
}

//Add a slight delay to weapon firing to mimic a human
void AIComponent::fireBullet()
{
	if (m_gameData->rapidFirePerk)
		input |= KeyPressed::SPACE;
	else if (delay.getElapsedTime().asMilliseconds() >= FIRE_DELAY_MS)
	{
		input |= KeyPressed::SPACE;
		delay.restart();
	}
}

//Set target to center of screen
void AIComponent::moveToCenter()
{
	sf::Vector2f target((float)SCRN_WIDTH/2, (float)SCRN_HEIGHT/2);
	sf::Vector2f pPos(m_player->x, m_player->y);
	sf::Vector2f diff(target - pPos);
	t.angle = vecAngle(diff);
	float playerAngle = m_player->angle;
	t.angleDelta = t.angle - playerAngle;
	if (t.angleDelta < 0.0f) t.angleDelta += 360.0f;
	t.centering = true;
}
