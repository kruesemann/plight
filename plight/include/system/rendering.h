#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Plight::Graphics
{
    class Renderer;
    struct RenderTarget;
}

namespace Plight::System::Rendering
{
    /*
        System for rendering entities to a render target
    */

    template<typename EnttExcludeType,
             typename... FilterComponents>
    void                        update(entt::registry&,
                                       Graphics::Renderer&,
                                       Graphics::RenderTarget const&);
}
