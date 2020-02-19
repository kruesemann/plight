#pragma once


namespace Plight
{
    namespace Component
    {
        struct UniformBufferData;
    }
}

namespace Plight::Graphics
{
    /*
        Function for updating shader uniform buffers
    */
    void                        updateUniformBuffer(Component::UniformBufferData const&);
}
