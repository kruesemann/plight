#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight
{
    struct Time;
}

namespace Labyrinth::System::UniformColor
{
    /*
        System for updating the uniform "u_color"
    */

    void                    update(entt::registry&,
                                   Plight::Time const&);
}
