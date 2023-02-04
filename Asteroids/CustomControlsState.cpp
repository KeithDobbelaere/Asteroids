#include "CustomControlsState.h"
#include "DialogState.h"

#include <iostream>


CustomControlsState::CustomControlsState(AppDataPtr data, GameDataPtr gameData) :
	MenuState(data, gameData), m_gameData(gameData), m_controls(data->controls), m_lastItem(0), m_lastSubItem(0), m_saveRequired(false)
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: CustomControlsState constructed!\n";
#	endif
	m_textColor = sf::Color::Yellow;

	m_backText = sf::Text("Back", *m_font, 56);
	m_backText.setFillColor(m_textColor);
	m_backText.setPosition(50, SCRN_HEIGHT - m_backText.getGlobalBounds().height - 50);

	m_saveText = sf::Text("Save", *m_font, 56);
	m_saveText.setFillColor(m_textColor);
	const auto& bounds = m_backText.getGlobalBounds();
	m_saveText.setPosition(SCRN_WIDTH - bounds.width - 50,
		SCRN_HEIGHT - bounds.height - 50);
	m_controlSelected = Control::None;
}

CustomControlsState::~CustomControlsState()
{
#	if _DEBUG
		std::cout << "STATE_MACHINE: CustomControlsState destroyed!\n";
#	endif
}

void CustomControlsState::init()
{
	auto& music = m_appData->music;
	music.setMaxVolume(m_appData->musicVolumeFactor * 100);
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);

	m_starField.init(m_appData);

	setDefaultColor(m_textColor);
	setHighlightColor(sf::Color::White);
	setTitle("(Custom\u00a0Controls)", 60.0f, m_appData->assets.getFont("arcadeBar"), sf::Text::Style::Regular, sf::Color::Yellow, sf::Color::Transparent, 84, .9f);
	setTopItemPos(200);
	setDefaultLineSpacing(3);
	setDefaultAttribs(sf::Text::Bold | sf::Text::Italic, 58, 48);
	addMenuItem("Thrust");
	addSubItem(m_controls.toString(Control::Thrust1));
	addSubItem(m_controls.toString(Control::Thrust2));
	addMenuItem("Rotate Left");
	addSubItem(m_controls.toString(Control::Left1));
	addSubItem(m_controls.toString(Control::Left2));
	addMenuItem("Rotate Right");
	addSubItem(m_controls.toString(Control::Right1));
	addSubItem(m_controls.toString(Control::Right2));
	addMenuItem("Reverse Direction");
	addSubItem(m_controls.toString(Control::Reverse1));
	addSubItem(m_controls.toString(Control::Reverse2));
	addMenuItem("Fire Weapon");
	addSubItem(m_controls.toString(Control::Fire_Weapon1));
	addSubItem(m_controls.toString(Control::Fire_Weapon2));
	addMenuItem("Fire Special Weapon");
	addSubItem(m_controls.toString(Control::Fire_Special1));
	addSubItem(m_controls.toString(Control::Fire_Special2));
	addMenuItem("Restore Defaults");
}

void CustomControlsState::cleanup()
{
}

void CustomControlsState::pause()
{
}

void CustomControlsState::resume()
{
}

void CustomControlsState::processInputImpl()
{
	if (m_saveRequired) {
		switch (m_appData->dialog)
		{
		case Dialog::Accept:
			m_controls.save();
			clearAndExit();
			return;
		case Dialog::Reject:
			m_controls.load();
			clearAndExit();
			return;
		case Dialog::Cancel:
			m_saveRequired = false;
			break;
		}
	}
}

