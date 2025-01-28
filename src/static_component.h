#ifndef STATIC_OBJECT
#define STATIC_OBJECT

#include <string>
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>
#include <optional>
#include <algorithm>

#include "physics_engine.h"
#include "component.h"

namespace igf
{

class Static_Component: public Component
{
private:
    bool mirror;
    bool hittable;
    std::optional<std::reference_wrapper<Physics_Engine>> physics_engine;
    sol::state L;
    sol::function update_pointer;
    sf::Sprite sp;
    sf::Texture tx;
public:
    Static_Component(std::string file, float x, float y, bool hittable = true);
    Static_Component(std::string sprite_file, std::string script_file, float x, float y, bool hittable = true);
   
    void attach_physics_engine(Physics_Engine &pe) { physics_engine = pe; }

    // component api overrides
    void update(const std::vector<std::string>& msgs) override;
    void draw(Render_Target& target)        override;
    void attach_script(std::string file)    override;
    bool intersects(const sf::FloatRect& r) const override;
    sf::Vector2f get_position()             const override { return sp.getPosition(); }
    sf::FloatRect get_bounding_box()        const override { return sp.getGlobalBounds(); }
    
    // lua api 
    void move(float x, float y)             override;
    void force_move(float x, float y)       override;

    void set_mirror(bool m) { mirror = m; }
    void toggle_mirror() { mirror = !mirror; }
    void jump();
};

}

#endif
