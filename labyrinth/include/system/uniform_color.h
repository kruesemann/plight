#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight
{
    struct Time;
}

namespace Labyrinth::System::UniformColor
{
    void                    update(entt::registry const&,
                                   Plight::Time const&);
}
