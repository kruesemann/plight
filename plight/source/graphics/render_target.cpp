#include "plight/include/graphics/render_target.h"

#include "glew/include/glew.h"


namespace Plight::Graphics
{
    RenderTarget::RenderTarget(int width,
                               int height,
                               bool isDisplay)
        : m_width(width)
        , m_height(height)
    {
        if (!isDisplay)
            glGenFramebuffers(1, &m_framebufferId);
    }
}
