#include "labyrinth/include/system/uniform_model_view_matrix.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/uniform_model_view_matrix.h"

#include "plight/include/common/matrix.h"
#include "plight/include/component/render_data.h"
#include "plight/include/graphics/update_uniform_buffer.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::UniformModelViewMatrix
{
    void
    initialize(entt::registry& rRegistry)
    {
        rRegistry.view<Component::UniformModelViewMatrix>().each([&](auto& rUniformModelViewMatrix)
                                                                 {
                                                                     Plight::Graphics::UniformBufferUpdateData<float> update;
                                                                     update.m_offset = 0;
                                                                     update.m_data = {
                                                                         1.0f, 0.0f, 0.0f, 0.0f,
                                                                         0.0f, 1.0f, 0.0f, 0.0f,
                                                                         0.0f, 0.0f, 1.0f, 0.0f,
                                                                         0.0f, 0.0f, 0.0f, 1.0f
                                                                     };
                                                                     rUniformModelViewMatrix.m_uniformBufferData.m_floatUpdateData = {update};
                                                                     Plight::Graphics::updateUniformBuffer(rUniformModelViewMatrix.m_uniformBufferData);
                                                                 });
    }


    /*
        Update all uniforms "u_modelViewMatrix" based on position
    */
    void
    update(entt::registry& rRegistry)
    {
        rRegistry.view<Component::Position,
                       Component::UniformModelViewMatrix,
                       Plight::Component::RenderData>().each([&](auto const& rPosition, auto& rUniformModelViewMatrix, auto& rRenderData)
                                                             {
                                                                 Plight::Graphics::UniformBufferUpdateData<float> update;
                                                                 update.m_offset = 12 * sizeof(float);
                                                                 update.m_data = {rPosition.m_value[0] / 10000.0f, rPosition.m_value[1] / 10000.0f};
                                                                 rUniformModelViewMatrix.m_uniformBufferData.m_floatUpdateData = {update};
                                                                 rRenderData.m_uniformBufferUpdates.emplace_back(rUniformModelViewMatrix.m_uniformBufferData);
                                                             });
    }
}
