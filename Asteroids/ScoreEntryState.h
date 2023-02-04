#pragma once

#include "Application.h"
#include "GameState.h"
#include "StarField.h"
#include "TextEffects.h"


class ScoreEntryState : public GameState
{
public:
	ScoreEntryState(AppDataPtr data, GameDataPtr gameData);
	~ScoreEntryState();

	void init() override;
	void cleanup() override;

	void processInput() override;
	void update(float dt) override;
	void draw(float dt) override;

private:
	char m_initials[4];
	int m_cursorPosition, m_lastCursorPos;
	const int m_characterSize = 120;
	sf::Vector2f m_cursorScreenPos;
	AppDataPtr m_appData;
	GameDataPtr m_gameData;
	sf::Font* m_font, *m_titleFont;
	sf::Text m_titleText, m_instructionText, m_initialsText;
	BlinkingText m_cursorText;
	StarField m_starField;
	sf::RenderWindow& m_window;
};
