#pragma once
#include "plight/include/component/render_data.h"

#include <vector>


namespace Plight
{
    class String;

    namespace Graphics
    {
        struct Attribute;
        struct Shader;
    }
}

namespace Plight::Graphics::RenderDataFactory
{
    /*
        Factory for initializing shader attributes and creating components that enable vertex rendering
    */
    Component::RenderData create(Shader const&,
                                 std::vector<Attribute> const&,
                                 std::vector<int> const&);
}
