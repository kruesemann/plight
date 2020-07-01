#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Labyrinth::System::Collision
{
    /*
        System for resolving collisions between entities
    */

    void                        update(entt::registry&,
                                       double);
}
