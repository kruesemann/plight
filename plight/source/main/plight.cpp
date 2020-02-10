#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/render_data_factory.h"
#include "plight/include/graphics/renderer.h"
#include "plight/include/graphics/shader_manager.h"


void test()
{
    Plight::Window window;
    Plight::Graphics::Renderer renderer;
    Plight::Graphics::ShaderManager shaderManager;

    renderer.setBlendFunction(Plight::Graphics::EBlendFactor::CurrentAlpha,
                              Plight::Graphics::EBlendFactor::OneMinusCurrentAlpha);

    Plight::String positionAttribute("a_position");
    std::vector<Plight::Graphics::Attribute> attributes = {
        Plight::Graphics::Attribute{positionAttribute, 2, std::vector<float>{1.0f, 1.0f,    // top right
                                                                             1.0f, 0.0f,    // bottom right
                                                                             0.0f, 0.0f,    // bottom left
                                                                             0.0f, 1.0f}}   // top left
    };
    std::vector<int> indices = {0, 1, 3, 1, 2, 3};

    auto const renderData = Plight::Graphics::RenderDataFactory::create(shaderManager, Plight::String("test_shader"), attributes, indices);

    while (window.pollEvents())
    {
        renderer.render(renderData);
        window.update();
    }
}
