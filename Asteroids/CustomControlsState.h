#pragma once

#include "MenuState.h"
#include "Application.h"
#include "StarField.h"
#include "Controls.h"


class CustomControlsState : public MenuState
{
public:
	CustomControlsState(AppDataPtr data, GameDataPtr gameData);
	~CustomControlsState();

	void init() override;
	void cleanup() override;

	void pause() override;
	void resume() override;

private:
	void refresh();
	void unselectSubItems();
	void setBlink(int item, int subItem);
	void onEscapePressed() override;
	void drawBackground() override;
	void updateImpl() override;

	Controls& m_controls;
	Control m_controlSelected;
	sf::Color m_textColor;
	sf::Text m_backText, m_saveText;
	StarField m_starField;
	int m_lastItem, m_lastSubItem;
};

