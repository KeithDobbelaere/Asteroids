#pragma once

#include <SFML/Graphics.hpp>

#include "FileManager.h"

#include <array>


enum class Control {
    Thrust1,
    Thrust2,
    Left1,
    Left2,
    Right1,
    Right2,
    Reverse1,
    Reverse2,
    Fire_Weapon1,
    Fire_Weapon2,
    Fire_Special1,
    Fire_Special2,
    None
};

class Controls : public FileManager {
public:
    Controls();

    bool load();
    void save();
    void change(Control c, sf::Keyboard::Key k);
    void defaults();

    const std::string& toString(Control c) const;
    const sf::Keyboard::Key& key(Control c) const;

    bool hasUndefined() const { return (m_undefined); }
    bool wasChanged() const { return m_needsSave; }
private:
    std::array<sf::Keyboard::Key, 12> m_keyBindings;
    static const std::map<sf::Keyboard::Key, std::string> m_keyStrings;
    int m_undefined = 0;
    bool m_needsSave = false;

    const int FILE_SIZE_OF_SCORE_DATA = 288;
};
