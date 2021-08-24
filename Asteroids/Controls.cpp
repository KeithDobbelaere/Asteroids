#include "Controls.h"

#include "Defines.h"

#include <iostream>
#include <fstream>


const std::map<sf::Keyboard::Key, std::string> Controls::m_keyStrings = {
       { sf::Keyboard::Key::Unknown,   "[Undefined]" },
       { sf::Keyboard::Key::A,                 "[A]" },
       { sf::Keyboard::Key::B,                 "[B]" },
       { sf::Keyboard::Key::C,                 "[C]" },
       { sf::Keyboard::Key::D,                 "[D]" },
       { sf::Keyboard::Key::E,                 "[E]" },
       { sf::Keyboard::Key::F,                 "[F]" },
       { sf::Keyboard::Key::G,                 "[G]" },
       { sf::Keyboard::Key::H,                 "[H]" },
       { sf::Keyboard::Key::I,                 "[I]" },
       { sf::Keyboard::Key::J,                 "[J]" },
       { sf::Keyboard::Key::K,                 "[K]" },
       { sf::Keyboard::Key::L,                 "[L]" },
       { sf::Keyboard::Key::M,                 "[M]" },
       { sf::Keyboard::Key::N,                 "[N]" },
       { sf::Keyboard::Key::O,                 "[O]" },
       { sf::Keyboard::Key::P,                 "[P]" },
       { sf::Keyboard::Key::Q,                 "[Q]" },
       { sf::Keyboard::Key::R,                 "[R]" },
       { sf::Keyboard::Key::S,                 "[S]" },
       { sf::Keyboard::Key::T,                 "[T]" },
       { sf::Keyboard::Key::U,                 "[U]" },
       { sf::Keyboard::Key::V,                 "[V]" },
       { sf::Keyboard::Key::W,                 "[W]" },
       { sf::Keyboard::Key::X,                 "[X]" },
       { sf::Keyboard::Key::Y,                 "[Y]" },
       { sf::Keyboard::Key::Z,                 "[Z]" },
       { sf::Keyboard::Key::Num0,           "[Num0]" },
       { sf::Keyboard::Key::Num1,           "[Num1]" },
       { sf::Keyboard::Key::Num2,           "[Num2]" },
       { sf::Keyboard::Key::Num3,           "[Num3]" },
       { sf::Keyboard::Key::Num4,           "[Num4]" },
       { sf::Keyboard::Key::Num5,           "[Num5]" },
       { sf::Keyboard::Key::Num6,           "[Num6]" },
       { sf::Keyboard::Key::Num7,           "[Num7]" },
       { sf::Keyboard::Key::Num8,           "[Num8]" },
       { sf::Keyboard::Key::Num9,           "[Num9]" },
       { sf::Keyboard::Key::Escape,       "[Escape]" },
       { sf::Keyboard::Key::LControl,   "[LControl]" },
       { sf::Keyboard::Key::LShift,       "[LShift]" },
       { sf::Keyboard::Key::LAlt,           "[LAlt]" },
       { sf::Keyboard::Key::LSystem,     "[LSystem]" },
       { sf::Keyboard::Key::RControl,   "[RControl]" },
       { sf::Keyboard::Key::RShift,       "[RShift]" },
       { sf::Keyboard::Key::RAlt,           "[RAlt]" },
       { sf::Keyboard::Key::RSystem,     "[RSystem]" },
       { sf::Keyboard::Key::Menu,           "[Menu]" },
       { sf::Keyboard::Key::LBracket ,  "[LBracket]" },
       { sf::Keyboard::Key::RBracket,   "[RBracket]" },
       { sf::Keyboard::Key::Semicolon, "[Semicolon]" },
       { sf::Keyboard::Key::Comma,         "[Comma]" },
       { sf::Keyboard::Key::Period,       "[Period]" },
       { sf::Keyboard::Key::Quote,         "[Quote]" },
       { sf::Keyboard::Key::Slash,         "[Slash]" },
       { sf::Keyboard::Key::Backslash, "[Backslash]" },
       { sf::Keyboard::Key::Tilde,         "[Tilde]" },
       { sf::Keyboard::Key::Equal,         "[Equal]" },
       { sf::Keyboard::Key::Hyphen,       "[Hyphen]" },
       { sf::Keyboard::Key::Space,         "[Space]" },
       { sf::Keyboard::Key::Enter,         "[Enter]" },
       { sf::Keyboard::Key::Backspace, "[Backspace]" },
       { sf::Keyboard::Key::Tab,             "[Tab]" },
       { sf::Keyboard::Key::PageUp,       "[PageUp]" },
       { sf::Keyboard::Key::PageDown,   "[PageDown]" },
       { sf::Keyboard::Key::End,             "[End]" },
       { sf::Keyboard::Key::Home,           "[Home]" },
       { sf::Keyboard::Key::Insert,       "[Insert]" },
       { sf::Keyboard::Key::Delete,       "[Delete]" },
       { sf::Keyboard::Key::Add,             "[Add]" },
       { sf::Keyboard::Key::Subtract,   "[Subtract]" },
       { sf::Keyboard::Key::Multiply,   "[Multiply]" },
       { sf::Keyboard::Key::Divide,       "[Divide]" },
       { sf::Keyboard::Key::Left,           "[Left]" },
       { sf::Keyboard::Key::Right,         "[Right]" },
       { sf::Keyboard::Key::Up,               "[Up]" },
       { sf::Keyboard::Key::Down,           "[Down]" },
       { sf::Keyboard::Key::Numpad0,     "[Numpad0]" },
       { sf::Keyboard::Key::Numpad1,     "[Numpad1]" },
       { sf::Keyboard::Key::Numpad2,     "[Numpad2]" },
       { sf::Keyboard::Key::Numpad3,     "[Numpad3]" },
       { sf::Keyboard::Key::Numpad4,     "[Numpad4]" },
       { sf::Keyboard::Key::Numpad5,     "[Numpad5]" },
       { sf::Keyboard::Key::Numpad6,     "[Numpad6]" },
       { sf::Keyboard::Key::Numpad7,     "[Numpad7]" },
       { sf::Keyboard::Key::Numpad8,     "[Numpad8]" },
       { sf::Keyboard::Key::Numpad9,     "[Numpad9]" },
       { sf::Keyboard::Key::F1,               "[F1]" },
       { sf::Keyboard::Key::F2,               "[F2]" },
       { sf::Keyboard::Key::F3,               "[F3]" },
       { sf::Keyboard::Key::F4,               "[F4]" },
       { sf::Keyboard::Key::F5,               "[F5]" },
       { sf::Keyboard::Key::F6,               "[F6]" },
       { sf::Keyboard::Key::F7,               "[F7]" },
       { sf::Keyboard::Key::F8,               "[F8]" },
       { sf::Keyboard::Key::F9,               "[F9]" },
       { sf::Keyboard::Key::F10,             "[F10]" },
       { sf::Keyboard::Key::F11,             "[F11]" },
       { sf::Keyboard::Key::F12,             "[F12]" },
       { sf::Keyboard::Key::F13,             "[F13]" },
       { sf::Keyboard::Key::F14,             "[F14]" },
       { sf::Keyboard::Key::F15,             "[F15]" },
       { sf::Keyboard::Key::Pause,         "[Pause]" }
};

