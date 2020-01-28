#pragma once


namespace Plight::Component
{
    /*
        Component holding everthing needed for rendering an object except uniforms
    */
    struct RenderData
    {
        unsigned int            m_shaderProgramId = 0;
        unsigned int            m_vertexNumber = 0;
        unsigned int            m_vertexArrayObject = 0;
    };
}
