#include <iostream>

#include "tilemap_component.h"
#include "physics_equipped_entity.h"
#include "physics_engine.h"
#include "render_target.h"
#include "static_component.h"
#include "entity.h"
#include "animated_component.h"

int main()
{
    igf::Render_Target window{ 800, 600, "KurWa!", 2.5f };
    
    igf::Physics_Equipped_Entity game_scene;
    
    game_scene.emplace_component<igf::Tilemap_Component>("assets/assets.png", "assets/desc.bin", 16); 
    game_scene.emplace_component<igf::Static_Component>("assets/box.png", 0.0f, 0.0f);
    
    game_scene.emplace_component<igf::Static_Component>("assets/box.png", 30.0f, 30.0f); 
    
    game_scene.emplace_component<igf::Animated_Component>("assets/character_right.png", "assets/m.lua", 4, 5, 50.0f, 30.0f);

    while (window.is_open())
    {
        window.poll_exit(); 
        
        game_scene.update();

        window.clear();
        game_scene.draw(window);
        window.display();
    }
    
    return 0;
}
