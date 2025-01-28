#include <iostream>
#include <stdexcept>
#include <sol/sol.hpp>

#include "static_component.h"

using namespace igf;

Static_Component::Static_Component(std::string file, float x, float y, bool h)
{
    mirror = false;
    hittable = h;

    if (!tx.loadFromFile(file))
    {
        throw std::runtime_error("Error, couldn't open texture file");
    }
    sp.setTexture(tx);
    sp.setPosition({ x, y });
}

Static_Component::Static_Component(std::string sprite_file, std::string script_file, float x, float y, bool h)
{
    hittable = h;

    if (!tx.loadFromFile(sprite_file))
    {
        throw std::runtime_error("Error, couldn't open texture file");
    }

    sp.setTexture(tx);
    sp.setPosition({ x, y });
    
    attach_script(script_file);
}

void Static_Component::update(const std::vector<std::string>& msgs)
{
    if (update_pointer) update_pointer(msgs);
}

void Static_Component::draw(Render_Target& target)
{
    if (mirror)
    {
        sp.setOrigin(sp.getTextureRect().width, 0);
        sp.setScale(-1.0f, 1.0f);
    }
    else 
    {
        sp.setOrigin(0, 0);
        sp.setScale(1.0f, 1.0f);
    }

    target.draw(sp);
}

void Static_Component::attach_script(std::string file)
{
    L.new_usertype<sf::Vector2f>("Vector2f",
            "x", &sf::Vector2f::x,
            "y", &sf::Vector2f::y);
    
    L.new_usertype<Static_Component>("Static_Component",
            "move", &Static_Component::move,
            "force_move", &Static_Component::force_move,
            "jump", &Static_Component::jump,
            "set_mirror", &Static_Component::set_mirror,
            "toggle_mirror", &Static_Component::toggle_mirror,
            "get_position", &Static_Component::get_position);

    L["comp"] = this;
    
    try
    {
        L.script_file(file);    
    }
    catch (const sol::error& e)
    {
        std::cout << "LUA error, " << e.what() << '\n';
        throw std::exception();
    }
    update_pointer = L["update"];

    if (!update_pointer)
    {
        std::runtime_error("LUA error, no update function provided");
    }
}

void Static_Component::jump()
{
    physics_engine->get().jump(index);
}

void Static_Component::move(float x, float y)
{
    const auto& p = sp.getGlobalBounds();
    const auto& pe = physics_engine->get();

    if (pe.can_move_to(x, y, p))
    {
        sp.move(x, y);
    }
}

void Static_Component::force_move(float x, float y)
{
    sp.move(x, y);
}

bool Static_Component::intersects(const sf::FloatRect& r) const
{
    return (hittable && sp.getGlobalBounds().intersects(r));
}
