#pragma once

#include <SFML/Graphics.hpp>

#include <array>

#include "Application.h"


class StarField : public sf::Drawable, public sf::Transformable
{
private:
	struct Layer
	{
		Layer() = default;
		Layer(const sf::Texture& texture, sf::IntRect texRect, float parallax, int alpha = 255) :
			alpha(alpha), parallaxFactor(parallax), texWidth(texRect.width), texHeight(texRect.height)
		{
			sprite1 = sf::Sprite(texture, texRect);
			sprite2 = sf::Sprite(texture, texRect);
			sprite1.setColor(sf::Color(255, 255, 255, alpha));
			sprite2.setColor(sf::Color(255, 255, 255, alpha));
		}
		Layer(const sf::Texture& texture, float parallax, int alpha = 255, bool invert = false) :
			alpha(alpha), parallaxFactor(parallax), texWidth(texture.getSize().x), texHeight(texture.getSize().y)
		{
			if (invert)
			{
				sf::IntRect texRect(texWidth, texHeight, -texWidth, -texHeight);
				sprite1 = sf::Sprite(texture, texRect);
				sprite2 = sf::Sprite(texture, texRect);
			}
			else
			{
				sprite1 = sf::Sprite(texture);
				sprite2 = sf::Sprite(texture);
			}
			sprite1.setColor(sf::Color(255, 255, 255, alpha));
			sprite2.setColor(sf::Color(255, 255, 255, alpha));
		}
		sf::Sprite sprite1, sprite2;
		sf::Vector2f pos;
		int alpha;
		int texWidth, texHeight;
		float parallaxFactor;
	};
public:
	bool init(AppDataRef data)
	{
		AssetManager& assets = data->assets;
		if (!assets.loadTexture("haze", "images/haze.png"))
			return false;
		if (!assets.loadTexture("star_field", "images/starfield.png"))
			return false;
	
		sf::Texture& texRef = assets.getTexture("star_field");
		sf::Vector2i dimensions(1200, 800);
		m_layers[7] = Layer(texRef, sf::IntRect(sf::Vector2i(2400, 800), dimensions), 1.0f);
		m_layers[6] = Layer(texRef, sf::IntRect(sf::Vector2i(1200, 800), dimensions), 0.9f);
		m_layers[5] = Layer(texRef, sf::IntRect(sf::Vector2i(0,    800), dimensions), 0.8f);
		m_layers[4] = Layer(texRef, sf::IntRect(sf::Vector2i(2400,   0), dimensions), 0.7f);
		m_layers[3] = Layer(texRef, sf::IntRect(sf::Vector2i(1200,   0), dimensions), 0.6f);
		m_layers[2] = Layer(texRef, sf::IntRect(sf::Vector2i(0,      0), dimensions), 0.5f);
		m_layers[1] = Layer(assets.getTexture("haze"), 0.6f, 24);
		m_layers[0] = Layer(assets.getTexture("haze"), 0.3f, 24, true);

		return true;
	}

	void update(const sf::Vector2f& offset)
	{
		for (auto& layer: m_layers)
		{
			layer.pos += offset * layer.parallaxFactor;
			if (layer.pos.x + layer.texWidth < 0.0f)
			{
				layer.pos += sf::Vector2f((float)layer.texWidth, 0.0f);
			}
			layer.sprite1.setPosition(layer.pos);
			sf::Vector2f newPos(layer.pos.x + layer.texWidth, layer.pos.y);
			layer.sprite2.setPosition(newPos);
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (auto& layer : m_layers)
		{
			target.draw(layer.sprite1, states);
			target.draw(layer.sprite2, states);
		}
	}

	std::array<Layer, 8> m_layers;
};
