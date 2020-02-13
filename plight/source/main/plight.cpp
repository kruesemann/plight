#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/render_data_factory.h"
#include "plight/include/graphics/renderer.h"
#include "plight/include/graphics/shader_manager.h"
#include "plight/include/graphics/uniform.h"
#include "plight/include/graphics/uniform_data.h"

#include "glfw/include/glfw3.h"


void test()
{
    Plight::Window window;
    Plight::Graphics::Renderer renderer;
    Plight::Graphics::ShaderManager shaderManager;

    renderer.setBlendFunction(Plight::Graphics::EBlendFactor::CurrentAlpha,
                              Plight::Graphics::EBlendFactor::OneMinusCurrentAlpha);
    renderer.setEnableAlphaBlending(true);

    std::vector<Plight::Graphics::Attribute> attributes = {
        Plight::Graphics::Attribute{Plight::String("a_position"), 2, std::vector<float>{1.0f, 1.0f,     // top right
                                                                                        1.0f, 0.0f,     // bottom right
                                                                                        0.0f, 0.0f,     // bottom left
                                                                                        0.0f, 1.0f}},   // top left
        Plight::Graphics::Attribute{Plight::String("a_color"), 3, std::vector<float>{1.0f, 0.0f, 0.0f,
                                                                                     1.0f, 1.0f, 0.0f,
                                                                                     0.0f, 1.0f, 0.0f,
                                                                                     0.0f, 0.0f, 1.0f}}
    };
    std::vector<int> indices = {0, 1, 3, 1, 2, 3};
    std::vector<Plight::Graphics::UniformData> uniformData = {
        Plight::Graphics::UniformData{Plight::String("u_colorData"), 8}
    };

    auto renderData = Plight::Graphics::RenderDataFactory::create(shaderManager,
                                                                  Plight::String("test_shader"),
                                                                  attributes,
                                                                  indices,
                                                                  uniformData);

    float lighting[3] = {1.0f, 1.0f, 1.0f};

    while (window.pollEvents())
    {
        auto const sinus = static_cast<float>(sin(glfwGetTime()) / 2.0f + 0.5f);
        lighting[1] = static_cast<float>(cos(glfwGetTime()) / 2.0f + 0.5f);
        lighting[2] = static_cast<float>(sin(glfwGetTime()) / 2.0f + 0.5f);

        renderData.m_floatUniformBufferData.back().m_data = {sinus, 0.0f, 0.0f, 0.0f, lighting[0], lighting[1], lighting[2], 0.0f};

        Plight::Graphics::Uniform::updateFloatBuffer(renderData.m_floatUniformBufferData.back().m_uniformBufferObject,
                                                     renderData.m_floatUniformBufferData.back().m_data);
        
        renderer.render(renderData);
        window.update();
    }
}
