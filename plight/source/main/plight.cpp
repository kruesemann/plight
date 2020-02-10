#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/render_data_factory.h"
#include "plight/include/graphics/renderer.h"
#include "plight/include/graphics/shader_manager.h"

#include "glew/include/glew.h"
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

    auto const renderData = Plight::Graphics::RenderDataFactory::create(shaderManager, Plight::String("test_shader"), attributes, indices);

    auto const location = glGetUniformLocation(renderData.m_shaderProgramId, "u_opacity");
    if (location < 0)
        return;

    while (window.pollEvents())
    {
        glUseProgram(renderData.m_shaderProgramId);
        glUniform1f(location, sin(glfwGetTime()) / 2.0f + 0.5f);
        
        renderer.render(renderData);
        window.update();
    }
}
