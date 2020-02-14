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
    Component::UniformData      create(ShaderManager&,
                                       String const&,
                                       Graphics::UniformInfo const&);

    Component::UniformBufferData create(ShaderManager&,
                                        String const&,
                                        Graphics::UniformBufferInfo const&);
}
