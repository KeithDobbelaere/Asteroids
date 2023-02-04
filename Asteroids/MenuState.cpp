#include "MenuState.h"

#include <algorithm>
#include <iostream>


MenuState::MenuState(AppDataPtr data, GameDataPtr gameData) :
	m_appData(data), m_window(data->window), m_font(m_appData->assets.getFont("default"))
{
	m_clickSound = m_appData->assets.linkSoundRef("click");
}

MenuState::~MenuState()
{
}

void MenuState::processInput()
{
	m_appData->input.update(m_window, m_appData->view);

	processInputImpl();

	auto& item = m_menuItems[m_itemHighlighted];
	const auto& input = m_appData->input;
	const auto& lastKey = input.lastKeyPressed();
	switch (lastKey)
	{
	case sf::Keyboard::Key::Escape:
		if (item.isOpen())
		{
			item.close();
			m_subItemHighlighted = 0;
		}
		else
			onEscapePressed();
		break;
	case sf::Keyboard::Key::Enter:
		if (item.isOpen() && item.getSubItem(m_subItemHighlighted)->type == SubItem::Type::RangeSlider)
			item.close();
		else
			selectItem();
		break;
	case sf::Keyboard::Key::Up:
		highlightPrevItem(item.isOpen());
		break;
	case sf::Keyboard::Key::Tab:
		if (input.isKeyDown(sf::Keyboard::Key::LShift) || input.isKeyDown(sf::Keyboard::Key::RShift))
			highlightPrevItem(item.isOpen());
		break;
	case sf::Keyboard::Key::Down:
		highlightNextItem(item.isOpen());
		break;
	case sf::Keyboard::Key::Left:
		adjustSlider(-.05f);
		break;
	case sf::Keyboard::Key::Right:
		adjustSlider(.05f);
		break;
	}

	for (auto& item : m_menuItems)
	{
		item.unhighlight();
	
		bool wasSelected = item.handleInput(input, m_window, m_itemHighlighted, m_subItemHighlighted);
		if (wasSelected)
		{
			selectItem();
			break;
		}
	}
	if (m_itemHighlighted < 0)
		m_itemHighlighted = (int)m_menuItems.size() - 1;
	if (m_itemHighlighted >= (int)m_menuItems.size())
		m_itemHighlighted = 0;

	auto& highlightedItem = m_menuItems[m_itemHighlighted];
	if (m_subItemHighlighted < 0)
		m_subItemHighlighted = (int)highlightedItem.subMenuSize() - 1;
	if (m_subItemHighlighted >= (int)highlightedItem.subMenuSize())
		m_subItemHighlighted = 0;


}

void MenuState::update(float dt)
{
	updateImpl();

	auto& highlightedItem = m_menuItems[m_itemHighlighted];
	highlightedItem.highlight();
	if (highlightedItem.isSelectable())
	{
		const auto& highlightedSubItem = highlightedItem.getSubItem(m_subItemHighlighted);
		highlightedSubItem->highlight();
	}
	for (auto it1 = m_menuItems.begin(); it1 < m_menuItems.end(); ++it1)
	{
		//Show/hide sub-items
		auto& item = *it1;
		if (int direction = item.update())
		{
			//Move concurrent items up/down accordingly
			for (auto it2 = it1 + 1; it2 < m_menuItems.end(); ++it2)
			{
				auto& lowerItem = *it2;
				lowerItem.moveAll(0, direction);
			}
		}
	}
}

void MenuState::draw(float dt)
{
	m_window.clear(m_clearColor);
	m_window.setView(m_appData->view);

	drawBackground();
	m_window.draw(m_titleText);
	for (const auto& item : m_menuItems)
		m_window.draw(item);

	m_window.display();
}

void MenuState::setTitle(const char* name, float yPos, sf::Font* font, sf::Uint32 style, const sf::Color& color, const sf::Color& outlineColor,
	unsigned int textSize, float letterSpacing)
{
	m_titleText = sf::Text(name, *font, textSize);
	m_titleText.setLetterSpacing(letterSpacing);
	m_titleText.setFillColor(color);
	if (outlineColor != sf::Color::Transparent)
	{
		m_titleText.setOutlineColor(outlineColor);
		m_titleText.setOutlineThickness(3.f);
	}
	m_titleText.setStyle(style);
	m_titleText.setPosition((float)SCRN_WIDTH / 2.f -
		m_titleText.getGlobalBounds().width / 2.f, yPos);
}

