#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight
{
    struct Time;
}

namespace Labyrinth::System::Movement
{
    void                    update(entt::registry&,
                                   Plight::Time const&);
}
