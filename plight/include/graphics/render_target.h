#pragma once


namespace Plight::Graphics
{
    struct Texture;
}

namespace Plight::Graphics
{
    /*
        Holds size and id of a framebuffer so that objects can be rendered to the display or to a texture
    */
    struct RenderTarget
    {
                                RenderTarget(int,
                                             int);

        explicit                RenderTarget(Texture const&);

        // Width
        int                     m_width;

        // Height
        int                     m_height;

        // GLFW-internal framebuffer id (0 = display)
        unsigned int            m_framebufferId = 0;
    };
}
