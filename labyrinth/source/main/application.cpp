#include "labyrinth/include/main/application.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/uniform_color.h"
#include "labyrinth/include/component/uniform_model_view_matrix.h"

#include "labyrinth/include/system/movement.h"
#include "labyrinth/include/system/uniform_color.h"
#include "labyrinth/include/system/uniform_model_view_matrix.h"

#include "plight/include/common/time.h"

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

#include <iostream>


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


namespace Labyrinth
{
    /*
        Start the game
    */
    void
    Application::run()
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
        auto& rPosition = registry.assign<Component::Position>(entity);

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

        Component::UniformModelViewMatrix uniformModelViewMatrix;
        uniformModelViewMatrix.m_uniformData = Plight::Graphics::UniformDataFactory::create(shaderManager,
                                                                                            shaderName,
                                                                                            Plight::Graphics::UniformInfo(Plight::String("u_modelViewMatrix"),
                                                                                                                          Plight::Graphics::EUniformType::FloatMat4));
        registry.assign<Component::UniformModelViewMatrix>(entity, uniformModelViewMatrix);

        Component::UniformColor uniformColor;
        uniformColor.m_uniformData = Plight::Graphics::UniformDataFactory::create(shaderManager,
                                                                                  shaderName,
                                                                                  Plight::Graphics::UniformInfo(Plight::String("u_color"),
                                                                                                                Plight::Graphics::EUniformType::FloatVec4));
        registry.assign<Component::UniformColor>(entity, uniformColor);

        size_t frameCount = 0;
        auto timestamp = Plight::Time::now();

        while (window.pollEvents())
        {
            auto const delta = Plight::Time::now();

            if (delta - timestamp >= 1.0f)
            {
                std::cout << frameCount << " fps\n";
                frameCount = 0;
                timestamp = delta;
            }

            System::Movement::update(registry, delta);
            System::UniformModelViewMatrix::update(registry);
            System::UniformColor::update(registry, delta);

            renderer.render(rRenderData);
            window.update();

            ++frameCount;
        }

        registry.destroy(entity);
    }
}
