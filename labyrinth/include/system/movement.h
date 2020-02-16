#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight
{
    struct Time;
}

namespace Labyrinth::System::Movement
{
    /*
        System for moving entities around based on elapsed time
    */
    void                    update(entt::registry&,
                                   Plight::Time const&);
}
