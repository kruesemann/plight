#include "labyrinth/include/system/position.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/velocity.h"

#include "plight/include/common/time.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::Position
{
    /*
        Move all entities based on velocity
    */
    void
    update(entt::registry& rRegistry)
    {
        rRegistry.view<Component::Position,
                       Component::Velocity>().each([&](auto& rPosition, auto const& rVelocity)
                                                   {
                                                       rPosition.m_value[0] += rVelocity.m_delta[0];
                                                       rPosition.m_value[1] += rVelocity.m_delta[1];
                                                   });
    }
}
