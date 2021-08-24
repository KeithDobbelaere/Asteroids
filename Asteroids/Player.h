#pragma once

#include "Defines.h"
#include "Entity.h"


class Player : public Entity
{
public:
	Player() :
		thrust(false), countdown(SPAWN_DELAY_SECONDS* TARGET_FRAME_RATE),
		livesRemaining(INITIAL_LIVES), specialWeapons(1), godMode(false),
		invincible(true), rapidFiring(false)
	{
		type = EntityType::Player; 
	}

	void updateImpl() override
	{
		if (thrust)
		{
			dx += (float)cos(angle*DEGTORAD)*0.2f;
			dy += (float)sin(angle*DEGTORAD)*0.2f;
		}
		else
		{
			dx *= DECELERATION;
			dy *= DECELERATION;
		}

		float speed = sqrt(dx * dx + dy * dy);
		if (speed > MAX_SPEED)
		{
			dx *= MAX_SPEED / speed;
			dy *= MAX_SPEED / speed;
		}

		x += dx;
		y += dy;

		wrapScreenPosition();

		if (--countdown == 0 && !godMode)
		{
			invincible = false;
			rapidFiring = false;
		}
	}

	bool collideWith(Entity* other) override
	{
		if (!invincible && other->alive)
		{
			switch (other->type)
			{
				case EntityType::Asteroid:
					if (isCollide(other))
					{
						--livesRemaining;
						return true;
					}
					break;
				case EntityType::RapidFire:
					if (isCollide(other))
					{
						rapidFiring = true;
						setInvincible(RAPID_FIRE_SECONDS);
						return true;
					}
					break;
				case EntityType::AddLife:
					if (isCollide(other))
					{
						livesRemaining++;
						return true;
					}
					break;
				case EntityType::AddSpecial:
					if (isCollide(other))
					{
						specialWeapons++;
						return true;
					}
			}
		}
		return false;
	}

	void reset()
	{
		x = SCRN_WIDTH / 2;
		y = SCRN_HEIGHT / 2;
		angle = 0.f;
		dx = 0;
		dy = 0;
		setInvincible(SPAWN_DELAY_SECONDS);
	}

	void fireSpecialWeapon() 
	{
		--specialWeapons; 
	}

	const int getLivesRemaining() const
	{
		return livesRemaining;
	}

	const int getSpecialWeapons() const
	{
		return specialWeapons;
	}

	const bool hasRapidFire() const
	{
		return rapidFiring;
	}

	void setInvincible(int seconds = -1)
	{
		invincible = true;
		countdown = seconds * TARGET_FRAME_RATE;
	}

	void setGodMode()
	{
		godMode = true;
		invincible = true;
	}

	const bool isInvincible() const
	{
		return invincible;
	}

	const int getCountdown() const
	{
		return countdown;
	}

	bool thrust, godMode;
	int livesRemaining;

private:
	bool blink() const override
	{
		if (invincible)
			if (countdown % 30 > 10)
				return true;
		return false;
	}

	unsigned int countdown;
	int specialWeapons;
	bool invincible, rapidFiring;
};