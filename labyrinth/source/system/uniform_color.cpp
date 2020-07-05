#include "labyrinth/include/system/uniform_color.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/uniform_color.h"

#include "plight/include/common/vector.h"
#include "plight/include/graphics/update_uniform_buffer.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::UniformColor
{
    /*
        Update all uniforms "u_path" based on the given path
    */
    void
    update(entt::registry& rRegistry,
           std::vector<Component::Position> const& rPath)
    {
        rRegistry.view<Component::Position,
                       Component::UniformColor>().each([&](auto const& rPosition, auto& rColorUniform)
                                                       {
                                                           Plight::Graphics::UniformBufferUpdateData<float> update;
                                                           for (size_t i = 0; i < 20; ++i)
                                                           {
                                                               if (i < rPath.size())
                                                               {
                                                                   update.m_data.emplace_back(static_cast<float>(rPath[i].m_value[0]));
                                                                   update.m_data.emplace_back(static_cast<float>(rPath[i].m_value[1]));
                                                               }
                                                               else
                                                               {
                                                                   update.m_data.emplace_back(0.0f);
                                                                   update.m_data.emplace_back(0.0f);
                                                               }
                                                               update.m_data.emplace_back(0.0f);
                                                               update.m_data.emplace_back(0.0f);
                                                           }
                                                           rColorUniform.m_uniformBufferData.m_floatUpdateData = {update};
                                                           Plight::Graphics::updateUniformBuffer(rColorUniform.m_uniformBufferData);
                                                       });
    }
}
