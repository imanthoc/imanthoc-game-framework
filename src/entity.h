#ifndef ENTITY_H
#define ENTITY_H

#include "render_target.h"

namespace igf
{

struct Entity
{
    virtual void update() = 0;
    virtual void draw(Render_Target& targeT) = 0;
};

}

#endif
