#include <SFML/Graphics.hpp>
#include <iostream>

#include "render_target.h"

using namespace igf;

Render_Target::Render_Target(int width, int height, std::string name, float zoom):
    window(sf::VideoMode(width, height), name)
{
    window.setFramerateLimit(60);
        
    sf::View v{{ 0, 0, static_cast<float>(width) / zoom, static_cast<float>(height) / zoom }};
    
    window.setView(v);
}

bool Render_Target::poll_exit()
{
    sf::Event ev;
    while (window.pollEvent(ev))
    {
        if (ev.type == sf::Event::Closed)
        {
            window.close();
            return true;
        }
    }

    return false;
}
