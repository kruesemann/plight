#pragma once
#include "plight/include/graphics/uniform_buffer_update_data.h"

#include <vector>


namespace Plight::Component
{
    /*
        Wrapping component holding everything needed for updating a shader uniform buffer
        Not intended as an actual component, but member of a uniform-buffer-specific component
    */
    struct UniformBufferData
    {
        // OpenGL-internal uniform buffer object
        unsigned int            m_uniformBufferObject = 0;

        // Data for a Buffer update
        std::vector<Graphics::UniformBufferUpdateData<bool>> m_boolUpdateData;
        std::vector<Graphics::UniformBufferUpdateData<int>> m_intUpdateData;
        std::vector<Graphics::UniformBufferUpdateData<unsigned int>> m_uintUpdateData;
        std::vector<Graphics::UniformBufferUpdateData<float>> m_floatUpdateData;
        std::vector<Graphics::UniformBufferUpdateData<double>> m_doubleUpdateData;
    };
}