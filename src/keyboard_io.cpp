#include <iostream>

#include "keyboard_io.h"

// this is some sketchy code, but without an out-of-class definition, the map is not really created
// and i get a undefined reference to map error 
std::map<sf::Keyboard::Key, bool> Keyboard_IO::was_last_pressed = std::map<sf::Keyboard::Key, bool>();

bool Keyboard_IO::is_key_pressed(sf::Keyboard::Key key)
{
    bool is_pressed = sf::Keyboard::isKeyPressed(key);
    was_last_pressed[key] = is_pressed;
    return is_pressed;
}

bool Keyboard_IO::is_key_pressed_once(sf::Keyboard::Key key)
{
    bool is_pressed = sf::Keyboard::isKeyPressed(key);
    bool result;

    if (is_pressed && !was_last_pressed[key]) result = true;
    else                                      result = false;

    was_last_pressed[key] = is_pressed;

    return result;
}

void Keyboard_IO::reset_key_press(sf::Keyboard::Key key)
{
    was_last_pressed[key] = false;
}
