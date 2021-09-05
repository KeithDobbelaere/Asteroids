#include "AIComponent.h"

#include <array>
#include <iostream>
#include <cmath>
#include <algorithm>


AIComponent::AIComponent(std::list<std::shared_ptr<Entity>>& entities, int& asteroidCount) :
	m_entities(entities), m_asteroidCount(asteroidCount), input(KeyPressed::NONE), specialWepCountdown(0),
	screenCenter((float)SCRN_WIDTH / 2, (float)SCRN_HEIGHT / 2)
{
}

void AIComponent::init(std::shared_ptr<Player> player)
{
	m_player = player;
}

void AIComponent::update()
{
	input = KeyPressed::NONE;

	findTarget();
	if (t.entity) findInterceptPoint();
	trackTarget();
#	ifdef _DEBUG
		updateDebugInfo();
#	endif
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
	return std::atan2(-vec.y, -vec.x) / (float)PI * 180 + 180;
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
		if (--specialWepCountdown == 0) specialWeaponUsable = true;

	pPos = { m_player->x, m_player->y };
	float distanceToE, shortestDistToE = FLT_MAX;
	t.entity = nullptr;
	//Loop through all entities
	for (const auto& entity : m_entities)
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
				if (!m_player->isInvincible() && specialWeaponUsable && distanceToE < m_player->R * 2 + entity->R)
				{
					specialWeaponUsable = false;
					specialWepCountdown = 10;
					input |= KeyPressed::SHIFT;
				}
			}
			if (distanceToE < shortestDistToE)
			{
				//If the closest entity is an asteroid, or if it's a power-up and player isn't invincible,
				//add that entity the list of potential targets.
				if (entity->type == EntityType::Asteroid || !m_player->isInvincible())
				{
					t.entity = entity;
					shortestDistToE = distanceToE;
				}
			}
		}
	}
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
	float distanceToE, eSpeed;
	distanceToE = vecLength(vecFromE);
	eSpeed = vecLength(eVel);
	//If target entity is close to a stand-still, no need to calculate an intercept point 
	if (eSpeed < 0.2f)
	{
		t.interceptPos = ePos;
		t.interceptTime = distanceToE / BULLET_SPEED;
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
			//If target is a power-up, set coefficient 'a' to player's max speed.
			a = MAX_SPEED * MAX_SPEED - eSpeed * eSpeed;
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
			t.interceptTime = std::min(t1, t2); //Both positive, take the smaller one
		else
			t.interceptTime = std::max(t1, t2); //One has to be negative, so take the larger one

		t.interceptPos = ePos + eVel * t.interceptTime;
	}
}

