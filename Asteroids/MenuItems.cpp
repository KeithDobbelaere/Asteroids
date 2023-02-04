#include "MenuItems.h"


TextItem::TextItem(sf::Text text, sf::Color* baseColor, sf::Color* highlightColor)
	: SubItem(baseColor, highlightColor), m_text(text)
{
	type = Type::Text;
}

void TextItem::highlight()
{
	m_text.setFillColor(*m_highlightColor);
}

void TextItem::unhighlight()
{
	m_text.setFillColor(*m_baseColor);
}

void TextItem::update()
{
	m_text.update();
}

bool TextItem::processInput(const InputManager& input, const sf::RenderWindow& window)
{
	if (input.wasItemClicked(*this, sf::Mouse::Button::Left, window))
		return true;
	return false;
}

sf::FloatRect TextItem::getGlobalBounds() const
{
	const auto& tempRect = m_text.getGlobalBounds();
	return sf::FloatRect(tempRect.left + this->getPosition().x, tempRect.top + this->getPosition().y,
		tempRect.width, tempRect.height);
}

int TextItem::getHeight() const
{
	return m_text.getCharacterSize();
}

void TextItem::setString(const sf::String& newString)
{
	m_text.setString(newString);
}

void TextItem::setStyle(sf::Uint32 style)
{
	m_text.setStyle(style);
}

void TextItem::select(bool state)
{
	m_text.effect(state);
}

void TextItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_text, states);
}

RangeSlider::RangeSlider(AppDataPtr data, float & refVar, sf::Color* baseColor, sf::Color* highlightColor) 
	: SubItem(baseColor, highlightColor), m_value(refVar), m_itemDimensions(312, 50)
{
	type = Type::RangeSlider;

	auto& assets = data->assets;
	m_valueText = sf::Text("100", *assets.getFont("default"), 20);
	m_valueText.setFillColor(sf::Color::Black);

	const auto& texture = data->assets.getTexture("slider");

	sf::IntRect tempRect(0, 0, 300, 12);
	m_barSpriteLeft.setTexture(texture);
	m_barSpriteLeft.setOrigin(300.0f / 2, 12.f / 2);
	m_barSpriteLeft.setTextureRect(tempRect);
	m_barSpriteLeft.setPosition(sf::Vector2f(312.f / 2, 50.f / 2));
	m_barSpriteLeft.setColor(*m_baseColor);
	
	m_barSpriteRight.setTexture(texture);
	m_barSpriteRight.setOrigin(300.f / 2, 12.f / 2);
	m_barSpriteRight.setTextureRect(tempRect);
	m_barSpriteRight.setPosition(sf::Vector2f(312.f / 2, 50.f / 2));
	sf::Uint32 rightBarColor = ext::multiply(*m_baseColor, .6f).integer;
	m_barSpriteRight.setColor(sf::Color(rightBarColor));

	tempRect = sf::IntRect(162, 15, 50, 32);
	m_sliderSprite.setTexture(texture);
	m_sliderSprite.setOrigin(50.f / 2, 32.f / 2);
	m_sliderSprite.setTextureRect(tempRect);
	m_sliderSprite.setColor(*m_baseColor);

	needsUpdate = true;
}

void RangeSlider::highlight()
{
	m_sliderSprite.setColor(*m_highlightColor);
	m_sliderSprite.setScale(1.1f, 1.2f);
}

void RangeSlider::unhighlight()
{
	m_sliderSprite.setColor(*m_baseColor);
	m_sliderSprite.setScale(1.f, 1.f);
}

void RangeSlider::update()
{
	if (needsUpdate)
	{
		sf::Vector2f sliderPos(m_value * 260.f + 26, 50.f / 2);
		m_barSpriteLeft.setTextureRect(sf::IntRect(0, 0, (int)sliderPos.x, 12));

		m_sliderSprite.setPosition(sliderPos);
		m_valueText.setString(std::to_string((int)(m_value * 100)));
		const auto& tempRect = m_valueText.getLocalBounds();
		m_valueText.setOrigin(tempRect.width / 2 + tempRect.left, tempRect.height / 2 + tempRect.top);
		m_valueText.setPosition(sf::Vector2f(m_value * 260.f + 26, 50.f / 2));
		needsUpdate = false;
	}
}

bool RangeSlider::processInput(const InputManager& input, const sf::RenderWindow& window)
{
	sf::Vector2f mousePos = input.getMousePosition(window);
	m_sliderSprite.move(this->getPosition());
	m_barSpriteRight.move(this->getPosition());
	bool barClicked = input.wasItemClicked(m_barSpriteRight, sf::Mouse::Button::Left, window);
	bool sliderClicked = input.wasItemClicked(m_sliderSprite, sf::Mouse::Button::Left, window);
	const auto barRect = m_barSpriteRight.getGlobalBounds();
	m_sliderSprite.move(-(this->getPosition()));
	m_barSpriteRight.move(-(this->getPosition()));
	if (barClicked || sliderClicked)
	{
		float offset = mousePos.x - barRect.left;
		float newValue = offset / barRect.width;
		setValue(newValue);
		dragging = true;
	}
	else if (dragging)
	{
		if (!input.isButtonDown(sf::Mouse::Button::Left))
		{
			dragging = false;
			return true; //select
		}
		if (input.isItemHovered(*this, window))
		{
			float offset = mousePos.x - barRect.left;
			float newValue = offset / barRect.width;
			newValue = std::fmin(std::fmax(0.f, newValue), 1.f);
			setValue(newValue);
		}
	}
	return false;
}

