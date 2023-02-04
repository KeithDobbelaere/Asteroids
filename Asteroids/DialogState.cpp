#include "DialogState.h"

DialogState::DialogState(AppDataPtr data, GameDataPtr gameData) :
	MenuState(data, gameData), m_gameData(gameData)
{
#	if _DEBUG
	std::cout << "STATE_MACHINE: DialogState constructed!\n";
#	endif

	auto& assets = m_appData->assets;
	m_effectsSound = assets.linkSoundRef("basic_phaser", 25.0f);
}

DialogState::~DialogState()
{
#	if _DEBUG
	std::cout << "STATE_MACHINE: DialogState destroyed!\n";
#	endif
}

void DialogState::init()
{
	auto& music = m_appData->music;
	music.setMaxVolume(m_appData->musicVolumeFactor * 100);
	music.play("Sounds/Stardust_Memories.ogg");
	music.getCurrent().setLoop(true);

	setDefaultColor(sf::Color::Yellow);
	setHighlightColor(sf::Color::White);
	setTitle("(Save\u00a0Changes?)", 200, m_appData->assets.getFont("arcadeBar"), sf::Text::Style::Regular, sf::Color::Yellow, sf::Color::Transparent, 84, .9f);
	setTopItemPos(340);
	setDefaultLineSpacing(7);
	setDefaultAttribs(sf::Text::Bold | sf::Text::Italic, 62, 54);
	addMenuItem("Save");
	addMenuItem("Don't Save");
	addMenuItem("Cancel");

	m_starField.init(m_appData);
}

void DialogState::cleanup()
{
}

void DialogState::pause()
{
}

void DialogState::resume()
{
}

void DialogState::updateImpl()
{
	m_starField.update(sf::Vector2f(-3.0f, 0.0f));
	if (m_itemSelected != -1)
	{
		switch (m_itemSelected)
		{
		case 0:
			m_appData->dialog = Dialog::Accept;
			m_appData->machine.removeState();
			break;
		case 1:
			m_appData->dialog = Dialog::Reject;
			m_appData->machine.removeState();
			break;
		case 2:
			m_appData->dialog = Dialog::Cancel;
			m_appData->machine.removeState();
			break;
		default:
			std::cerr << "Error selecting menu item!" << '\n';
		}
		m_itemSelected = -1;
		m_subItemSelected = -1;
	}
}

void DialogState::onEscapePressed()
{
	m_appData->dialog = Dialog::Cancel;
	m_appData->machine.removeState();
}

void DialogState::drawBackground()
{
	m_appData->window.draw(m_starField);
}
