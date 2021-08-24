#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <functional>

class InputManager
{
public:
	InputManager() :
		m_resizeViewFunPtr(nullptr) {}
	InputManager(sf::Vector2f(* resizeCallback) (sf::View&, sf::Vector2u, float&)) :
		m_resizeViewFunPtr(resizeCallback) {}
	InputManager(std::function<sf::Vector2f(sf::View&, sf::Vector2u, float&)>resizeCallback) :
		m_resizeViewFunPtr(resizeCallback) {}
	~InputManager() = default;


	void update(sf::RenderWindow& window, sf::View& view);

	bool windowSizeChanged(const sf::RenderWindow& window) const;

	template<typename T>
	bool isItemHovered(const T& object, const sf::RenderWindow& window) const;
	template<typename T>
	bool wasItemClicked(const T& object, sf::Mouse::Button button, const sf::RenderWindow& window) const;
	template<typename T>
	bool isItemDragged(const T& object, sf::Mouse::Button button, const sf::RenderWindow& window) const;

	sf::Vector2f getMousePosition(const sf::RenderWindow& window) const;

	bool mousePositionChanged() const;
	bool wasKeyPressed(sf::Keyboard::Key keys) const;
	bool isKeyDown(sf::Keyboard::Key keys) const;
	bool anyKeyPressed() const;

	bool wasButtonPressed(sf::Mouse::Button button) const;
	bool isButtonDown(sf::Mouse::Button button) const;
	sf::Keyboard::Key lastKeyPressed() const;

	uint8_t getCharTyped() const;

private:
	void calculateScale();

	bool m_windowResized = false, m_hasFocus = true;
	uint8_t m_lastCharTyped = '\0';

	sf::Event m_event {};
	std::unordered_map<sf::Keyboard::Key, bool> m_keyStates;
	std::unordered_map<sf::Mouse::Button, bool> m_buttonStates;
	sf::Keyboard::Key m_lastKeyPressed = sf::Keyboard::Key::Unknown;
	std::function<sf::Vector2f(sf::View&, sf::Vector2u, float&)> m_resizeViewFunPtr;
	sf::Vector2i m_mousePosition{}, m_lastMousePos{};
	sf::Vector2f viewOffsets{};
	float viewScaleFactor = 1;
};

template<typename T>
inline bool InputManager::isItemHovered(const T& object, const sf::RenderWindow & window) const
{
	const auto& tempRect = object.getGlobalBounds();
	return tempRect.contains(getMousePosition(window));
}

template<typename T>
inline bool InputManager::wasItemClicked(const T& object, sf::Mouse::Button button, const sf::RenderWindow & window) const
{
	if (wasButtonPressed(button))
	{
		return isItemHovered(object, window);
	}
	return false;
}

template<typename T>
inline bool InputManager::isItemDragged(const T & object, sf::Mouse::Button button, const sf::RenderWindow & window) const
{
	if (isButtonDown(button))
	{
		return isItemHovered(object, window);
	}
	return false;
}
