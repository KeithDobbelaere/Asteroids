#include "MenuState.h"


MenuState::MenuState(AppDataRef data, GameDataRef gameData) :
	m_data(data), m_window(data->window), m_font(m_data->assets.getFont("default"))
{
	m_window.setKeyRepeatEnabled(true);

	m_data->assets.loadSoundBuffer("click", "sounds/click_sound.ogg");
	m_clickSound = m_data->assets.linkSoundRef("click");
}

MenuState::~MenuState()
{
	m_window.setKeyRepeatEnabled(false);
}

void MenuState::processInput()
{
	m_data->input.update(m_window);

	auto& item = m_menuItems[m_itemHighlighted];
	bool& subMenuOpen = item.subMenuOpened;
	const auto& input = m_data->input;
	switch (input.lastKeyPressed())
	{
	case sf::Keyboard::Escape:
		if (subMenuOpen)
		{
			subMenuOpen = false;
			m_subItemHighlighted = 0;
		}
		else
			onEscapePressed();
		break;
	case sf::Keyboard::Key::Enter: 
		if (subMenuOpen && item.subItems[m_subItemHighlighted]->type == SubItem::Type::RangeSlider)
			item.subMenuOpened = false;
		else
			selectItem();
		break;
	case sf::Keyboard::Key::Up:
		if (subMenuOpen)
			m_subItemHighlighted--;
		else
			m_itemHighlighted--;
		break;
	case sf::Keyboard::Down:
		if (subMenuOpen)
			m_subItemHighlighted++;
		else
			m_itemHighlighted++;
		break;
	case sf::Keyboard::Left:
		adjustSlider(-.05f);
		break;
	case sf::Keyboard::Right:
		adjustSlider(.05f);
		break;
	}
	sf::Vector2f mousePos = input.getMousePosition(m_window);
	bool mousePosChanged = (m_lastMousePos != mousePos);
	m_lastMousePos = mousePos;
	int i = 0;
	for (auto& item : m_menuItems)
	{
		item.text.setFillColor(m_defaultColor);
		if (mousePosChanged && input.isItemHovered(item.text, m_window))
		{
			m_itemHighlighted = i;
		}
		if (input.wasItemClicked(item.text, sf::Mouse::Button::Left, m_window))
		{
			if (item.subMenuOpened)
				item.subMenuOpened = false;
			else
				selectItem();
		}
		if (item.subMenuOpened)
		{
			int j = 0;
			for (auto& subItem : item.subItems)
			{
				subItem->unhighlight();
				if (mousePosChanged && input.isItemHovered(*subItem, m_window))
				{
					m_itemHighlighted = i;
					m_subItemHighlighted = j;
				}
				if (subItem->type == SubItem::Type::RangeSlider)
				{
					auto rangeSlider = static_cast<RangeSlider*>(subItem.get());
					const auto& sliderElement = rangeSlider->getSliderElement();
					const auto& barElement = rangeSlider->getBarElement();
					const auto& barRect = barElement.getGlobalBounds();
					if (input.wasItemClicked(barElement, sf::Mouse::Button::Left, m_window) ||
						input.wasItemClicked(sliderElement, sf::Mouse::Button::Left, m_window))
					{
						float offset = mousePos.x - barRect.left;
						float newValue = offset / barRect.width;
						rangeSlider->setValue(newValue);
						rangeSlider->dragging = true;
					}
					else if (rangeSlider->dragging)
					{
						if (input.isItemDragged(*rangeSlider, sf::Mouse::Button::Left, m_window))
						{
							float offset = mousePos.x - barRect.left;
							float newValue = offset / barRect.width;
							newValue  = std::fmin(std::fmax(0.f, newValue), 1.f);
							rangeSlider->setValue(newValue);
						}
						else if (!input.isButtonDown(sf::Mouse::Button::Left))
						{
							rangeSlider->dragging = false;
							selectItem();
						}
						else
						{
							item.text.setFillColor(m_highlightColor);
							rangeSlider->highlight();
							return;
						}
					}
				}
				else if (input.wasItemClicked(*subItem, sf::Mouse::Button::Left, m_window))
				{
					m_itemHighlighted = i;
					m_subItemHighlighted = j;
					if (item.selectable) selectItem();
				}
				j++;
			}
		}
		i++;
	}
	if (m_itemHighlighted < 0) m_itemHighlighted = (int)m_menuItems.size() - 1;
	if (m_itemHighlighted >= (int)m_menuItems.size()) m_itemHighlighted = 0;
	if (m_subItemHighlighted < 0) m_subItemHighlighted = (int)m_menuItems[m_itemHighlighted].subItems.size() - 1;
	if (m_subItemHighlighted >= (int)m_menuItems[m_itemHighlighted].subItems.size()) m_subItemHighlighted = 0;
	m_menuItems[m_itemHighlighted].text.setFillColor(m_highlightColor);
	if (m_menuItems[m_itemHighlighted].selectable)
		m_menuItems[m_itemHighlighted].subItems[m_subItemHighlighted]->highlight();
}

