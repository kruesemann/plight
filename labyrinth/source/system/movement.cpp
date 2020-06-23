#include "labyrinth/include/system/movement.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/velocity.h"

#include "plight/include/common/time.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::Movement
{
    /*
        Move all entities based on elapsed time
    */
    void
    update(entt::registry& rRegistry)
    {
        rRegistry.view<Component::Velocity>().each([&](auto& rVelocity)
                                                   {
                                                       rVelocity.m_delta[0] += 1;
                                                       rVelocity.m_delta[1] += 1;
                                                   });
    }
}
