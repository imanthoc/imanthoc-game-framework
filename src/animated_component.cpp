#include <vector>
#include <string>

#include "animated_component.h"

using namespace igf;

Animated_Component::Animated_Component(std::string spritesheet_file, int fc, int as, float x, float y, bool h)
{

    if (!tx.loadFromFile(spritesheet_file))
    {
        throw std::runtime_error("Error, couldn't open texture file");
    }

    sp.setTexture(tx);
    sp.setPosition(x, y);

    mirror          = false;
    hittable        = h;
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

Animated_Component::Animated_Component(std::string spritesheet_file, std::string script_file, int fc, int as, float x, float y, bool h)
{

    if (!tx.loadFromFile(spritesheet_file))
    {
        throw std::runtime_error("Error, couldn't open texture file");
    }

    sp.setTexture(tx);
    sp.setPosition(x, y);

    mirror          = false;
    hittable        = h;
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
    
    attach_script(script_file);
}

void Animated_Component::update(const std::vector<std::string>& msgs)
{
    if (update_pointer) update_pointer(msgs);

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
            "get_position", &Animated_Component::get_position);
    
    L["comp"] = this;

    try
    {
        L.script_file(file);
    }
    catch (const sol::error& e)
    {
        std::cout << "LUA error, " << e.what() << '\n';
    }

    update_pointer = L["update"];

    if (!update_pointer)
    {
        std::runtime_error("LUA error, no update function provided");
    }
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
