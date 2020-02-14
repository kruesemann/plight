#pragma once
#include <vector>


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
    };
}
