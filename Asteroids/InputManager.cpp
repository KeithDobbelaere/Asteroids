#include "InputManager.h"

#include <iostream>


void InputManager::update(sf::RenderWindow & window, sf::View& view)
{
	m_lastCharTyped = '\0';
	m_lastKeyPressed = sf::Keyboard::Key::Unknown;
	m_lastMousePos = m_mousePosition;
	m_windowResized = false;
	for (auto& button : m_buttonStates)
	{
		button.second = false;
	}
	for (auto& keys : m_keyStates)
	{
		keys.second = false;
	}
	while (window.pollEvent(m_event))
	{
		if (m_event.type == sf::Event::Closed)
			window.close();

		if (m_event.type == sf::Event::Resized) {
			m_windowResized = true;
			if (m_resizeViewFunPtr != nullptr)
				viewOffsets = m_resizeViewFunPtr(view, { m_event.size.width, m_event.size.height }, viewScaleFactor);
			calculateScale();
			window.setView(view);
		}

		if (m_event.type == sf::Event::LostFocus)
			m_hasFocus = false;

		if (m_event.type == sf::Event::GainedFocus)
			m_hasFocus = true;

		if (m_event.type == sf::Event::KeyPressed)
		{
			m_lastKeyPressed = m_event.key.code;
			m_keyStates[m_event.key.code] = true;
			if (m_event.key.code != sf::Keyboard::Key::Unknown)
				m_keyStates[sf::Keyboard::Key::KeyCount] = true;
		} 
		else if (m_event.type == sf::Event::MouseButtonPressed)
		{
			m_buttonStates[m_event.mouseButton.button] = true;
		}
		if (m_event.type == sf::Event::TextEntered)
		{
			m_lastCharTyped = (uint8_t)toupper(std::min(std::max((int)m_event.text.unicode, 33), 126));
		}
	}
	m_mousePosition = sf::Mouse::getPosition(window);
}

bool InputManager::windowSizeChanged(const sf::RenderWindow & window) const
{
	return m_windowResized;
}

sf::Vector2f InputManager::getMousePosition(const sf::RenderWindow & window) const
{
	const auto& pos = m_mousePosition;
	return (sf::Vector2f((float)pos.x, (float)pos.y) - viewOffsets) * viewScaleFactor;
}

bool InputManager::mousePositionChanged() const
{
	return (m_mousePosition != m_lastMousePos);
}

bool InputManager::wasKeyPressed(sf::Keyboard::Key keys) const
{
	if (m_keyStates.count(keys) > 0)
		return m_keyStates.at(keys);

	return false;
}

bool InputManager::isKeyDown(sf::Keyboard::Key keys) const
{
	if (m_hasFocus)
		return sf::Keyboard::isKeyPressed(keys);

	return false;
}

bool InputManager::anyKeyPressed() const
{
	if (m_keyStates.count(sf::Keyboard::Key::KeyCount) > 0)
		return m_keyStates.at(sf::Keyboard::Key::KeyCount);

	return false;
}

bool InputManager::wasButtonPressed(sf::Mouse::Button button) const
{
	if (m_buttonStates.count(button) > 0)
		return m_buttonStates.at(button);

	return false;
}

bool InputManager::isButtonDown(sf::Mouse::Button button) const
{
	if (m_hasFocus)
		return sf::Mouse::isButtonPressed(button);

	return false;
}

sf::Keyboard::Key InputManager::lastKeyPressed() const
{
	return m_lastKeyPressed;
}

uint8_t InputManager::getCharTyped() const
{
	return m_lastCharTyped;
}

void InputManager::calculateScale()
{

}