void MenuState::update(float dt)
{
	for (auto it1 = m_menuItems.begin(); it1 < m_menuItems.end(); ++it1)
	{
		auto& item = *it1;
		float moveDir = 0.f;
		if (item.subMenuOpened && item.subMenuCurHeight < item.subMenuMaxHeight)
			moveDir = 8.f;
		if (!item.subMenuOpened && item.subMenuCurHeight > 0)
			moveDir = -8.f;
		if (moveDir != 0.f)
		{
			item.subMenuCurHeight += (int)moveDir;
			if (moveDir + item.subMenuCurHeight > item.subMenuMaxHeight)
			{
				moveDir -= (item.subMenuMaxHeight - item.subMenuCurHeight);
				item.subMenuCurHeight = item.subMenuMaxHeight;
			}
			else if (moveDir + item.subMenuCurHeight < 0)
			{
				moveDir += item.subMenuCurHeight;
				item.subMenuCurHeight = 0;
			}
			//Move concurrent items up/down
			for (auto it2 = it1 + 1; it2 < m_menuItems.end(); ++it2)
			{
				auto& lowerItem = *it2;
				lowerItem.text.move(0.f, moveDir);
				for (auto& subItem : lowerItem.subItems)
				{
					subItem->move(0.f, moveDir);
				}
			}
		}
		int k = 0;
		for (auto& subItem : item.subItems)
		{
			subItem->visible = (item.subMenuCurHeight >= m_lineSpacing * (k + 1));
			subItem->update();
			k++;
		}
	}
	updateImpl();
}

void MenuState::draw(float dt)
{
	m_window.clear(m_clearColor);
	drawBackground();
	m_window.draw(m_titleText);
	for (const auto& item : m_menuItems)
	{
		m_window.draw(item.text);
		if (item.hasSubItems)
		{
			for (const auto& subItem : item.subItems)
			{
				if (subItem->visible)
					m_window.draw(*subItem);
			}
		}
	}
	m_window.display();
}

void MenuState::selectItem()
{
	if (m_menuItems[m_itemHighlighted].hasSubItems)	//currently highlighted item has sub-items:
	{
		if (!m_menuItems[m_itemHighlighted].subMenuOpened)	//sub-menu not opened:
		{
			if (m_subMenuLastOpened != -1)
				m_menuItems[m_subMenuLastOpened].subMenuOpened = false;	//close last sub-menu opened, if exists
			m_menuItems[m_itemHighlighted].subMenuOpened = true;		//open currently highlighted sub-menu
			m_subItemHighlighted = 0;									//highlight first item
			m_subMenuLastOpened = m_itemHighlighted;					//update last sub-menu opened variable
		}
		else if (m_menuItems[m_itemHighlighted].selectable)	//sub-menu opened and is selectable:
		{
			m_itemSelected = m_itemHighlighted;							//select item
			m_subItemSelected = m_subItemHighlighted;					//select sub-item
		}
		else												//sub-menu opened and non-selectable:
		{
			m_menuItems[m_itemHighlighted].subMenuOpened = false;		//close currently highlighted sub-menu
		}
	}
	else											//currently highlighted item has no sub-items:
	{
		m_itemSelected = m_itemHighlighted;					//select item
	}
	m_clickSound->play();
}

void MenuState::setTitle(const char * name, float yPos, sf::Font* font, int style, const sf::Color& color, const sf::Color& outlineColor, 
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
	m_titleText.setPosition((float)m_window.getSize().x / 2.f -
		m_titleText.getGlobalBounds().width / 2.f, yPos);
}

void MenuState::addMenuItem(const char * name)
{
	float itemPosition;
	if (m_menuItems.empty())
		itemPosition = (float)m_topItemPos;
	else
		itemPosition = m_menuItems.back().text.getGlobalBounds().top + m_lineSpacing;
	m_menuItems.emplace_back(sf::Text(name, *m_font, m_itemTextSize));
	m_menuItems.back().text.setStyle(m_itemStyle);
	centerItem(m_menuItems.back().text, itemPosition);
}

void MenuState::addSubItem(const sf::String& string, bool selectable, bool visible)
{
	MenuItem& item = m_menuItems.back();
	item.hasSubItems = true;
	item.selectable = selectable;
	float itemPosition;
	if (item.subItems.empty())
	{
		itemPosition = item.text.getGlobalBounds().top + m_lineSpacing;
		item.subMenuMaxHeight = m_lineSpacing;
	}
	else
		itemPosition = item.subItems.back()->getGlobalBounds().top + m_lineSpacing;

	sf::Text tempText = sf::Text(string, *m_font, m_subItemTextSize);
	tempText.setStyle(m_itemStyle);
	m_menuItems.back().subItems.push_back(std::make_shared<Text>(tempText,
		m_defaultColor, m_highlightColor));

	SubItem& subItem = *m_menuItems.back().subItems.back();
	centerItem(subItem, itemPosition);
	subItem.visible = visible;
	item.subMenuMaxHeight += m_lineSpacing;
}

void MenuState::addSlider(float & ref)
{
	MenuItem& item = m_menuItems.back();
	item.hasSubItems = true;
	item.selectable = true;
	float itemPosition;
	if (item.subItems.empty())
	{
		itemPosition = item.text.getGlobalBounds().top + m_lineSpacing;
		item.subMenuMaxHeight = m_lineSpacing;
	}
	else
		itemPosition = item.subItems.back()->getGlobalBounds().top + m_lineSpacing;

	m_menuItems.back().subItems.push_back(std::make_shared<RangeSlider>(m_data, ref, sf::Color::Transparent, m_highlightColor, m_defaultColor));
	SubItem& slider = *m_menuItems.back().subItems.back();
	centerItem(slider, itemPosition);
	item.subMenuMaxHeight += m_lineSpacing;
}

void MenuState::adjustSlider(float val)
{
	MenuItem& item = m_menuItems[m_itemHighlighted];
	if (item.hasSubItems)
		if (item.subItems[m_subItemHighlighted]->type == SubItem::Type::RangeSlider)
		{
			item.subItems[m_subItemHighlighted]->moveValue(val);
			m_itemSelected = m_itemHighlighted;
			m_subItemSelected = m_subItemHighlighted;
		}
	m_clickSound->play();
}

void MenuState::setItemAttribs(int style, int textSize, int subItemTextSize)
{
	m_itemStyle = style;
	m_itemTextSize = textSize;
	m_subItemTextSize = subItemTextSize;
}
