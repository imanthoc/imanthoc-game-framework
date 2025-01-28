#include <iostream>
#include <vector>
#include <string>

#include "animated_component.h"

using namespace igf;

Animated_Component::Animated_Component(float x, float y, bool h)
{
    sp.setPosition(x, y);
    mirror = false;
    hittable = h;
}

Animated_Component::Animated_Component(std::string script_file, float x, float y, bool h)
{
    sp.setPosition(x, y);
    mirror = false;
    hittable = h;

    attach_script(script_file);
}

void Animated_Component::add_animation(std::string tag, std::string spritesheet_file, int fc, int as)
{
    animations_table[tag] = { spritesheet_file, fc, as };
}

void Animated_Component::set_active_animation(std::string tag)
{
    if (tag != current_animation_tag)
    {
        current_animation_tag = tag;
        if (!animations_table.count(tag))
        {
            throw std::invalid_argument("Error, animation tag not found");
        }
        reset_spritesheet(animations_table[tag]);
    }
}

void Animated_Component::reset_spritesheet(const Animation_Info& info)
{
    const auto& [spritesheet_file, fc, as] = info;
    if (!tx.loadFromFile(spritesheet_file))
    {
        throw std::runtime_error("Error, couldn't open texture file");
    }

    sp.setTexture(tx);

    frame_count     = fc;
    current_frame   = 0;
    anim_speed      = as;
    tick            = 0;
    spritesheet_w   = tx.getSize().x;
    spritesheet_h   = tx.getSize().y;
    
    if (spritesheet_w % frame_count != 0)
    {
        throw std::runtime_error("Error, texture file width must be divisible by frame count");
    }

    frame_h = spritesheet_h;
    frame_w = spritesheet_w / frame_count; 
    
    rectangle = { { 0, 0 }, { frame_w, frame_h } };

    sp.setTextureRect(rectangle);
}

void Animated_Component::update(const std::vector<std::string>& msgs)
{
    if (update_pointer) update_pointer(msgs);

    if (current_animation_tag.empty()) return;

    if (tick == anim_speed)
    {
        tick = 0;
        if (rectangle.left + frame_w == spritesheet_w)
        {
            rectangle.left = 0;
        }
        else
        {
            rectangle.left += frame_w;
        }
        sp.setTextureRect(rectangle);
    }
    else 
    {
        tick++;
    }
}

void Animated_Component::attach_script(std::string file)
{
    L.new_usertype<sf::Vector2f>("Vector2f",
            "x", &sf::Vector2f::x,
            "y", &sf::Vector2f::y);
    
    L.new_usertype<Animated_Component>("Animated_Component",
            "move", &Animated_Component::move,
            "force_move", &Animated_Component::force_move,
            "jump", &Animated_Component::jump,
            "set_mirror", &Animated_Component::set_mirror,
            "toggle_mirror", &Animated_Component::toggle_mirror,
            "add_animation", &Animated_Component::add_animation,
            "set_active_animation", &Animated_Component::set_active_animation,
            "get_position", &Animated_Component::get_position);
    
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
        throw std::runtime_error("LUA error, no update function provided");
    }

    // changing this to auto crashes the program for some reason
    sol::function ready_pointer = L["ready"];

    if (!ready_pointer)
    {
        throw std::runtime_error("LUA error, no ready function provided");
    }
    ready_pointer();
}

void Animated_Component::draw(Render_Target& target)
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


void Animated_Component::jump()
{
    physics_engine->get().jump(index);
}

void Animated_Component::move(float x, float y)
{
    const auto& p = sp.getGlobalBounds();
    const auto& pe = physics_engine->get();

    if (pe.can_move_to(x, y, p))
    {
        sp.move(x, y);
    }
}

bool Animated_Component::intersects(const sf::FloatRect& r) const
{
    return (hittable && sp.getGlobalBounds().intersects(r));
}
