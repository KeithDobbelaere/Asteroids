#pragma once

#include <SFML/Graphics.hpp>

#include "Application.h"


class SubItem : public sf::Drawable, public sf::Transformable
{
public:
	virtual ~SubItem() = default;

	virtual void highlight() {};
	virtual void unhighlight() {};
	virtual void update() {};
	virtual sf::FloatRect getGlobalBounds() const { return sf::FloatRect(); }

	virtual void setValue(float) {};
	virtual void moveValue(float) {};

public:
	enum class Type
	{
		None,
		RangeSlider,
		Text,
	};
	Type type = Type::None;
	bool visible = false, needsUpdate = true;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {}
};

struct MenuItem
{
	MenuItem(sf::Text text_in) : text(text_in) {}
	sf::Text text;
	bool hasSubItems = false;
	bool subMenuOpened = false;
	bool selectable = false;

	int subMenuCurHeight = 0, subMenuMaxHeight = 0;
	std::vector<std::shared_ptr<SubItem>> subItems;
};

class RangeSlider : public SubItem
{
public:
	RangeSlider(AppDataRef data, float& refVar, const sf::Color& baseColor, const sf::Color& highlightColor, const sf::Color& accentColor = sf::Color::Cyan);

	void highlight() override;
	void unhighlight() override;
	void update() override;

	sf::FloatRect getGlobalBounds() const override;
	const sf::RectangleShape& getBarElement();
	const sf::RectangleShape& getSliderElement();

	float getValue() const { return m_value; }
	void setValue(float val) override { m_value = val; needsUpdate = true; }
	void moveValue(float val) override;
	void setAccentColor(const sf::Color& color) { m_accentColor = color; }

public:
	bool dragging = false;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	float& m_value;
	sf::Text m_valueText;
	sf::Sprite m_barSpriteLeft, m_barSpriteRight, m_sliderSprite;
	sf::RectangleShape m_base;
	sf::RectangleShape m_returnObject1, m_returnObject2;
	sf::Color m_baseColor, m_highlightColor;
	sf::Color m_accentColor;
};

class Text : public SubItem
{
public:
	Text(sf::Text text, const sf::Color& baseColor, const sf::Color& highlightColor);

	void highlight() override;
	void unhighlight() override;
	void update() override;
	sf::FloatRect getGlobalBounds() const override;
	void setString(const char* newString) { m_text.setString(newString); }

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.transform *= getTransform();

		target.draw(m_text, states);
	}

	sf::Text m_text;
	sf::Color m_baseColor, m_highlightColor;
};
