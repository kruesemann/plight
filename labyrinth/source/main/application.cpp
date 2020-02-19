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
#include "plight/include/graphics/update_uniform_buffer.h"
#include "plight/include/graphics/uniform_buffer_info.h"

#include "entt/src/entt/entt.hpp"

#include "glew/include/glew.h"
#include "glfw/include/glfw3.h"

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

        Plight::String modelViewMatrixUniformName("b_modelViewMatrix");
        auto const& rShader = shaderManager.getOrCreateShader(Plight::String("test_shader"),
                                                              {Plight::Graphics::UniformBufferInfo(modelViewMatrixUniformName,
                                                                                                   16 * sizeof(float))});

        auto playerEntity = registry.create();
        registry.assign<Component::Position>(playerEntity);

        std::vector<Plight::Graphics::Attribute> playerAttributes = {
            Plight::Graphics::Attribute{Plight::String("a_position"), 2, std::vector<float>{ 0.5f,  0.5f,
                                                                                             0.5f, -0.5f,
                                                                                            -0.5f, -0.5f,
                                                                                            -0.5f,  0.5f}},
            Plight::Graphics::Attribute{Plight::String("a_color"), 3, std::vector<float>{1.0f, 1.0f, 0.0f,
                                                                                         1.0f, 1.0f, 0.0f,
                                                                                         1.0f, 1.0f, 0.0f,
                                                                                         1.0f, 1.0f, 0.0f}},
        };
        std::vector<int> playerIndices = {0, 1, 3, 1, 2, 3};

        registry.assign<Plight::Component::RenderData>(playerEntity,
                                                       Plight::Graphics::RenderDataFactory::create(rShader,
                                                                                                   playerAttributes,
                                                                                                   playerIndices));

        Component::UniformModelViewMatrix playerUniformModelViewMatrix;
        playerUniformModelViewMatrix.m_uniformBufferData = rShader.m_uniformBufferDataMap.at(modelViewMatrixUniformName);
        registry.assign<Component::UniformModelViewMatrix>(playerEntity, playerUniformModelViewMatrix);


        auto mapEntity = registry.create();
        registry.assign<Component::Position>(mapEntity);

        std::vector<float> vertices;
        std::vector<float> color;
        std::vector<int> indices;
        for (unsigned int i = 0; i < 10; ++i)
        {
            float const y = i - 5.0f;

            for (unsigned int j = 0; j < 10; ++j)
            {
                float const x = j - 5.0f;

                vertices.insert(vertices.end(), {x + 1.0f, y + 1.0f,
                                                 x + 1.0f, y,
                                                 x, y,
                                                 x, y + 1.0f});

                if (i == 0 || i == 9 || j == 0 || j == 9)
                    color.insert(color.end(), {0.0f, 0.0f, 0.1f,
                                               0.0f, 0.0f, 0.1f,
                                               0.0f, 0.0f, 0.1f,
                                               0.0f, 0.0f, 0.1f});
                else
                    color.insert(color.end(), {0.25f, 0.0f, 0.0f,
                                               0.25f, 0.0f, 0.0f,
                                               0.25f, 0.0f, 0.0f,
                                               0.25f, 0.0f, 0.0f});

                auto const lastIndex = indices.empty() ? -1 : indices.back();
                indices.insert(indices.end(), {lastIndex + 1, lastIndex + 2, lastIndex + 4, lastIndex + 2, lastIndex + 3, lastIndex + 4});
            }
        }


        std::vector<Plight::Graphics::Attribute> attributes = {
            Plight::Graphics::Attribute{Plight::String("a_position"), 2, vertices},
            Plight::Graphics::Attribute{Plight::String("a_color"), 3, color},
        };

        registry.assign<Plight::Component::RenderData>(mapEntity,
                                                       Plight::Graphics::RenderDataFactory::create(rShader,
                                                                                                   attributes,
                                                                                                   indices));

        Component::UniformModelViewMatrix uniformModelViewMatrix;
        uniformModelViewMatrix.m_uniformBufferData = rShader.m_uniformBufferDataMap.at(modelViewMatrixUniformName);
        registry.assign<Component::UniformModelViewMatrix>(mapEntity, uniformModelViewMatrix);

        System::UniformModelViewMatrix::initialize(registry);

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

            if (window.pollKey(GLFW_KEY_W) == GLFW_PRESS)
            {
                auto& rPosition = registry.get<Component::Position>(playerEntity);
                rPosition.m_y += 0.001f;
            }
            if (window.pollKey(GLFW_KEY_A) == GLFW_PRESS)
            {
                auto& rPosition = registry.get<Component::Position>(playerEntity);
                rPosition.m_x -= 0.001f;
            }
            if (window.pollKey(GLFW_KEY_S) == GLFW_PRESS)
            {
                auto& rPosition = registry.get<Component::Position>(playerEntity);
                rPosition.m_y -= 0.001f;
            }
            if (window.pollKey(GLFW_KEY_D) == GLFW_PRESS)
            {
                auto& rPosition = registry.get<Component::Position>(playerEntity);
                rPosition.m_x += 0.001f;
            }

            //System::Movement::update(registry, delta);
            System::UniformModelViewMatrix::update(registry);
            System::UniformColor::update(registry, delta);

            renderer.clear();
            renderer.render(registry.get<Plight::Component::RenderData>(mapEntity));
            renderer.render(registry.get<Plight::Component::RenderData>(playerEntity));
            window.update();

            ++frameCount;
        }

        registry.destroy(mapEntity);
        registry.destroy(playerEntity);
    }
}
