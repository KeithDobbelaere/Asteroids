#pragma once
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Application.h"
#include "MenuSubItem.h"

#include <vector>

class MenuState : public GameState
{
public:
	MenuState(AppDataRef data, GameDataRef gameData);
	~MenuState();

	void init() override = 0;
	void cleanup() override = 0;

	void pause() override = 0;
	void resume() override = 0;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;


protected:
	void selectItem();
	void setTitle(const char* string, float yPos, sf::Font* font, int style = sf::Text::Style::Bold,
		const sf::Color& color = sf::Color::Yellow, const sf::Color& outlineColor = sf::Color::Transparent,
		unsigned int textSize = 70, float letterSpacing = 1.f);
	void addMenuItem(const char* string);
	void addSubItem(const sf::String& string, bool selectable = true, bool visible = false);
	void addSlider(float& referenceVar);
	void adjustSlider(float val);

	void setLineSpacing(int lineSpacing) { m_lineSpacing = lineSpacing; }
	void setTopItemPos(int yPosition) { m_topItemPos = yPosition; }
	void setDefaultColor(const sf::Color& color) { m_defaultColor = color; }
	void setHighlightColor(const sf::Color& color) { m_highlightColor = color; }
	void setClearColor(sf::Color color) { m_clearColor = color; }
	void setItemAttribs(int style, int textSize = 44, int subItemTextSize = 40);

	virtual void onEscapePressed() {};
	virtual void drawBackground() {};
	virtual void updateImpl() {};

	template<typename T>
	void centerItem(T& object, float yPosition = -1);

protected:
	int m_topItemPos = 200, m_lineSpacing = 100, m_itemTextSize = 44, m_subItemTextSize = 40;
	int m_itemSelected = -1, m_itemHighlighted = 0, m_subItemSelected = -1, m_subItemHighlighted = 0;
	int m_subMenuLastOpened = -1;
	int m_itemStyle = sf::Text::Bold;
	sf::Vector2f m_lastMousePos;
	AppDataRef m_data;
	sf::Font* m_font;
	sf::Text m_titleText;

	std::vector<MenuItem> m_menuItems;
	sf::Color m_highlightColor = sf::Color::Blue;
	sf::Color m_defaultColor = sf::Color::White;
	sf::Color m_clearColor = sf::Color(1, 20, 51);
	SoundRef m_clickSound;
	sf::RenderWindow& m_window;
};

template<typename T>
inline void MenuState::centerItem(T & object, float yPosition)
{
	if (yPosition == -1.f)
		yPosition = object.getPosition().y;
	object.setPosition((float)m_data->window.getSize().x / 2.0f -
		object.getGlobalBounds().width / 2.0f, yPosition);
}
