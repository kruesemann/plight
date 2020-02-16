#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/render_data_factory.h"
#include "plight/include/graphics/renderer.h"
#include "plight/include/graphics/shader_manager.h"
#include "plight/include/graphics/update_uniform.h"
#include "plight/include/graphics/update_uniform_cpp.h"
#include "plight/include/graphics/uniform_data_factory.h"
#include "plight/include/graphics/uniform_info.h"

#include "entt/src/entt/entt.hpp"

#include "glew/include/glew.h"
#include "glfw/include/glfw3.h"

#include <iostream>


struct Position
{
    float x = 0;
    float y = 0;
};

struct ModelViewMatrixUniform
{
    Plight::Component::UniformData m_uniformData;
};

struct ColorUniform
{
    Plight::Component::UniformData m_uniformData;
};

void
move(entt::registry& rRegistry, double delta)
{
    rRegistry.view<Position>().each([&](auto const entity, auto& rPosition)
                                    {
                                        rPosition.x = static_cast<float>(sin(2.0f * delta) / 2.0f);
                                        rPosition.y = static_cast<float>(cos(3.0f * delta) / 2.0f);
                                        rRegistry.replace<Position>(entity, rPosition);
                                    });
}

void
updateModelViewMatrixUniform(entt::registry& rRegistry)
{
    static Plight::Matrix<float, 4, 4> modelViewMatrix(std::array<float, 16>{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
    rRegistry.view<Position, ModelViewMatrixUniform>().each([&](auto const entity, auto const& rPosition, auto const& rModelViewMatrixUniform)
                                                            {
                                                                modelViewMatrix.m_data[12] = rPosition.x;
                                                                modelViewMatrix.m_data[13] = rPosition.y;
                                                                Plight::Graphics::UpdateUniform::update(rModelViewMatrixUniform.m_uniformData,
                                                                                                        modelViewMatrix);
                                                            });
}

void
updateColorUniform(entt::registry& rRegistry, double delta)
{
    static Plight::Vector<float, 4> color(std::array<float, 4>{1.0f, 0.0f, 1.0f, 1.0f});
    rRegistry.view<Position, ColorUniform>().each([&](auto const entity, auto const& rPosition, auto const& rColorUniform)
                                                  {
                                                      color.m_data[3] = static_cast<float>(sin(delta) / 2.0f + 0.5f);
                                                      color.m_data[1] = static_cast<float>(cos(delta) / 2.0f + 0.5f);
                                                      color.m_data[2] = static_cast<float>(sin(delta) / 2.0f + 0.5f);
                                                  
                                                      Plight::Graphics::UpdateUniform::update(rColorUniform.m_uniformData,
                                                                                              color);
                                                  });
}

void
debugCallback(GLenum,
              GLenum type,
              GLuint,
              GLenum severity,
              GLsizei,
              const GLchar* message,
              const void*)
{
    if (type == GL_DEBUG_TYPE_ERROR)
        std::cerr << "GL Error: " << "type = " << type
            << ", severity = " << severity
            << ", message = " << message << '\n';
}

void test()
{
    Plight::Window window;
    window.setEnableVSync(false);

    entt::registry registry;
    Plight::Graphics::Renderer renderer;
    Plight::Graphics::ShaderManager shaderManager;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, 0);

    renderer.setBlendFunction(Plight::Graphics::EBlendFactor::CurrentAlpha,
                              Plight::Graphics::EBlendFactor::OneMinusCurrentAlpha);
    renderer.setEnableAlphaBlending(true);

    auto entity = registry.create();
    auto& rPosition = registry.assign<Position>(entity);

    Plight::String shaderName("test_shader");
    std::vector<Plight::Graphics::Attribute> attributes = {
        Plight::Graphics::Attribute{Plight::String("a_position"), 2, std::vector<float>{ 0.5f,  0.5f,     // top right
                                                                                         0.5f, -0.5f,     // bottom right
                                                                                        -0.5f, -0.5f,     // bottom left
                                                                                        -0.5f,  0.5f}},   // top left
        Plight::Graphics::Attribute{Plight::String("a_color"), 3, std::vector<float>{1.0f, 0.0f, 0.0f,
                                                                                     1.0f, 1.0f, 0.0f,
                                                                                     0.0f, 1.0f, 0.0f,
                                                                                     0.0f, 0.0f, 1.0f}}
    };
    std::vector<int> indices = {0, 1, 3, 1, 2, 3};

    auto const& rRenderData = registry.assign<Plight::Component::RenderData>(entity,
                                                                             Plight::Graphics::RenderDataFactory::create(shaderManager,
                                                                                                                         shaderName,
                                                                                                                         attributes,
                                                                                                                         indices));

    ModelViewMatrixUniform modelViewMatrixUniform;
    modelViewMatrixUniform.m_uniformData = Plight::Graphics::UniformDataFactory::create(shaderManager,
                                                                                        shaderName,
                                                                                        Plight::Graphics::UniformInfo(Plight::String("u_modelViewMatrix"),
                                                                                                                      Plight::Graphics::EUniformType::FloatMat4));
    registry.assign<ModelViewMatrixUniform>(entity, modelViewMatrixUniform);

    ColorUniform colorUniform;
    colorUniform.m_uniformData = Plight::Graphics::UniformDataFactory::create(shaderManager,
                                                                              shaderName,
                                                                              Plight::Graphics::UniformInfo(Plight::String("u_color"),
                                                                                                            Plight::Graphics::EUniformType::FloatVec4));
    registry.assign<ColorUniform>(entity, colorUniform);

    size_t frameCount = 0;
    double timestamp = glfwGetTime();

    while (window.pollEvents())
    {
        auto const delta = glfwGetTime();

        if (delta - timestamp >= 1)
        {
            std::cout << frameCount << " fps\n";
            frameCount = 0;
            timestamp = delta;
        }

        move(registry, delta);
        updateModelViewMatrixUniform(registry);
        updateColorUniform(registry, delta);
        
        renderer.render(rRenderData);
        window.update();

        ++frameCount;
    }

    registry.destroy(entity);
}
