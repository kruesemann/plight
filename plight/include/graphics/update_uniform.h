#pragma once


namespace Plight
{
    namespace Component
    {
        struct UniformBufferData;
        struct UniformTextureData;
    }
}

namespace Plight::Graphics
{
    /*
        Functions for updating shader uniforms
    */

    void                        updateUniformBuffer(Component::UniformBufferData const&);

    void                        updateUniformTexture(Component::UniformTextureData const&);
}
