#include "plight/include/graphics/render_target.h"

#include "plight/include/graphics/texture.h"

#include "glew/include/glew.h"

#include <exception>


namespace Plight::Graphics
{
    RenderTarget::RenderTarget(int width,
                               int height)
        : m_width(width)
        , m_height(height)
    {
    }

    RenderTarget::RenderTarget(Texture const& rTexture)
        : m_width(rTexture.m_width)
        , m_height(rTexture.m_height)
    {
        glGenFramebuffers(1, &m_framebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

        // Set texture as colour attachement #0
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rTexture.m_textureObject, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::exception("Graphics error: Unable to create framebuffer");
    }
}