Controls::Controls() {
    if (!load()) {
        defaults();
        save();
    }
}

bool Controls::load()
{
    std::array<sf::Keyboard::Key, 12> temp;
    read(288, temp);
    int u = 0;
    bool valid = true;
    for (int i = 0; i < 12; i++) {
        if (temp[i] == sf::Keyboard::Key::Unknown)
            u++;
        if (temp[i] < sf::Keyboard::Key::Unknown || temp[i] > sf::Keyboard::Key::Pause)
            valid = false;
        for (int j = i + 1; j < 12; j++) {
            if (temp[i] != sf::Keyboard::Key::Unknown && temp[i] == temp[j])
                valid = false;
        }
    }
    if (!valid) {
        std::cerr << "ERROR: App data file has been corrupted.\nRegenerating custom controls file...\n";
        return false;
    }
    m_undefined = u;
    m_needsSave = false;
    m_keyBindings = temp;
    return true;
}

void Controls::save()
{
    if (m_needsSave) {
        write(288, m_keyBindings);
        m_needsSave = false;
    }
}


void Controls::change(Control c, sf::Keyboard::Key k) {
    for (auto& control : m_keyBindings)
        if (control == k) {
            control = sf::Keyboard::Key::Unknown;
            m_undefined++;
        }

    if (m_keyBindings[(int)c] == sf::Keyboard::Key::Unknown)
        m_undefined--;
    m_keyBindings[(int)c] = k;
    m_needsSave = true;
}

void Controls::defaults() {
    m_keyBindings = {
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::W,
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::A,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::D,
        sf::Keyboard::Key::Down,
        sf::Keyboard::Key::S,
        sf::Keyboard::Key::Space,
        sf::Keyboard::Key::Enter,
        sf::Keyboard::Key::LShift,
        sf::Keyboard::Key::RShift
    };
    m_undefined = 0;
    m_needsSave = true;
}

const std::string& Controls::toString(Control c) const {
    return m_keyStrings.at(m_keyBindings[(int)c]);
}

const sf::Keyboard::Key& Controls::key(Control c) const {
    return m_keyBindings[(int)c];
}
