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

    RenderTarget::RenderTarget(std::vector<Texture> const& rTextures)
    {
        if (rTextures.empty())
            throw std::exception("Graphics error: Unable to create framebuffer without data");

        if (rTextures.size() > 16)
            throw std::exception("Graphics error: Unable to create framebuffer with more than 16 render target textures");

        m_width = rTextures.front().m_width;
        m_height = rTextures.front().m_height;

        for (auto const& rTexture : rTextures)
            if (rTexture.m_width != m_width ||
                rTexture.m_height != m_height)
                throw std::exception("Graphics error: Unable to create framebuffer inconsistent data");

        glGenFramebuffers(1, &m_framebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

        // Set textures as colour attachements
        unsigned int colorAttachement = 0;
        std::vector<GLenum> colorAttachements;
        for (auto const& rTexture : rTextures)
        {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachement, rTexture.m_textureObject, 0);
            colorAttachements.push_back(GL_COLOR_ATTACHMENT0 + colorAttachement);
            ++colorAttachement;
        }

        glDrawBuffers(static_cast<GLsizei>(colorAttachements.size()), colorAttachements.data());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::exception("Graphics error: Unable to create framebuffer");
    }
}