void AIComponent::trackTarget()
{
	if (t.entity) {
		//If entity is a power-up, chase it!
		if (t.entity->alive && t.entity->type != EntityType::Asteroid)
		{
			chasing = true;
			currentTrajectory = pPos + pVel * t.interceptTime;
			t.angle = vecAngle(t.interceptPos - currentTrajectory);
		}
		//Otherwise, point at it
		else
		{
			chasing = false;
			t.angle = vecAngle(t.interceptPos - pPos);
		}
	}
	float pAngle = m_player->angle;
	t.angleDelta = t.angle - pAngle;
	if (t.angleDelta < 0.0f) t.angleDelta += 360.0f; //Keep angles between 0 and 360
	t.distance = vecLength(pPos - t.interceptPos);
	if (!chasing)
	{
		//Re-position player to center of screen if too close to perimeter
		if ((!t.entity || t.distance > 80) && (m_player->x < SCRN_WIDTH / 2 - 150 || m_player->x > SCRN_WIDTH / 2 + 150 ||
			m_player->y < SCRN_HEIGHT / 2 - 100 || m_player->y > SCRN_HEIGHT / 2 + 100))
			moveToCenter();
		else
			centering = false;
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
	if (difference > -2 && difference < 2)
	{
		float pSpeed = vecLength(sf::Vector2f(m_player->dx, m_player->dy));
		float decelDistance = pSpeed * STOP_DISTANCE_MULTIPLIER;
		//If target is a power-up:
		if (t.entity && (t.entity->type == EntityType::AddLife || t.entity->type == EntityType::AddSpecial ||
			t.entity->type == EntityType::RapidFire))
		{
			//Slow down as player gets closer to power-up:
			//if (t.distance > decelDistance - 160)
			if (t.distance > 160 * (pSpeed / MAX_SPEED))
				input |= KeyPressed::UP;
		}
		//If currently re-centering on the screen:
		else if (centering)
		{
			currentTrajectory = pPos + pVel * (decelDistance / pSpeed);
			//Slow down as player gets nearer to center:
			if (t.distance > decelDistance)
				input |= KeyPressed::UP;
		}
		//If target is an asteroid:
		else if (t.entity)
			fireBullet();
	}
	//Fire weapon and abandon re-centering if Rapid Fire power-up is active or if asteroid
	//is dangerously close
	if (m_asteroidCount > 12 || m_player->hasRapidFire() || (t.entity && !chasing && t.distance < 80.0f))
	{
		centering = false;
		fireBullet();
	}
}

//Add a slight delay to weapon firing to mimic a human
void AIComponent::fireBullet()
{
	if (m_player->hasRapidFire())
		input |= KeyPressed::SPACE;
	else if (delay.getElapsedTime().asMilliseconds() >= AI_FIRE_DELAY_MS)
	{
		input |= KeyPressed::SPACE;
		delay.restart();
	}
}

//Set target to center of screen
void AIComponent::moveToCenter()
{
	sf::Vector2f diff(screenCenter - pPos);
	t.distance = vecLength(diff);
	t.angle = vecAngle(diff);
	t.angleDelta = t.angle - m_player->angle;
	if (t.angleDelta < 0.0f) t.angleDelta += 360.0f;
	centering = true;
	chasing = false;
}


#if _DEBUG
void AIComponent::updateDebugInfo()
{
	lines.clear();
	std::array<sf::Vertex, 2> lineVertices;
	float x, y, dx, dy;
	const float SCALE = 10.0f;
	for (auto& entity : m_gameData->entities)
	{
		x = entity->x;
		y = entity->y;
		dx = entity->dx;
		dy = entity->dy;
		lineVertices[0] = sf::Vertex(sf::Vector2f(x, y));
		lineVertices[1] = sf::Vertex(sf::Vector2f(SCALE * dx + x, SCALE * dy + y));
		lines.emplace_back(lineVertices);
	}
	if (t.entity) {
		targetCircle = sf::CircleShape(t.entity->R);
		targetCircle.setPosition(t.entity->x - t.entity->R, t.entity->y - t.entity->R);
		targetCircle.setFillColor(sf::Color::Transparent);
		targetCircle.setOutlineThickness(1);
	}
	const float R = 5.0;
	interceptPosCircle = sf::CircleShape(R);
	interceptPosCircle.setPosition(t.interceptPos - sf::Vector2f(R, R));
	interceptPosCircle.setFillColor(sf::Color::Transparent);
	auto interceptPosColor = t.entity ? sf::Color::Yellow : sf::Color::Transparent;
	interceptPosCircle.setOutlineColor(interceptPosColor);
	interceptPosCircle.setOutlineThickness(2);
	trajectoryPosCircle = sf::CircleShape(R);
	trajectoryPosCircle.setPosition(currentTrajectory - sf::Vector2f(R, R));
	trajectoryPosCircle.setFillColor(sf::Color::Transparent);
	auto trajectoryPosColor = chasing ? sf::Color::Red : centering ? sf::Color::Green : sf::Color::Transparent;
	trajectoryPosCircle.setOutlineColor(trajectoryPosColor);
	trajectoryPosCircle.setOutlineThickness(2);
}
#endif
