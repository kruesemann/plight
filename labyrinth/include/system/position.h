#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight
{
    struct Time;
}

namespace Labyrinth::System::Position
{
    /*
        System for moving entities around based on velocity
    */

    void                    update(entt::registry&);
}