void CustomControlsState::updateImpl()
{	
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
	auto& input = m_appData->input;
	if (m_controlSelected != Control::None) { //one of the controls is selected
		m_itemHighlighted = m_lastItem;
		m_subItemHighlighted = m_lastSubItem;
		if (input.anyKeyPressed())
		{
			if (!input.wasKeyPressed(sf::Keyboard::Key::Escape)) //escape bails out of binding change
				m_controls.change(m_controlSelected, input.lastKeyPressed());
			m_controlSelected = Control::None;
			refresh();
			unselectSubItems();
			if (input.wasKeyPressed(sf::Keyboard::Key::Enter)) //prevents reselecting this control binding by parent class (hacky)
			{
				m_itemSelected = -1;
				m_subItemSelected = -1;
			}
		}
	}
	if (input.isItemHovered(m_backText, m_window))
		m_backText.setFillColor(sf::Color::White);
	else
		m_backText.setFillColor(m_textColor);
	if (input.isItemHovered(m_saveText, m_window))
		m_saveText.setFillColor(sf::Color::White);
	else
		m_saveText.setFillColor(m_textColor);
	if (input.wasItemClicked(m_backText, sf::Mouse::Button::Left, m_window)) {
		m_clickSound->play();
		if (m_controls.wasChanged())
			promptForSave();
		else
			m_appData->machine.removeState();
	}
	if (input.wasKeyPressed(sf::Keyboard::Key::F1) ||
		input.wasItemClicked(m_saveText, sf::Mouse::Button::Left, m_window)) {
		m_clickSound->play();
		m_controls.save();
		m_appData->machine.removeState();
	}
	if (!m_menuItems[m_lastItem].isOpen())
	{
		m_controlSelected = Control::None;
		unselectSubItems();
	}
	if (m_itemSelected != -1)
	{
		m_lastItem = m_itemSelected;
		m_lastSubItem = m_subItemSelected;
		unselectSubItems();
		if (m_itemSelected != 6)
		{
			setBlink(m_itemSelected, m_subItemSelected);
		}
		switch (m_itemSelected)
		{
		case 0:
			if (m_subItemSelected == 0)
				m_controlSelected = Control::Thrust1;
			else
				m_controlSelected = Control::Thrust2;
			break;
		case 1:
			if (m_subItemSelected == 0)
				m_controlSelected = Control::Left1;
			else
				m_controlSelected = Control::Left2;
			break;
		case 2:
			if (m_subItemSelected == 0)
				m_controlSelected = Control::Right1;
			else
				m_controlSelected = Control::Right2;
			break;
		case 3:
			if (m_subItemSelected == 0)
				m_controlSelected = Control::Reverse1;
			else
				m_controlSelected = Control::Reverse2;
			break;
		case 4:
			if (m_subItemSelected == 0)
				m_controlSelected = Control::Fire_Weapon1;
			else
				m_controlSelected = Control::Fire_Weapon2;
			break;
		case 5:
			if (m_subItemSelected == 0)
				m_controlSelected = Control::Fire_Special1;
			else
				m_controlSelected = Control::Fire_Special2;
			break;
		case 6:
			m_controls.defaults();
			refresh();
			break;
		default:
			std::cerr << "Error selecting menu item!" << '\n';
		}
		m_itemSelected = -1;
		m_subItemSelected = -1;
	}
}

void CustomControlsState::promptForSave()
{
	m_saveRequired = true;
	m_appData->machine.addState(StatePtr(std::make_unique<DialogState>(m_appData, m_gameData)));
}

void CustomControlsState::clearAndExit()
{
	m_backText.setString({});
	m_saveText.setString({});
	m_menuItems.clear();
	addMenuItem({});
	m_lastItem = 0;
	m_itemHighlighted = 0;
	m_subItemHighlighted = 0;
	m_titleText.setString({});
	m_appData->machine.removeState();
}

void CustomControlsState::refresh()
{
	int i = 0;
	for (auto& item : m_menuItems)
		for (auto& subItem : item.getSubItems()) {
			subItem->setString(m_controls.toString((Control)i++));
			MenuItem::centerItem(*subItem);
		}

}

void CustomControlsState::unselectSubItems()
{
	for (auto& item : m_menuItems)
		if (item.hasSubItems())
			for (auto& subItem : item.getSubItems())
				subItem->select(false);
}

void CustomControlsState::setBlink(int item, int subItem)
{
	m_menuItems[item].getSubItem(subItem)->select(true);
}

void CustomControlsState::onEscapePressed()
{
	if (m_controls.wasChanged())
		promptForSave();
	else
		m_appData->machine.removeState();
}

void CustomControlsState::drawBackground()
{
	auto& window = m_appData->window;
	window.draw(m_starField);
	window.draw(m_backText);
	window.draw(m_saveText);
}


