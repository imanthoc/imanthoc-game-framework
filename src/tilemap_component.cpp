#include <iostream>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <cassert>

#include "tilemap_component.h"

using namespace igf;

// A lot of error checking happens here
// since we are dealing with a lot of external input
Tilemap_Component::Tilemap_Component(std::string tf, std::string df, int ts)
{
    tile_size = ts;
    
    if (ts <= 0)
    {
        throw std::invalid_argument("Error, can't have non positive tile size");
    }

    if (!tx.loadFromFile(tf))
    {
        throw std::runtime_error("Error, couldn't open texture file");
    }
    
    if ((tx.getSize().x % tile_size != 0) || (tx.getSize().y % tile_size != 0))
    {
        throw std::runtime_error("Error, can't have texture size non divisible by tile size");
    }

    assert((tx.getSize().x % tile_size) == 0);
    assert((tx.getSize().y % tile_size) == 0);

    std::ifstream df_stream(df, std::ifstream::in);
    std::vector<uint8_t> buffer;
    
    if (df_stream.good())
    {
        std::vector<uint8_t> temp(std::istreambuf_iterator<char>(df_stream), {});
        buffer = std::move(temp);

        df_stream.close();
    }
    else
    {
        throw std::runtime_error("Error, couldn't open binary file");
    }

    width = (buffer[1] << 8) | buffer[0]; 
    height = (buffer[3] << 8) | buffer[2];
    
    if (width <= 0 || height <= 0)
    {
        throw std::runtime_error("Error, can't have non positive width / height");
    }

    int size = width * height;
    
    tile_vector.reserve(size);
    auto binary_data = buffer.begin() + 4; // skip the 4 bytes for width and height
    
    int x = 0;
    int y = 0; 
    for (int i = 0; i < size; ++i)
    {
        int index = *binary_data;
        bool hittable = *(binary_data + 1);
        
        sf::Sprite sp{ tx, get_texture_rectangle(index) };
        sp.setPosition(x * tile_size, y * tile_size);

        tile_vector.push_back({ sp, hittable });

        if (x == width - 1)
        {
            x = 0;
            y++;
        }
        else
        {
            x++;
        }

        binary_data += 2;
    }
}

sf::IntRect Tilemap_Component::get_texture_rectangle(int index) const
{
    int t_height_tiles = tx.getSize().y / tile_size;
    int t_width_tiles  = tx.getSize().x / tile_size;

    int height_index = index / t_width_tiles;
    int width_index  = index % t_width_tiles;

    assert(height_index < t_height_tiles);
    assert(width_index < t_width_tiles);

    return { 
        width_index * tile_size,
        height_index * tile_size,
        tile_size,
        tile_size
    };    
}

void Tilemap_Component::draw(Render_Target& target)
{
    for (auto& [s, h]: tile_vector)
    {
        target.draw(s);
    }
}

bool Tilemap_Component::intersects(const sf::FloatRect& r) const
{
    for (auto& [s, h]: tile_vector)
    {
        if (h && s.getGlobalBounds().intersects(r)) return true;
    }

    return false;
}


