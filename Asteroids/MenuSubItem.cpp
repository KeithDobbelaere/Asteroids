#include "MenuSubItem.h"


RangeSlider::RangeSlider(AppDataRef data, float & refVar, const sf::Color& baseColor, const sf::Color& highlightColor, const sf::Color& accentColor) :
	m_value(refVar), m_accentColor(accentColor), m_baseColor(baseColor), m_highlightColor(highlightColor)
{
	type = Type::RangeSlider;
	auto& assets = data->assets;
	m_valueText = sf::Text("100", *assets.getFont("default"), 20);
	m_valueText.setFillColor(sf::Color::Black);
	data->assets.loadTexture("slider", "images/range_slider.png");

	data->assets.getTexture("slider").setSmooth(true);

	sf::IntRect tempRect(0, 0, 300, 12);
	m_barSpriteLeft.setTexture(assets.getTexture("slider"));
	m_barSpriteLeft.setOrigin(300.0f / 2, 12.f / 2);
	m_barSpriteLeft.setTextureRect(tempRect);
	m_barSpriteLeft.setPosition(sf::Vector2f(312.f / 2, 50.f / 2));
	m_barSpriteLeft.setColor(m_accentColor);
	
	m_barSpriteRight.setTexture(assets.getTexture("slider"));
	m_barSpriteRight.setOrigin(300.f / 2, 12.f / 2);
	m_barSpriteRight.setTextureRect(tempRect);
	m_barSpriteRight.setPosition(sf::Vector2f(312.f / 2, 50.f / 2));
	int r = (int)(m_accentColor.r * .6f),
		g = (int)(m_accentColor.g * .6f),
		b = (int)(m_accentColor.b * .6f);
	m_barSpriteRight.setColor(sf::Color(r, g, b));

	tempRect = sf::IntRect(162, 15, 50, 32);
	m_sliderSprite.setTexture(assets.getTexture("slider"));
	m_sliderSprite.setOrigin(50.f / 2, 32.f / 2);
	m_sliderSprite.setTextureRect(tempRect);
	m_sliderSprite.setColor(m_highlightColor);

	m_base = sf::RectangleShape(sf::Vector2f(312, 50));
	m_base.setFillColor(m_baseColor);
	needsUpdate = true;
	m_returnObject1 = m_returnObject2 = m_base;
}

void RangeSlider::highlight()
{
	m_sliderSprite.setColor(m_highlightColor);
	m_sliderSprite.setScale(1.1f, 1.2f);
}

void RangeSlider::unhighlight()
{
	m_sliderSprite.setColor(m_accentColor);
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

sf::FloatRect RangeSlider::getGlobalBounds() const
{
	const auto& tempRect = m_base.getGlobalBounds();
	return sf::FloatRect(tempRect.left + this->getPosition().x, tempRect.top + this->getPosition().y,
		tempRect.width, tempRect.height);
}

const sf::RectangleShape& RangeSlider::getBarElement()
{
	const auto& pos = this->getPosition();
	const auto& p = m_barSpriteRight.getPosition();
	const auto& tempRect = m_barSpriteRight.getGlobalBounds();
	const auto& origin = m_barSpriteRight.getOrigin();
	m_returnObject1.setSize({ tempRect.width, tempRect.height });
	m_returnObject1.setOrigin(origin);
	m_returnObject1.setPosition(p.x + pos.x, p.y + pos.y);
	return m_returnObject1;
}

const sf::RectangleShape& RangeSlider::getSliderElement()
{
	const auto& pos = this->getPosition();
	const auto& p = m_sliderSprite.getPosition();
	const auto& tempRect = m_sliderSprite.getGlobalBounds();
	const auto& origin = m_sliderSprite.getOrigin();
	const auto& scale = m_sliderSprite.getScale();
	m_returnObject2.setSize({ tempRect.width, tempRect.height });
	m_returnObject2.setOrigin(origin);
	m_returnObject2.setPosition(p.x + pos.x, p.y + pos.y);
	m_returnObject2.setScale(scale);
	return m_returnObject2;
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

	target.draw(m_base, states);

	target.draw(m_barSpriteRight, states);
	target.draw(m_barSpriteLeft, states);
	target.draw(m_sliderSprite, states);
	target.draw(m_valueText, states);
}

Text::Text(sf::Text text, const sf::Color& baseColor, const sf::Color& highlightColor) :
	m_text(text), m_baseColor(baseColor), m_highlightColor(highlightColor)
{
	type = Type::Text;
}

void Text::update()
{
}

sf::FloatRect Text::getGlobalBounds() const
{
	const auto& tempRect = m_text.getGlobalBounds();
	return sf::FloatRect(tempRect.left + this->getPosition().x, tempRect.top + this->getPosition().y,
		tempRect.width, tempRect.height);
}

void Text::highlight()
{
	m_text.setFillColor(m_highlightColor);
}

void Text::unhighlight()
{
	m_text.setFillColor(m_baseColor);
}
