#pragma once

#include <SFML/Graphics.hpp>

#include "Application.h"
#include "TextEffects.h"
#include "Color.h"

class SubItem 
	: public sf::Drawable, public sf::Transformable
{
public:
	SubItem(sf::Color* base, sf::Color* highlight) :
		m_baseColor(base), m_highlightColor(highlight)
	{
	}
	virtual ~SubItem() = default;

	virtual void highlight() = 0;
	virtual void unhighlight() = 0;
	virtual void update() = 0;
	virtual bool processInput(const InputManager&, const sf::RenderWindow&) = 0;

	virtual sf::FloatRect getGlobalBounds() const = 0;
	virtual int getHeight() const = 0;

	virtual void setValue(float) {}
	virtual void moveValue(float) {}
	virtual void setString(const sf::String&) {}
	virtual void setStyle(sf::Uint32) {}
	virtual void select(bool) {}

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
	void draw(sf::RenderTarget&, sf::RenderStates) const override {}
	sf::Color* m_baseColor, * m_highlightColor;
};

class TextItem : public SubItem
{
public:
	TextItem(sf::Text text, sf::Color* baseColor, sf::Color* highlightColor);

	void highlight() override;
	void unhighlight() override;
	void update() override;
	bool processInput(const InputManager& input, const sf::RenderWindow& window) override;

	sf::FloatRect getGlobalBounds() const override;
	int getHeight() const override;

	void setString(const sf::String& newString) override;
	void setStyle(sf::Uint32 style) override;
	void select(bool state) override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	BlinkingText m_text;
};


class RangeSlider : public SubItem
{
public:
	RangeSlider(AppDataPtr data, float& refVar, sf::Color* baseColor, sf::Color* highlightColor);

	void highlight() override;
	void unhighlight() override;
	void update() override;
	bool processInput(const InputManager& input, const sf::RenderWindow& window) override;

	sf::FloatRect getGlobalBounds() const override;
	int getHeight() const override;

	float getValue() const;
	void setValue(float val) override;
	void moveValue(float val) override;

public:
	bool dragging = false;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	float& m_value;
	sf::Text m_valueText;
	sf::Sprite m_barSpriteLeft, m_barSpriteRight, m_sliderSprite;
	sf::Vector2f m_itemDimensions;
};

class MenuItem : public sf::Drawable
{
public:
	MenuItem(const sf::Text& text_in, sf::Color* baseColor, sf::Color* highlightColor);

	void setSubItemTextSize(sf::Uint32 subTextSize = 40);
	SubItem* getSubItem(int index);
	bool handleInput(const InputManager& input, const sf::RenderWindow& window, int& itemHighlighted, int& subItemHighlighted);
	void moveAll(int x, int y);
	void setLineSpacing(int lineSpacing);
	bool isSelectable() const;
	void open();
	void close();
	bool isOpen() const;
	int update();
	void setPosition(const sf::Vector2f& position);
	void setPosition(const float x, const float y);
	const sf::Vector2f& getPosition() const;
	void setString(const sf::String& string);
	void highlight();
	void unhighlight();
	void setStyle(sf::Uint32 style);
	const sf::FloatRect getGlobalBounds() const;
	bool hasSubItems() const;
	size_t subMenuSize() const;
	const std::vector<std::shared_ptr<SubItem>>& getSubItems() const;
	void setSelectable(bool selectable);
	void setIndex(sf::Uint8 index);
	const sf::Uint8 getIndex() const;

	template<typename T>
	void addSubItem(T object);

	template<typename T>
	static void centerItem(T& object, float yPosition = -1);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool m_subMenuOpened = false;
	bool m_selectable = false;
	sf::Uint8 m_index;
	sf::Text m_text;
	int m_subMenuCurHeight = 0, m_subMenuMaxHeight = 0, m_lineSpacing = 100;
	sf::Uint32 m_subItemTextSize = 40;
	sf::Color* m_baseColor, *m_highlightColor;

	std::vector <std::shared_ptr<SubItem>> m_subItems;
};


template<typename T>
inline void MenuItem::centerItem(T& object, float yPosition)
{
	if (yPosition == -1.f)
		yPosition = object.getPosition().y;
	object.setPosition(SCRN_WIDTH / 2.0f - object.getGlobalBounds().width / 2.0f,
		yPosition);
}

template<typename T>
inline void MenuItem::addSubItem(T object)
{
	float itemPosition;
	if (m_subItems.empty())
	{
		const auto& prevLine = m_text.getGlobalBounds();
		itemPosition = prevLine.top + prevLine.height + m_lineSpacing;
		m_subMenuMaxHeight = m_lineSpacing;
	}
	else {
		const auto& prevLine = m_subItems.back()->getGlobalBounds();
		itemPosition = prevLine.top + prevLine.height + m_lineSpacing;
	}
	object.setStyle(m_text.getStyle());
	MenuItem::centerItem(object, itemPosition);
	m_subMenuMaxHeight += object.getHeight() + m_lineSpacing;

	m_subItems.push_back(std::make_shared<T>(object));
}
