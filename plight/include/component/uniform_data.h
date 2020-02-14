#pragma once


namespace Plight::Component
{
    /*
        Wrapping component holding everthing needed for updating a shader uniform
        Not intended as an actual component, but member of a uniform-specific component
    */
    struct UniformData
    {
        // OpenGL-internal shader program id
        unsigned int            m_shaderProgramId = 0;

        // OpenGL-internal uniform location
        int                     m_location = 0;
    };

    /*
        Wrapping component holding everthing needed for updating a shader uniform buffer
        Not intended as an actual component, but member of a uniform-buffer-specific component
    */
    struct UniformBufferData
    {
        // OpenGL-internal uniform buffer object
        unsigned int            m_uniformBufferObject = 0;
    };
}