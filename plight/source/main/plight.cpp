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
move(entt::registry& rRegistry, entt::entity const& rEntity)
{
    auto& rPosition = rRegistry.get<Position>(rEntity);
    rPosition.x = static_cast<float>(sin(2.0f * glfwGetTime()) / 2.0f);
    rPosition.y = static_cast<float>(cos(3.0f * glfwGetTime()) / 2.0f);
    rRegistry.replace<Position>(rEntity, rPosition);
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

    auto const renderData = Plight::Graphics::RenderDataFactory::create(shaderManager,
                                                                        shaderName,
                                                                        attributes,
                                                                        indices);

    ModelViewMatrixUniform modelViewMatrixUniform;
    modelViewMatrixUniform.m_uniformData = Plight::Graphics::UniformDataFactory::create(shaderManager,
                                                                                        shaderName,
                                                                                        Plight::Graphics::UniformInfo(Plight::String("u_modelViewMatrix"),
                                                                                                                      Plight::Graphics::EUniformType::FloatMat4));

    ColorUniform colorUniform;
    colorUniform.m_uniformData = Plight::Graphics::UniformDataFactory::create(shaderManager,
                                                                              shaderName,
                                                                              Plight::Graphics::UniformInfo(Plight::String("u_color"),
                                                                                                            Plight::Graphics::EUniformType::FloatVec4));

    Plight::Matrix<float, 4, 4> modelViewMatrix(std::array<float, 16>{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
    Plight::Vector<float, 4> color(std::array<float, 4>{1.0f, 0.0f, 1.0f, 1.0f});

    while (window.pollEvents())
    {
        move(registry, entity);

        auto const& rPosition = registry.get<Position>(entity);
        modelViewMatrix.m_data[12] = rPosition.x;
        modelViewMatrix.m_data[13] = rPosition.y;

        color.m_data[3] = static_cast<float>(sin(glfwGetTime()) / 2.0f + 0.5f);
        color.m_data[1] = static_cast<float>(cos(glfwGetTime()) / 2.0f + 0.5f);
        color.m_data[2] = static_cast<float>(sin(glfwGetTime()) / 2.0f + 0.5f);

        Plight::Graphics::UpdateUniform::update(modelViewMatrixUniform.m_uniformData,
                                                modelViewMatrix);
        Plight::Graphics::UpdateUniform::update(colorUniform.m_uniformData,
                                                color);
        
        renderer.render(renderData);
        window.update();
    }

    registry.destroy(entity);
}
