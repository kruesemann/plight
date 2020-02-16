#include "labyrinth/include/system/movement.h"

#include "labyrinth/include/component/position.h"

#include "plight/include/common/time.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::Movement
{
    void
    update(entt::registry& rRegistry,
           Plight::Time const& rDelta)
    {
        rRegistry.view<Component::Position>().each([&](auto const entity, auto& rPosition)
                                                   {
                                                       rPosition.m_x = static_cast<float>(sin(2.0f * rDelta.m_seconds) / 2.0f);
                                                       rPosition.m_y = static_cast<float>(cos(3.0f * rDelta.m_seconds) / 2.0f);
                                                       rRegistry.replace<Component::Position>(entity, rPosition);
                                                   });
    }
}
