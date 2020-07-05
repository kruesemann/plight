#include "plight/include/graphics/renderer.h"

#include "plight/include/component/render_data.h"

#include "plight/include/graphics/update_uniform_buffer.h"

#include "glew/include/glew.h"

#include <exception>


namespace Plight::Graphics
{
    Renderer::Renderer()
    {
        // Initialize GL
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
            throw std::exception("Graphics error: Unable to initialize GLEW");
    }

    void
    Renderer::setEnableAlphaBlending(bool enableAlphaBlending)
    {
        if (enableAlphaBlending)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    void
    Renderer::setBlendEquation(EBlendEquation blendEquation)
    {
        switch (blendEquation)
        {
        case EBlendEquation::CurrentPlusBuffer:
            glBlendEquation(GL_FUNC_ADD);
            break;
        case EBlendEquation::CurrentMinusBuffer:
            glBlendEquation(GL_FUNC_SUBTRACT);
            break;
        case EBlendEquation::BufferMinusCurrent:
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            break;
        }
    }

    void
    Renderer::setBlendFunction(EBlendFactor blendFactorCurrent, EBlendFactor blendFactorBuffer)
    {
        glBlendFunc(getGlBlendFactor(blendFactorCurrent),
                    getGlBlendFactor(blendFactorBuffer));
    }

    void
    Renderer::setBlendFunctionRgbAndAlphaSeparately(EBlendFactor blendFactorCurrentRgb,
                                                    EBlendFactor blendFactorBufferRgb,
                                                    EBlendFactor blendFactorCurrentAlpha,
                                                    EBlendFactor blendFactorBufferAlpha)
    {
        glBlendFuncSeparate(getGlBlendFactor(blendFactorCurrentRgb),
                            getGlBlendFactor(blendFactorBufferRgb),
                            getGlBlendFactor(blendFactorCurrentAlpha),
                            getGlBlendFactor(blendFactorBufferAlpha));
    }

    void
    Renderer::setConstantColor(Color const& rColor)
    {
        glBlendColor(rColor.r / 255.0f,
                     rColor.g / 255.0f,
                     rColor.b / 255.0f,
                     rColor.a / 255.0f);
    }

    void
    Renderer::setEnableDepthTesting(bool enableDepthTesting)
    {
        if (enableDepthTesting)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void
    Renderer::setViewport(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    /*
        Clears render buffers
    */
    void
    Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /*
        Render given vertex array with given shader and uniforms to given render target
    */
    void
    Renderer::render(Component::RenderData const& rRenderData)
    {
        // Use shader
        glUseProgram(rRenderData.m_shaderProgramId);

        for (auto const& rUniformBufferUpdate : rRenderData.m_uniformBufferUpdates)
            updateUniformBuffer(rUniformBufferUpdate);
        rRenderData.m_uniformBufferUpdates.clear();

        // Draw vertices
        glBindVertexArray(rRenderData.m_vertexArrayObject);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(rRenderData.m_vertexVisits), GL_UNSIGNED_INT, 0);
    }

    GLenum
    Renderer::getGlBlendFactor(EBlendFactor const& rBlendFactor)
    {
        switch (rBlendFactor)
        {
        case EBlendFactor::Zero:
            return GL_ZERO;
        case EBlendFactor::One:
            return GL_ONE;
        case EBlendFactor::CurrentColor:
            return GL_SRC_COLOR;
        case EBlendFactor::OneMinusCurrentColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case EBlendFactor::BufferColor:
            return GL_DST_COLOR;
        case EBlendFactor::OneMinusBufferColor:
            return GL_ONE_MINUS_DST_COLOR;
        case EBlendFactor::CurrentAlpha:
            return GL_SRC_ALPHA;
        case EBlendFactor::OneMinusCurrentAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case EBlendFactor::BufferAlpha:
            return GL_DST_ALPHA;
        case EBlendFactor::OneMinusBufferAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case EBlendFactor::ConstantColor:
            return GL_CONSTANT_COLOR;
        case EBlendFactor::OneMinusConstantColor:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case EBlendFactor::ConstantAlpha:
            return GL_CONSTANT_ALPHA;
        case EBlendFactor::OneMinusConstantAlpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        }
        return GL_ZERO;
    }
}
