#pragma once
#include "plight/include/component/uniform_data.h"


namespace Plight
{
    class String;

    namespace Graphics
    {
        class ShaderManager;

        struct UniformBufferInfo;
        struct UniformInfo;
    }
}

namespace Plight::Graphics::UniformDataFactory
{
    /*
        Factory for initializing shader uniforms and creating components that enable uniform updates
    */
    Component::UniformData      create(ShaderManager&,
                                       String const&,
                                       Graphics::UniformInfo const&);

    /*
        Factory for initializing shader uniform buffers and creating components that enable uniform buffer updates
    */
    Component::UniformBufferData create(ShaderManager&,
                                        String const&,
                                        Graphics::UniformBufferInfo const&);
}