void MenuState::setDefaultAttribs(int style, int textSize, int subItemTextSize)
{
	m_style = style;
	m_textSize = textSize;
	m_subItemTextSize = subItemTextSize;
}

void MenuState::setDefaultColor(const sf::Color& color)
{
	m_baseColor = color;
}

void MenuState::setHighlightColor(const sf::Color& color)
{
	m_highlightColor = color;
}

void MenuState::setTopItemPos(int yPosition)
{
	m_topItemPos = yPosition;
}

void MenuState::setDefaultLineSpacing(int spacing)
{
	m_lineSpacing = spacing;
}

void MenuState::addMenuItem(const char* name, sf::Uint32 style, sf::Uint32 textSize)
{
	float itemPosition;
	if (m_menuItems.empty())
		itemPosition = (float)m_topItemPos;
	else {
		itemPosition = nextMenuItemPosition();
	}
	m_menuItems.emplace_back(sf::Text(name, *m_font, m_textSize), &m_baseColor, &m_highlightColor);
	auto& lastItem = m_menuItems.back();
	lastItem.setStyle(m_style);
	lastItem.setLineSpacing(m_lineSpacing);
	lastItem.setSubItemTextSize(m_subItemTextSize);
	MenuItem::centerItem(lastItem, itemPosition);
	lastItem.setIndex((sf::Uint8)m_menuItems.size() - 1);
}

void MenuState::addSubItem(const sf::String& string, bool selectable, bool visible)
{
	TextItem text(sf::Text(string, *m_font, m_subItemTextSize), &m_baseColor, &m_highlightColor);
	text.visible = visible;
	auto& item = m_menuItems.back();
	item.addSubItem(text);
	item.setSelectable(selectable);
}

void MenuState::addSlider(float& ref)
{
	RangeSlider slider(m_appData, ref, &m_baseColor, &m_highlightColor);
	auto& item = m_menuItems.back();
	item.addSubItem(slider);
	item.setSelectable(true);
}

float MenuState::nextMenuItemPosition()
{
	const auto& lastItem = m_menuItems.back();
	float menuItemTextPos = lastItem.getGlobalBounds().top;
	float menuItemTextHeight = lastItem.getGlobalBounds().height;
	return menuItemTextPos + menuItemTextHeight + m_lineSpacing;
}

void MenuState::highlightNextItem(bool subMenuOpen)
{
	if (subMenuOpen)
		m_subItemHighlighted++;
	else
		m_itemHighlighted++;
}

void MenuState::highlightPrevItem(bool subMenuOpen)
{
	if (subMenuOpen)
		m_subItemHighlighted--;
	else
		m_itemHighlighted--;
}

void MenuState::adjustSlider(float val)
{
	MenuItem& item = m_menuItems[m_itemHighlighted];
	if (item.hasSubItems())
		if (item.getSubItem(m_subItemHighlighted)->type == SubItem::Type::RangeSlider)
		{
			item.getSubItem(m_subItemHighlighted)->moveValue(val);
			m_itemSelected = m_itemHighlighted;
			m_subItemSelected = m_subItemHighlighted;
		}
	m_clickSound->play();
}

void MenuState::selectItem()
{
	auto& highlightedItem = m_menuItems[m_itemHighlighted];
	if (highlightedItem.hasSubItems())	//currently highlighted item has sub-items:
	{
		if (!highlightedItem.isOpen())			//but sub-menu not opened:
		{
			if (m_subMenuLastOpened != -1) {
				auto& lastItemOpened = m_menuItems[m_subMenuLastOpened];
				lastItemOpened.close();					//close last sub-menu opened, if exists
			}
			highlightedItem.open();						//open currently highlighted sub-menu
			m_subItemHighlighted = 0;					//highlight first item
			m_subMenuLastOpened = m_itemHighlighted;	//update last sub-menu opened
		}
		else if (highlightedItem.isSelectable())//sub-menu opened and is selectable:
		{
			m_itemSelected = m_itemHighlighted;			//select item
			m_subItemSelected = m_subItemHighlighted;	//select sub-item
		}
		else									//sub-menu opened and non-selectable:
		{
			highlightedItem.close();					//close currently highlighted sub-menu
		}
	}
	else								//currently highlighted item has no sub-items:
	{
		m_itemSelected = m_itemHighlighted;		//select item
	}
	m_clickSound->play();
}
