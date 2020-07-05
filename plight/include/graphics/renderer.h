#pragma once
#include "plight/include/graphics/color.h"


typedef unsigned int GLenum;

namespace Plight
{
    namespace Component
    {
        struct RenderData;
    }
}

namespace Plight::Graphics
{
    /*
        Equation for alpha blending
        e.g.: result = factorCurrent * current + factorBuffer + buffer
    */
    enum class EBlendEquation
    {
        CurrentPlusBuffer,
        CurrentMinusBuffer,
        BufferMinusCurrent
    };

    /*
        Factor for alpha blending equation
        e.g.: result = currentAlpha * current + (1 - currentAlpha) * buffer
    */
    enum class EBlendFactor
    {
        Zero,
        One,
        CurrentColor,
        OneMinusCurrentColor,
        BufferColor,
        OneMinusBufferColor,
        CurrentAlpha,
        OneMinusCurrentAlpha,
        BufferAlpha,
        OneMinusBufferAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha
    };

    /*
        Manages Gl settings and renders vertices with shader, attributes and uniforms to a render target
    */
    class Renderer
    {
    public:

                                Renderer();

        void                    setEnableAlphaBlending(bool);

        void                    setBlendEquation(EBlendEquation);

        void                    setBlendFunction(EBlendFactor, EBlendFactor);

        void                    setBlendFunctionRgbAndAlphaSeparately(EBlendFactor, EBlendFactor, EBlendFactor, EBlendFactor);

        void                    setConstantColor(Color const&);

        void                    setEnableDepthTesting(bool);

        void                    setViewport(int, int);

        void                    clear();

        void                    render(Component::RenderData const&);

    private:

        static GLenum           getGlBlendFactor(EBlendFactor const&);

    };
}
