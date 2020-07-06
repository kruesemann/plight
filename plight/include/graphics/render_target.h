#pragma once


namespace Plight::Graphics
{
    /*
        Holds size and id of a framebuffer so that objects can be rendered to the display or to a texture
    */
    struct RenderTarget
    {
                                RenderTarget(int width,
                                             int height,
                                             bool isDisplay);

        // Width
        int                     m_width;

        // Height
        int                     m_height;

        // GLFW-internal framebuffer id (0 = display)
        unsigned int            m_framebufferId = 0;
    };
}