sf::FloatRect RangeSlider::getGlobalBounds() const
{
	const auto& dims = m_itemDimensions;
	return sf::FloatRect(this->getPosition().x, this->getPosition().y,
		dims.x, dims.y);
}

int RangeSlider::getHeight() const
{
	return (int)m_itemDimensions.y;
}

float RangeSlider::getValue() const
{
	return m_value;
}

void RangeSlider::setValue(float val)
{
	m_value = val;
	needsUpdate = true;
}

void RangeSlider::moveValue(float val)
{
	m_value += val; 
	if (m_value < 0.f)
		m_value = 0.f;
	if (m_value > 1.f)
		m_value = 1.f;
	needsUpdate = true;
}

void RangeSlider::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_barSpriteRight, states);
	target.draw(m_barSpriteLeft, states);
	target.draw(m_sliderSprite, states);
	target.draw(m_valueText, states);
}

MenuItem::MenuItem(const sf::Text& text_in, sf::Color* baseColor, sf::Color* highlightColor)
	: m_text(text_in), m_index(0), m_baseColor(baseColor), m_highlightColor(highlightColor) {}

void MenuItem::setSubItemTextSize(sf::Uint32 subTextSize)
{
	m_subItemTextSize = subTextSize;
}

SubItem* MenuItem::getSubItem(int index)
{
	return m_subItems.at(index).get();
}

bool MenuItem::handleInput(const InputManager& input, const sf::RenderWindow& window, int& itemHighlighted, int& subItemHighlighted)
{
	if (input.mousePositionChanged() && input.isItemHovered(*this, window))
	{
		itemHighlighted = m_index;
	}
	if (input.wasItemClicked(*this, sf::Mouse::Button::Left, window))
	{
		if (this->isOpen())
		{
			this->close();
		}
		else
		{
			itemHighlighted = m_index;
			return true;
		}
	}
	if (this->isOpen())
	{
		int i = 0;
		for (const auto& subItem : m_subItems)
		{
			subItem->unhighlight();
			if (input.isItemHovered(*subItem, window))
			{
				itemHighlighted = m_index;
				subItemHighlighted = i;
			}
			if (subItem->processInput(input, window))
			{
				return true;
			}
			i++;
		}
	}
	return false;
}

void MenuItem::moveAll(int x, int y)
{
	m_text.move((float)x, (float)y);
	for (auto& subItem : m_subItems)
	{
		subItem->move((float)x, (float)y);
	}
}

void MenuItem::setLineSpacing(int lineSpacing)
{
	m_lineSpacing = lineSpacing;
}

bool MenuItem::isSelectable() const
{
	return m_selectable;
}

void MenuItem::open()
{
	m_subMenuOpened = true;
}

void MenuItem::close()
{
	m_subMenuOpened = false;
}

bool MenuItem::isOpen() const
{
	return m_subMenuOpened;
}

int MenuItem::update()
{
	int direction = 0;
	if (m_subMenuOpened && m_subMenuCurHeight < m_subMenuMaxHeight)
		direction = 8;
	if (!m_subMenuOpened && m_subMenuCurHeight > 0)
		direction = -8;
	if (direction != 0)
	{
		m_subMenuCurHeight += direction;
		if (direction + m_subMenuCurHeight > m_subMenuMaxHeight)
		{
			direction -= (m_subMenuMaxHeight - m_subMenuCurHeight);
			m_subMenuCurHeight = m_subMenuMaxHeight;
		}
		else if (direction + m_subMenuCurHeight < 0)
		{
			direction += m_subMenuCurHeight;
			m_subMenuCurHeight = 0;
		}
	}
	int k = 1;
	for (auto& subItem : m_subItems)
	{
		subItem->visible = (m_subMenuCurHeight >= (m_lineSpacing + subItem->getHeight()) * k);
		subItem->update();
		k++;
	}
	return direction;
}

void MenuItem::setPosition(const sf::Vector2f& position)
{
	m_text.setPosition(position);
}

void MenuItem::setPosition(const float x, const float y)
{
	m_text.setPosition(x, y);
}

const sf::Vector2f& MenuItem::getPosition() const
{
	return m_text.getPosition();
}

void MenuItem::setString(const sf::String& string)
{
	m_text.setString(string);
}

void MenuItem::highlight()
{
	m_text.setFillColor(*m_highlightColor);
}

void MenuItem::unhighlight()
{
	m_text.setFillColor(*m_baseColor);
}

void MenuItem::setStyle(sf::Uint32 style)
{
	m_text.setStyle(style);
}

const sf::FloatRect MenuItem::getGlobalBounds() const
{
	return m_text.getGlobalBounds();
}

bool MenuItem::hasSubItems() const
{
	return m_subItems.size() > 0;
}

size_t MenuItem::subMenuSize() const
{
	return m_subItems.size();
}

const std::vector<std::shared_ptr<SubItem>>& MenuItem::getSubItems() const
{
	return m_subItems;
}

void MenuItem::setSelectable(bool selectable)
{
	m_selectable = selectable;
}

void MenuItem::setIndex(sf::Uint8 index)
{
	m_index = index;
}

const sf::Uint8 MenuItem::getIndex() const
{
	return m_index;
}

void MenuItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_text, states);

	if (hasSubItems())
	{
		for (const auto& subItem : m_subItems)
		{
			if (subItem->visible)
				target.draw(*subItem, states);
		}
	}
}
