#pragma once
#include "plight/include/component/render_data.h"

#include <vector>


namespace Plight
{
    class String;

    namespace Graphics
    {
        class ShaderManager;

        struct Attribute;
        struct UniformData;
    }
}

namespace Plight::Graphics::RenderDataFactory
{
    Component::RenderData create(ShaderManager&,
                                 String const&,
                                 std::vector<Attribute> const&,
                                 std::vector<int> const&,
                                 std::vector<UniformData> const&);
}
