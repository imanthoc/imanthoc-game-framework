#include <cmath>
#include <iostream>

#include "physics_engine.h"

using namespace igf;

Physics_Engine::Physics_Engine(std::vector<std::unique_ptr<Component>>& components_reference): components(components_reference)
{
    g = 0.075;
    en = true;
}

bool Physics_Engine::can_move_to(float x, float y, const sf::FloatRect& r) const
{
    sf::FloatRect r_moved = { r.left + x, r.top + y, r.width, r.height };

    for (auto& c: components)
    {
        if ((c->get_bounding_box() != r) && c->intersects(r_moved)) return false;
    }

    return true;
}

void Physics_Engine::jump(int index)
{
    // this smells like a bug waiting to happen
    if (forces[index] == 0.0f) forces[index] = -2;
}

void Physics_Engine::update(const std::vector<std::string>& msgs)
{
    if (components.size() != forces.size())
    {
        forces.resize(components.size(), 0.0f);    
    }

    for (int i = 0; i < components.size(); ++i)
    {
        auto& c = components[i];
        auto& f = forces[i];
        const auto& bb = c->get_bounding_box();
        
        if (can_move_to(0, f, bb))
        {
            components[i]->force_move(0, f);
            forces[i] += g;
        }
        else if (can_move_to(0, 1, bb))
        {
            components[i]->force_move(0, 1);

            forces[i] = 0;
        }
        else
        {
            forces[i] = 0;
        }
    }
}
