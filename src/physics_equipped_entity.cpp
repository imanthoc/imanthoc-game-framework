#include <SFML/Window.hpp>
#include <vector>

#include "physics_equipped_entity.h"
#include "keyboard_io.h"

using namespace igf;

void Physics_Equipped_Entity::draw(Render_Target& target)
{
    for (auto& c: components)
    {
        c->draw(target);
    }
}

std::vector<std::string> Physics_Equipped_Entity::get_event() const
{
    std::vector<std::string> events;
    events.reserve(3);

    if (Keyboard_IO::is_key_pressed(sf::Keyboard::W)) events.push_back("W");
    if (Keyboard_IO::is_key_pressed(sf::Keyboard::S)) events.push_back("S");
    if (Keyboard_IO::is_key_pressed(sf::Keyboard::A)) events.push_back("A");
    if (Keyboard_IO::is_key_pressed(sf::Keyboard::D)) events.push_back("D");
    
    return events;
}

void Physics_Equipped_Entity::update()
{
    // maybe need to make a list of events in the future
    auto event = get_event();

    for (auto& c: components)
    {
        c->update(event);
    }
    
    if (physics_engine.enabled()) physics_engine.update(event);
}
