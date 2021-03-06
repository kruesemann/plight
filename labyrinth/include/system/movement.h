#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight
{
    struct Time;
}

namespace Labyrinth::System::Movement
{
    /*
        System for moving entities around based on velocity
    */

    void                    update(entt::registry&);
}
