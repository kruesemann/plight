#include "labyrinth/include/system/uniform_color.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/uniform_color.h"

#include "plight/include/common/time.h"
#include "plight/include/common/vector.h"
#include "plight/include/graphics/update_uniform_buffer.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::UniformColor
{
    /*
        Update all uniforms "u_color" based on elapsed time
    */
    void
    update(entt::registry& rRegistry,
           Plight::Time const& rTime)
    {
        static Plight::Vector<float, 4> color(std::array<float, 4>{1.0f, 0.0f, 1.0f, 1.0f});
        rRegistry.view<Component::Position,
                       Component::UniformColor>().each([&](auto const& rPosition, auto& rColorUniform)
                                                       {
                                                           Plight::Graphics::UniformBufferUpdateData<float> update;
                                                           update.m_data[0] = 1.0f;
                                                           update.m_data[1] = static_cast<float>(cos(rTime.m_ms) / 2.0f + 0.5f);
                                                           update.m_data[2] = static_cast<float>(sin(rTime.m_ms) / 2.0f + 0.5f);
                                                           update.m_data[3] = static_cast<float>(sin(rTime.m_ms) / 2.0f + 0.5f);
                                                           rColorUniform.m_uniformBufferData.m_floatUpdateData = {update};
                                                       
                                                           Plight::Graphics::updateUniformBuffer(rColorUniform.m_uniformBufferData);
                                                       });
    }
}
