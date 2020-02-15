#pragma once

#include <SFML/Graphics.hpp>

#include <list>

#include "Defines.h"


class PowerUpText : public sf::Drawable, public sf::Transformable
{
public:
	PowerUpText() = default;
	PowerUpText(const sf::Font& font) :
		font(font), color(255, 255, 0, 255) {}

	void add(const char* text, int x, int y)
	{
		int countdown = FADE_DURATION * TARGET_FRAME_RATE;
		sf::Text newText(text, font, size);
		newText.setStyle(sf::Text::Bold);
		sf::FloatRect bounds = newText.getLocalBounds();
		newText.setPosition(x - bounds.width / 2, y - bounds.height / 2);
		textElements.emplace_back(newText, countdown);
	}

	void update()
	{
		for (auto it = textElements.begin(); it != textElements.end();)
		{
			int countdown = --(it->second);
			if (countdown <= 0)
			{
				it = textElements.erase(it);
			}
			else
			{
				sf::Text& text = it->first;
				color.a = (uint8_t)(255 * (float)countdown / (float)(FADE_DURATION * TARGET_FRAME_RATE));
				text.setFillColor(color);
				++it;
			}
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const auto& element : textElements)
			target.draw(element.first, states);
	}
private:
	const int FADE_DURATION = 3;

	int size = 30;
	const sf::Font& font;
	std::list<std::pair<sf::Text, int>> textElements;
	sf::Color color;
};

struct PulsingText
{
	void update(float inc)
	{
		count += inc;
		color.a = (uint8_t)(255 * (cos(count) * 0.5f + 0.5f));
		text.setFillColor(color);
	}
	float count = 0;
	sf::Text text;
	sf::Color color = sf::Color(255, 255, 255, 255);
};