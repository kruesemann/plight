#include "labyrinth/include/system/uniform_color.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/uniform_color.h"

#include "plight/include/common/time.h"
#include "plight/include/common/vector.h"
#include "plight/include/graphics/update_uniform.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::UniformColor
{
    /*
        Update all uniforms "u_color" based on elapsed time
    */
    void
    update(entt::registry const& rRegistry,
           Plight::Time const& rDelta)
    {
        static Plight::Vector<float, 4> color(std::array<float, 4>{1.0f, 0.0f, 1.0f, 1.0f});
        rRegistry.view<Component::Position const,
                       Component::UniformColor const>().each([&](auto const entity, auto const& rPosition, auto const& rColorUniform)
                                                             {
                                                                 color.m_data[3] = static_cast<float>(sin(rDelta.m_seconds) / 2.0f + 0.5f);
                                                                 color.m_data[1] = static_cast<float>(cos(rDelta.m_seconds) / 2.0f + 0.5f);
                                                                 color.m_data[2] = static_cast<float>(sin(rDelta.m_seconds) / 2.0f + 0.5f);
                                                             
                                                                 Plight::Graphics::UpdateUniform::update(rColorUniform.m_uniformData,
                                                                                                         color);
                                                             });
    }
}
