#ifndef TILEMAP_COMPONENT_H
#define TILEMAP_COMPONENT_H

#include <vector>
#include<SFML/Graphics.hpp>
#include <string>
#include <optional>
#include <algorithm>

#include "physics_engine.h"
#include "component.h"

namespace igf
{

class Tilemap_Component: public Component
{
private:
    int tile_size;
    int width, height;
    sf::Texture tx;
    std::vector<std::tuple<sf::Sprite, bool>> tile_vector;
    std::optional<std::reference_wrapper<Physics_Engine>> physics_engine;
private:
    // utility function to map indices to rectangles on the texture file
    sf::IntRect get_texture_rectangle(int index) const;
public:
    Tilemap_Component(std::string tilemap_file, std::string descriptor_file, int tile_size);
    
    void attach_physics_engine(Physics_Engine& pe) { physics_engine = pe; } 
    void update(const std::vector<std::string>& msgs) override {}
    void draw(Render_Target& target)        override;
    void attach_script(std::string file)    override {}    
    bool intersects(const sf::FloatRect& r) const override;
    sf::FloatRect get_bounding_box()        const override { return {}; }
    sf::Vector2f  get_position()            const override { return {}; }
    void move(float x, float y)             override {}
    void force_move(float x, float y)       override {} 
};

}

#endif
