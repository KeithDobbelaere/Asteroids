#pragma once

#include <SFML/Graphics.hpp>

#include "Defines.h"

#include <list>


struct FadingText : public sf::Text
{
public:
	FadingText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30, int fadeDuration = 3)
		: sf::Text(string, font, characterSize), m_totalFrames(fadeDuration * TARGET_FRAME_RATE)
	{
		m_countdown = m_totalFrames;
		setStyle(sf::Text::Bold);
	}

	void update()
	{
		--m_countdown;
		sf::Color color = getFillColor();
		color.a = (uint8_t)(255 * (float)m_countdown / (float)m_totalFrames);
		setFillColor(color);
		if (m_countdown <= 0)
			m_alive = false;
	}
	const bool isAlive() const { return m_alive; }

protected:
	void resetCount()
	{ 
		m_countdown = m_totalFrames; 
		m_alive = true;
	}

private:
	bool m_alive = true;
	int m_totalFrames = 0;
	int m_countdown = 0;
};

struct TransitionText : public FadingText
{
public:
	TransitionText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30)
		: FadingText(string, font, characterSize, 4)
	{
		centerOnScreen();
	}

	void reset(const sf::String& string)
	{
		setString(string);
		centerOnScreen();
		resetCount();
	}

	void centerOnScreen()
	{
		const auto& bounds = getLocalBounds();
		setPosition(SCRN_WIDTH / 2 - bounds.width / 2, SCRN_HEIGHT / 2 - bounds.height / 2 - 50);
	}
};

struct PowerUpTextElement : public FadingText
{
public:
	PowerUpTextElement(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30)
		: FadingText(string, font, characterSize)
	{
		sf::FloatRect bounds = getLocalBounds();
		setOrigin(bounds.width / 2, bounds.height / 2);
	}
};

class PowerUpText : public sf::Drawable, public sf::Transformable
{
public:
	PowerUpText() = default;
	PowerUpText(const sf::Font& font) :
		m_font(font) {}

	void add(const char* text, int x, int y)
	{
		m_elements.emplace_back(text, m_font);
		m_elements.back().setFillColor({ 255, 255, 0, 255 });
		m_elements.back().setPosition((float)x, (float)y);
	}

	void update()
	{
		for (auto it = m_elements.begin(); it != m_elements.end(); )
		{
			it->update();
			if (!it->isAlive())
				it = m_elements.erase(it);
			else
				++it;
		}
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const auto& element : m_elements)
			target.draw(element, states);
	}

private:
	const sf::Font& m_font;
	std::list<PowerUpTextElement> m_elements;
};

struct PulsingText : public sf::Text
{
	PulsingText() = default;
	PulsingText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30)
		: sf::Text(string, font, characterSize) {}
	PulsingText(const sf::Text& other) : sf::Text(other) {}

	void update(float inc)
	{
		m_count += inc;
		sf::Color color = getFillColor();
		color.a = (uint8_t)(255 * (cos(m_count) * 0.5f + 0.5f));
		setFillColor(color);
	}
	float m_count = 0;
};

struct RainbowShiftText : public sf::Text
{
	RainbowShiftText() = default;
	RainbowShiftText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30)
		: sf::Text(string, font, characterSize) {}
	RainbowShiftText(const sf::Text& other) : sf::Text(other) {}

	void update(int alpha)
	{
		if (active)
		{
			auto color = sineBow(hue += .005f);
			color.a = alpha;
			setFillColor(color);
		}
	}

	void effect(bool state)
	{
		active = state;
	}

private:
	sf::Color sineBow(float h) {
		float r = .666f * std::max(.5f + std::cos((float)TWO_PI * h), 0.f);
		float g = .666f * std::max(.5f + std::cos((float)TWO_PI * (h + .333f)), 0.f);
		float b = .666f * std::max(.5f + std::cos((float)TWO_PI * (h + .666f)), 0.f);

		return {
			(uint8_t)(127.5f * r + 127.5f),
			(uint8_t)(127.5f * g + 127.5f),
			(uint8_t)(127.5f * b + 127.5f)
		};
	}

	float hue = 0.f;
	bool active = false;
};

struct BlinkingText : public sf::Text {
public:
	BlinkingText() = default;
	BlinkingText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 30)
		: sf::Text(string, font, characterSize) {}
	BlinkingText(const sf::Text& other) : sf::Text(other) {}

	void update()
	{
		if (active)
		{
			bool visible = m_count % 60 > 30;
			sf::Color color = getFillColor();
			setFillColor(visible ? color : sf::Color::Transparent);
			m_count++;
		}
	}

	void effect(bool state)
	{
		active = state;
	}

private:
	int m_count = 0;
	bool active = false;
};
