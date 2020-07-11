#pragma once
#include "plight/include/component/uniform_buffer_data.h"
#include "plight/include/component/uniform_texture_data.h"


namespace Plight::Component
{
    /*
        Component holding everthing needed for rendering an object
    */
    struct RenderData
    {
        // OpenGL-internal shader program id
        unsigned int            m_shaderProgramId = 0;

        // Total number of visits to the drawn vertices (some vertices are visited multiple times while drawing)
        size_t                  m_vertexVisits = 0;

        // OpenGL-internal vertex array object id
        unsigned int            m_vertexArrayObject = 0;

        // Uniform buffers that should be updated before rendering (with update data)
        mutable std::vector<UniformBufferData> m_uniformBufferUpdates;

        // Uniform textures that should be updated before rendering
        std::vector<UniformTextureData> m_uniformTextureUpdates;
    };
}
