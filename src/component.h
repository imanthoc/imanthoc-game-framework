#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

#include "render_target.h"

namespace igf 
{

struct Component
{
protected:
    int index;
public:
    Component() = default;
    Component(const Component& c) = delete;
    Component(Component&& c) = delete;

    // pass by ref is performance critical here
    virtual void update(const std::vector<std::string>& msgs)   = 0;
    virtual void draw(Render_Target& target)                    = 0;
    virtual void attach_script(std::string file)                = 0;
    virtual bool intersects(const sf::FloatRect& r) const       = 0;
    virtual sf::FloatRect get_bounding_box()        const       = 0;
    virtual sf::Vector2f  get_position()            const       = 0;
    virtual void move(float x, float y)                         = 0;
    virtual void force_move(float x, float y)                   = 0;
    void set_index(int i) { index = i; }
    int get_index() const { return index; }
};

}

#endif
