#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "component.h"

namespace igf
{

class Physics_Engine
{
private:
    bool en;
    std::vector<std::unique_ptr<Component>>& components;
    std::vector<float> forces;
    double g;
public:
    Physics_Engine(std::vector<std::unique_ptr<Component>>& components_reference);

    Physics_Engine(const Physics_Engine& pe) = delete;
    Physics_Engine(Physics_Engine&& pe) = delete;
    
    float epsilon = 0.00001;    

    bool can_move_to(float x, float y, const sf::FloatRect& r) const;
    void set_g(double g_) { g = g_; }

    void enable() { en = true; }
    void disable() { en = false; }
    bool enabled() const { return en; }
    
    float get_force(int index) const { return forces[index]; }
    void update(const std::vector<std::string>& msgs);
    void jump(int index);
};

}

#endif
