#ifndef PHYSICS_EQUIPPED_ENTITY_H
#define PHYSICS_EQUIPPED_ENTITY_H

#include <vector>
#include <string>
#include <memory>

#include "entity.h"
#include "component.h"
#include "physics_engine.h"

namespace igf
{

// physics engine type
class Physics_Equipped_Entity: public Entity
{
private:
    std::vector<std::unique_ptr<Component>> components;
    int count;
private:
    // reads possible events
    // for example user keyboard input, and passes it as a message to the 
    // lua update functions, like an observer pattern
    std::vector<std::string> get_event() const;

public:
    // maybe make template type if needed
    Physics_Engine physics_engine;
public:
    Physics_Equipped_Entity():
        physics_engine(components) { count = 0; }

    void draw(Render_Target& target) override;
    void update() override;
    
    template <typename CT, typename... T>
    void emplace_component(T... args)
    {
        //TODO: forward the args instead of copying
        auto c = std::make_unique<CT>(args...); 
        c->attach_physics_engine(physics_engine);
        c->set_index(count++);

        components.push_back(std::move(c));
    }
};

}

#endif
