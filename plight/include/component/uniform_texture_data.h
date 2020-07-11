#pragma once


namespace Plight::Component
{
    /*
        Component holding everthing needed for binding a texture uniform to an OpenGL-internal texture unit
    */
    struct UniformTextureData
    {
        // OpenGL-internal uniform location in the shader
        int                     m_uniformLocation = -1;

        // OpenGL-internal texture unit to which the appropriate texture object is bound
        unsigned int            m_textureUnit = 0;
    };
}
