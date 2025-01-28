#ifndef ANIMATED_COMPONENT
#define ANIMATED_COMPONENT

#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include <map>

#include "physics_engine.h"
#include "component.h"

namespace igf
{

struct Animation_Info
{
    std::string spritesheet_file;
    int frame_count;
    int anim_speed;
};

class Animated_Component: public Component
{
private:
    std::string current_animation_tag;
    std::map<std::string, Animation_Info> animations_table;
    std::optional<std::reference_wrapper<Physics_Engine>> physics_engine;
    sol::state L;
    sol::function update_pointer;
    sf::Texture tx;
    sf::Sprite sp;
    sf::IntRect rectangle;
    int current_frame;
    int frame_count;
    int anim_speed;
    int frame_w, frame_h;
    int spritesheet_w, spritesheet_h;
    int tick;
    bool mirror;
    bool hittable;
private:
    void reset_spritesheet(const Animation_Info& info); 
public:
    Animated_Component(float x, float y, bool hittable = true);
    Animated_Component(std::string script_file, float x, float y, bool hittable = true);

    void attach_physics_engine(Physics_Engine& pe) { physics_engine = pe; }
    void set_active_animation(std::string tag);
    void add_animation(std::string tag, std::string spritesheet_file, int frame_count, int anim_speed);

    // component api overrides
    void update(const std::vector<std::string>& msgs)   override;
    void draw(Render_Target& target)                    override;
    void attach_script(std::string file)                override;
    bool intersects(const sf::FloatRect& r)             const override;
    sf::FloatRect get_bounding_box()                    const override { return sp.getGlobalBounds(); }
    sf::Vector2f  get_position()                        const override { return sp.getPosition(); }
    void move(float x, float y)                         override;
    void force_move(float x, float y)                   override { sp.move(x, y); }
    
    void set_mirror(bool m) { mirror = m; }
    void toggle_mirror() { mirror = !mirror; }
    void jump(); 
};
 
}

#endif
