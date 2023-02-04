#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "MenuItems.h"

#include <vector>

class MenuState : public GameState
{
public:
	MenuState(AppDataPtr data, GameDataPtr gameData);
	~MenuState();

	void init() override = 0;
	void cleanup() override = 0;

	void pause() override = 0;
	void resume() override = 0;

	void processInput() override;

	void update(float dt) override;
	void draw(float dt) override;

protected:
	void setTitle(const char* string, float yPos, sf::Font* font, sf::Uint32 style = sf::Text::Style::Bold,
		const sf::Color& color = sf::Color::Yellow, const sf::Color& outlineColor = sf::Color::Transparent,
		unsigned int textSize = 70, float letterSpacing = 1.f);
	void setDefaultAttribs(int style, int textSize = 44, int subItemTextSize = 40);
	void setDefaultColor(const sf::Color& color);
	void setHighlightColor(const sf::Color& color);
	void setTopItemPos(int yPosition);
	void setDefaultLineSpacing(int spacing);

	void addMenuItem(const char* name, sf::Uint32 style = sf::Text::Style::Bold, sf::Uint32 textSize = 44);
	void addSubItem(const sf::String& string, bool selectable = true, bool visible = true);
	void addSlider(float& ref);
	float nextMenuItemPosition();
	void highlightNextItem(bool subMenuOpen);
	void highlightPrevItem(bool subMenuOpen);
	void adjustSlider(float val);
	void selectItem();

	void setClearColor(sf::Color color) { m_clearColor = color; }
	virtual void onEscapePressed() {};
	virtual void drawBackground() {};
	virtual void processInputImpl() {};
	virtual void updateImpl() {};
protected:
	sf::Uint32 m_style = sf::Text::Style::Bold;
	int m_topItemPos = 400, m_textSize = 44, m_lineSpacing = 10, m_subItemTextSize = 40;
	int m_itemSelected = -1, m_subItemSelected = -1;
	int m_itemHighlighted = 0, m_subItemHighlighted = 0;
	int m_subMenuLastOpened = -1;
	sf::Text m_titleText;
	SoundRef m_clickSound;
	AppDataPtr m_appData;
	sf::Font* m_font;
	sf::Color m_baseColor = sf::Color::White;
	sf::Color m_highlightColor = sf::Color::Blue;
	sf::Color m_clearColor = sf::Color(1, 20, 51);
	sf::RenderWindow& m_window;
	std::vector<MenuItem> m_menuItems;
};
