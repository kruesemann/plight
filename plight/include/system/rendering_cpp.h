#pragma once
#include "plight/include/system/rendering.h"

#include "plight/include/component/render_data.h"

#include "plight/include/graphics/renderer.h"

#include "entt/src/entt/entt.hpp"
#include <tuple>


namespace Plight::System::Rendering
{
    /*
        Renders all entities with given filter components and without components in given entt::exclude_t to given render target
    */
    template<typename EnttExcludeType,
             typename... FilterComponents>
    void
    update(entt::registry& rRegistry,
           Graphics::Renderer& rRenderer,
           Graphics::RenderTarget const& rRenderTarget)
    {
        rRenderer.setRenderTarget(rRenderTarget);
        rRenderer.clear();
        rRegistry.view<Component::RenderData,
                       FilterComponents...>(EnttExcludeType{}).each([&](Component::RenderData const& rRenderData, auto ...)
                                                                    {
                                                                        rRenderer.render(rRenderData);
                                                                    });
    }
}
