#pragma once
#include "entt/src/entt/fwd.hpp"

#include <vector>


namespace Labyrinth::Component
{
    struct Position;
}

namespace Labyrinth::System::UniformColor
{
    /*
        System for updating the uniform "u_color"
    */

    void                    update(entt::registry&,
                                   std::vector<Component::Position> const&);
}
