#include "labyrinth/include/system/uniform_model_view_matrix.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/uniform_model_view_matrix.h"

#include "plight/include/common/matrix.h"
#include "plight/include/graphics/update_uniform.h"

#include "entt/src/entt/entt.hpp"


namespace Labyrinth::System::UniformModelViewMatrix
{
    void
    update(entt::registry const& rRegistry)
    {
        static Plight::Matrix<float, 4, 4> modelViewMatrix(std::array<float, 16>{
            1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        });
        rRegistry.view<Component::Position const,
                       Component::UniformModelViewMatrix const>().each([&](auto const entity, auto const& rPosition, auto const& rUniformModelViewMatrix)
                                                                       {
                                                                           modelViewMatrix.m_data[12] = rPosition.m_x;
                                                                           modelViewMatrix.m_data[13] = rPosition.m_y;
                                                                           Plight::Graphics::UpdateUniform::update(rUniformModelViewMatrix.m_uniformData,
                                                                                                                   modelViewMatrix);
                                                                       });
    }
}
