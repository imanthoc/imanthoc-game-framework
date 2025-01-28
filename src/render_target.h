#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

namespace igf
{

class Render_Target
{
private:
    sf::RenderWindow window;
public:
    Render_Target(int width, int height, std::string name, float zoom = 1);

    // a window can neither be moved or copied
    Render_Target(const Render_Target& r) = delete;
    Render_Target(Render_Target&& r) = delete;

    bool is_open() const { return window.isOpen(); }
    void clear() { window.clear(); }
    void display() { window.display(); }

    bool poll_exit();

    template<typename T>
    void draw(const T& drawable) { window.draw(drawable); }
};

}

#endif
