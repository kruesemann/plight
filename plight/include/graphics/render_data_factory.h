#pragma once
#include "plight/include/component/render_data.h"

#include <vector>


namespace Plight
{
    class String;

    namespace Graphics
    {
        struct Attribute;
        class ShaderManager;
    }
}

namespace Plight::Graphics::RenderDataFactory
{
    Component::RenderData create(ShaderManager&,
                                 String const&,
                                 std::vector<Attribute> const&,
                                 std::vector<int> const&);
}
