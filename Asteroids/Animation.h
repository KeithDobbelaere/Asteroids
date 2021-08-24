#pragma once

#include <SFML/Graphics.hpp>


class Animation
{
public:
	float frame{}, speed{};
	sf::Sprite sprite;
	std::vector<sf::IntRect> frames;

	Animation() = default;
	Animation(const sf::Texture& t, sf::IntRect rect, int count, float Speed) :
		frame(0), speed(Speed)
	{
		for (int i = 0; i < count; i++)
		{
			frames.emplace_back(rect.left + i * rect.width, rect.top, rect.width, rect.height);
		}

		sprite.setTexture(t);
		sprite.setOrigin((float)rect.width / 2, (float)rect.height / 2);
		sprite.setTextureRect(frames[0]);
	}


	void update()
	{
		frame += speed;
		int n = (int)frames.size();
		if (frame >= n) frame -= n;
		if (n > 0) sprite.setTextureRect(frames[size_t(frame)]);
	}

	bool isEnd()
	{
		return frame + speed >= frames.size();
	}
};