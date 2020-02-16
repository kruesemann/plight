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
    }
}

namespace Plight::Graphics::RenderDataFactory
{
    /*
        Factory for initializing shader attributes and creating components that enable vertex rendering
    */
    Component::RenderData create(ShaderManager&,
                                 String const&,
                                 std::vector<Attribute> const&,
                                 std::vector<int> const&);
}
