#pragma once
#include <vector>


namespace Plight::Component
{
    struct FloatUniformBufferData
    {
        // OpenGL-internal uniform buffer object id
        unsigned int            m_uniformBufferObject = 0;

        // Data (including padding)
        std::vector<float>      m_data;
    };

    struct IntUniformBufferData
    {
        // OpenGL-internal uniform buffer object id
        unsigned int            m_uniformBufferObject = 0;

        // Data (including padding)
        std::vector<int>        m_data;
    };

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

        // Float uniform data
        std::vector<FloatUniformBufferData> m_floatUniformBufferData;

        // Int uniform data
        std::vector<IntUniformBufferData> m_intUniformBufferData;
    };
}
