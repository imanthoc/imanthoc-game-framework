#ifndef KEYBOARD_IO
#define KEYBOARD_IO

#include <SFML/Window.hpp>
#include <map>

class Keyboard_IO
{
private:
    static std::map<sf::Keyboard::Key, bool> was_last_pressed;
public:
    static bool is_key_pressed     (sf::Keyboard::Key key);
    static bool is_key_pressed_once(sf::Keyboard::Key key);
    static void reset_key_press    (sf::Keyboard::Key key);
};

#endif