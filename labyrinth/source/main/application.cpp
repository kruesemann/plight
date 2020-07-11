#include "labyrinth/include/main/application.h"

#include "labyrinth/include/component/collider.h"
#include "labyrinth/include/component/grid_collider.h"
#include "labyrinth/include/component/hit_points.h"
#include "labyrinth/include/component/player.h"
#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/tile_map.h"
#include "labyrinth/include/component/uniform_color.h"
#include "labyrinth/include/component/uniform_model_view_matrix.h"
#include "labyrinth/include/component/velocity.h"

#include "labyrinth/include/map/path_finding.h"

#include "labyrinth/include/system/collision.h"
#include "labyrinth/include/system/movement.h"
#include "labyrinth/include/system/position.h"
#include "labyrinth/include/system/uniform_color.h"
#include "labyrinth/include/system/uniform_model_view_matrix.h"

#include "plight/include/common/math.h"
#include "plight/include/common/time.h"

#include "plight/include/component/uniform_buffer_data.h"
#include "plight/include/component/uniform_texture_data.h"

#include "plight/include/main/window.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/camera.h"
#include "plight/include/graphics/render_data_factory.h"
#include "plight/include/graphics/renderer.h"
#include "plight/include/graphics/shader_manager.h"
#include "plight/include/graphics/update_uniform_buffer.h"
#include "plight/include/graphics/uniform_buffer_info.h"
#include "plight/include/graphics/uniform_texture_info.h"

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
        window.setEnableVSync(true);

        entt::registry registry;
        Plight::Graphics::Renderer renderer;
        Plight::Graphics::ShaderManager shaderManager;

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugCallback, 0);

        renderer.setBlendFunction(Plight::Graphics::EBlendFactor::CurrentAlpha,
                                  Plight::Graphics::EBlendFactor::OneMinusCurrentAlpha);
        renderer.setEnableAlphaBlending(true);

        Plight::String const cameraMatricesUniformBlockName("b_cameraMatrices");
        Plight::String const modelViewMatrixUniformBlockName("b_modelViewMatrix");
        Plight::String const textureUniformName("u_texture");
        Plight::String const pathUniformName("b_path");
        Plight::Graphics::UniformBufferInfo cameraMatricesUniformBufferInfo(cameraMatricesUniformBlockName,
                                                                            32 * sizeof(float),
                                                                            true /* shareBetweenShaders */);
        Plight::Graphics::UniformBufferInfo modelViewMatrixUniformBufferInfo(modelViewMatrixUniformBlockName,
                                                                             16 * sizeof(float));
        Plight::Graphics::TextureData textureData;
        textureData.m_data = {
            255,   0,   0, 255,
              0, 255,   0, 255,
              0,   0, 255, 255,
            255, 255,   0, 255
        };
        textureData.m_width = 2;
        textureData.m_height = 2;
        Plight::Graphics::UniformTextureInfo textureUniformInfo(textureUniformName,
                                                                Plight::Graphics::Texture::create(textureData));
        auto const& rPlayerShader = shaderManager.getOrCreateShader(Plight::String("test_shader_player"),
                                                                    {cameraMatricesUniformBufferInfo,
                                                                     modelViewMatrixUniformBufferInfo},
                                                                    {textureUniformInfo});
        auto const& rMapShader = shaderManager.getOrCreateShader(Plight::String("test_shader_map"),
                                                                 {cameraMatricesUniformBufferInfo,
                                                                  modelViewMatrixUniformBufferInfo,
                                                                  Plight::Graphics::UniformBufferInfo(pathUniformName,
                                                                                                      80 * sizeof(float))},
                                                                 {});

        auto const& rUniformTextureData = rPlayerShader.m_uniformTextureDataMap.at(textureUniformName);
        glUseProgram(rPlayerShader.m_programId);
        glUniform1i(rUniformTextureData.m_uniformLocation, rUniformTextureData.m_textureUnit);
        auto uniformBufferData = rMapShader.m_uniformBufferDataMap.at(cameraMatricesUniformBlockName);

        auto playerEntity = registry.create();
        registry.assign<Component::Player>(playerEntity);
        registry.assign<Component::Position>(playerEntity);
        registry.assign<Component::Velocity>(playerEntity);
        registry.assign<Component::HitPoints>(playerEntity, 1000);
        registry.assign<Component::Collider>(playerEntity, Component::Collider{std::vector<Component::ColliderRectangle>{Component::ColliderRectangle{{-5000, -5000}, {10000, 10000}}},
                                                                               {10000, 10000}});
        //registry.assign<Component::Collider>(playerEntity, Component::Collider{std::vector<Component::ColliderRectangle>{Component::ColliderRectangle{{-10000, -10000}, {20000, 20000}}},
        //                                                                       {20000, 20000}});

        std::vector<Plight::Graphics::Attribute> playerAttributes = {
            Plight::Graphics::Attribute{Plight::String("a_position"), 2, std::vector<float>{ 0.5f,  0.5f,
                                                                                             0.5f, -0.5f,
                                                                                            -0.5f, -0.5f,
                                                                                            -0.5f,  0.5f}},
            //Plight::Graphics::Attribute{Plight::String("a_position"), 2, std::vector<float>{ 1.0f,  1.0f,
            //                                                                                 1.0f, -1.0f,
            //                                                                                -1.0f, -1.0f,
            //                                                                                -1.0f,  1.0f}},
            Plight::Graphics::Attribute{Plight::String("a_uv"), 2, std::vector<float>{1.0f, 1.0f,
                                                                                      1.0f, 0.0f,
                                                                                      0.0f, 0.0f,
                                                                                      0.0f, 1.0f}},
        };
        std::vector<int> playerIndices = {0, 1, 3, 1, 2, 3};

        registry.assign<Plight::Component::RenderData>(playerEntity,
                                                       Plight::Graphics::RenderDataFactory::create(rPlayerShader,
                                                                                                   playerAttributes,
                                                                                                   playerIndices));

        Component::UniformModelViewMatrix playerUniformModelViewMatrix;
        playerUniformModelViewMatrix.m_uniformBufferData = rPlayerShader.m_uniformBufferDataMap.at(modelViewMatrixUniformBlockName);
        registry.assign<Component::UniformModelViewMatrix>(playerEntity, playerUniformModelViewMatrix);


        auto mapEntity = registry.create();
        auto& rPosition = registry.assign<Component::Position>(mapEntity);
        rPosition.m_value[0] = -50000;
        rPosition.m_value[1] = -50000;
        registry.assign<Component::Velocity>(mapEntity);
        Component::UniformColor mapUniformPath;
        mapUniformPath.m_uniformBufferData = rMapShader.m_uniformBufferDataMap.at(pathUniformName);
        registry.assign<Component::UniformColor>(mapEntity, mapUniformPath);

        Component::TileMap tileMap;
        tileMap.m_width = 100;
        tileMap.m_height = 100;
        tileMap.m_tileWidth = 10000;
        tileMap.m_tileHeight = 10000;
        auto& rCollider = registry.assign<Component::GridCollider>(mapEntity);
        rCollider.m_dim[0] = tileMap.m_width;
        rCollider.m_dim[1] = tileMap.m_height;
        rCollider.m_cellDim[0] = tileMap.m_tileWidth;
        rCollider.m_cellDim[1] = tileMap.m_tileHeight;
        std::vector<float> vertices;
        std::vector<float> color;
        std::vector<int> indices;
        for (int i = 0; i < rCollider.m_dim[1]; ++i)
        {
            float const y = static_cast<float>(i);

            for (int j = 0; j < rCollider.m_dim[0]; ++j)
            {
                float const x = static_cast<float>(j);

                vertices.insert(vertices.end(), {x + 1.0f, y + 1.0f,
                                                 x + 1.0f, y,
                                                 x, y,
                                                 x, y + 1.0f});

                if (i == 0 || i == rCollider.m_dim[1] - 1 || j == 0 || j == rCollider.m_dim[0] - 1 ||
                    (i != 4 && j == 6) ||
                    (i == 7 && j != 2))
                {
                    color.insert(color.end(), {0.0f, 0.0f, 0.1f,
                                               0.0f, 0.0f, 0.1f,
                                               0.0f, 0.0f, 0.1f,
                                               0.0f, 0.0f, 0.1f});
                    rCollider.m_occupancies.emplace_back(true);
                    tileMap.m_tiles.emplace_back(Map::Tile{Map::ETileType::Wall});
                }
                else
                {
                    color.insert(color.end(), {0.25f, 0.0f, 0.0f,
                                               0.25f, 0.0f, 0.0f,
                                               0.25f, 0.0f, 0.0f,
                                               0.25f, 0.0f, 0.0f});
                    rCollider.m_occupancies.emplace_back(false);
                    tileMap.m_tiles.emplace_back(Map::Tile{Map::ETileType::Ground});
                }

                auto const lastIndex = indices.empty() ? -1 : indices.back();
                indices.insert(indices.end(), {lastIndex + 1, lastIndex + 2, lastIndex + 4, lastIndex + 2, lastIndex + 3, lastIndex + 4});
            }
        }

        std::vector<Plight::Graphics::Attribute> attributes = {
            Plight::Graphics::Attribute{Plight::String("a_position"), 2, vertices},
            Plight::Graphics::Attribute{Plight::String("a_color"), 3, color},
        };

        registry.assign<Plight::Component::RenderData>(mapEntity,
                                                       Plight::Graphics::RenderDataFactory::create(rMapShader,
                                                                                                   attributes,
                                                                                                   indices));

        Component::UniformModelViewMatrix uniformModelViewMatrix;
        uniformModelViewMatrix.m_uniformBufferData = rMapShader.m_uniformBufferDataMap.at(modelViewMatrixUniformBlockName);
        registry.assign<Component::UniformModelViewMatrix>(mapEntity, uniformModelViewMatrix);

        auto enemyEntity = registry.create();
        registry.assign<Component::Position>(enemyEntity, Component::Position{-30000, 0});
        registry.assign<Component::Velocity>(enemyEntity);
        registry.assign<Component::Collider>(enemyEntity, Component::Collider{std::vector<Component::ColliderRectangle>{Component::ColliderRectangle{{-5000, -5000}, {10000, 10000}}},
                                                                              {10000, 10000}});

        std::vector<Plight::Graphics::Attribute> enemyAttributes = {
            Plight::Graphics::Attribute{Plight::String("a_position"), 2, std::vector<float>{ 0.5f,  0.5f,
                                                                                             0.5f, -0.5f,
                                                                                            -0.5f, -0.5f,
                                                                                            -0.5f,  0.5f}},
            Plight::Graphics::Attribute{Plight::String("a_uv"), 2, std::vector<float>{1.0f, 1.0f,
                                                                                      1.0f, 0.0f,
                                                                                      0.0f, 0.0f,
                                                                                      0.0f, 1.0f}},
        };
        std::vector<int> enemyIndices = {0, 1, 3, 1, 2, 3};

        registry.assign<Plight::Component::RenderData>(enemyEntity,
                                                       Plight::Graphics::RenderDataFactory::create(rPlayerShader,
                                                                                                   enemyAttributes,
                                                                                                   enemyIndices));

        Component::UniformModelViewMatrix enemyUniformModelViewMatrix;
        enemyUniformModelViewMatrix.m_uniformBufferData = rPlayerShader.m_uniformBufferDataMap.at(modelViewMatrixUniformBlockName);
        registry.assign<Component::UniformModelViewMatrix>(enemyEntity, enemyUniformModelViewMatrix);

        System::UniformModelViewMatrix::initialize(registry);

        size_t frameCount = 0;
        auto timestamp = Plight::Time::now();
        auto lastSecond = Plight::Time::now();

        double counter = 0;
        std::vector<Component::Position> path;
        std::set<Map::ETileType> allowedTiles = {Map::ETileType::Ground};
        auto const normalize = [](int& rx, int& ry, double delta)
        {
            auto const round = [](double x)
            {
                return x < 0 ? std::floor(x) : std::ceil(x);
            };

            static double const speed = 50.0;
            auto const norm = Plight::sqrt(rx * rx + ry * ry);
            if (norm > speed * delta)
            {
                rx = static_cast<int>(round(speed * delta * rx / norm));
                ry = static_cast<int>(round(speed * delta * ry / norm));
                return false;
            }
            return true;
        };

        int lastHitPoints = 1000;

        while (window.pollEvents())
        {
            auto const now = Plight::Time::now();
            auto const delta = std::min(now - timestamp, 100.0);
            timestamp = now;

            if (timestamp - lastSecond >= 1000.0)
            {
                std::cout << frameCount << " fps\n";
                frameCount = 0;
                lastSecond = timestamp;
            }

            auto& rVelocity = registry.get<Component::Velocity>(playerEntity);
            rVelocity.m_delta[0] = 0;
            rVelocity.m_delta[1] = 0;

            if (window.pollKey(GLFW_KEY_W) == GLFW_PRESS)
                rVelocity.m_delta[1] += 1;
            if (window.pollKey(GLFW_KEY_A) == GLFW_PRESS)
                rVelocity.m_delta[0] -= 1;
            if (window.pollKey(GLFW_KEY_S) == GLFW_PRESS)
                rVelocity.m_delta[1] -= 1;
            if (window.pollKey(GLFW_KEY_D) == GLFW_PRESS)
                rVelocity.m_delta[0] += 1;
            if (window.pollKey(GLFW_KEY_SPACE) == GLFW_PRESS)
                window.toggleFullscreen();

            if (rVelocity.m_delta[0] != 0 && rVelocity.m_delta[1] != 0)
            {
                static auto const norm = std::sqrt(0.5);
                rVelocity.m_delta[0] *= static_cast<int>(100.0 * delta * norm);
                rVelocity.m_delta[1] *= static_cast<int>(100.0 * delta * norm);
            }
            else
            {
                rVelocity.m_delta[0] *= static_cast<int>(100.0 * delta);
                rVelocity.m_delta[1] *= static_cast<int>(100.0 * delta);
            }

            auto const& rEnemyPosition = registry.get<Component::Position>(enemyEntity);
            auto const& rPlayerPosition = registry.get<Component::Position>(playerEntity);
            if (path.empty())
                path = Map::findShortestPath(tileMap,
                                             registry.get<Component::Position>(mapEntity),
                                             rEnemyPosition,
                                             rPlayerPosition,
                                             allowedTiles);
            auto& rEnemyVelocity = registry.get<Component::Velocity>(enemyEntity);
            if (!path.empty())
            {
                auto const nextTile = path.back();
                rEnemyVelocity.m_delta[0] = nextTile.m_value[0] - rEnemyPosition.m_value[0];
                rEnemyVelocity.m_delta[1] = nextTile.m_value[1] - rEnemyPosition.m_value[1];
                if (normalize(rEnemyVelocity.m_delta[0], rEnemyVelocity.m_delta[1], delta))
                    path.pop_back();
            }
            else
            {
                rEnemyVelocity.m_delta[0] = 0;
                rEnemyVelocity.m_delta[1] = 0;
            }

            //System::Movement::update(registry);
            System::Collision::update(registry, delta);
            System::Position::update(registry);
            System::UniformModelViewMatrix::update(registry);
            System::UniformColor::update(registry, path);

            auto const& rWindowRenderTarget = window.getRenderTarget();
            Plight::Graphics::UniformBufferUpdateData<float> cameraUpdate;
            cameraUpdate.m_offset = 0;
            auto const projectionMatrix = Plight::Graphics::compute2DProjectionMatrix(rPlayerPosition.m_value[0], rPlayerPosition.m_value[1]);
            auto const cameraMatrix = Plight::Graphics::compute2DCameraMatrix(35, 60, rWindowRenderTarget.m_width, rWindowRenderTarget.m_height);
            for (auto value : projectionMatrix.m_data)
                cameraUpdate.m_data.push_back(value);
            for (auto value : cameraMatrix.m_data)
                cameraUpdate.m_data.push_back(value);
            uniformBufferData.m_floatUpdateData = {cameraUpdate};
            Plight::Graphics::updateUniformBuffer(uniformBufferData);

            renderer.clear();
            renderer.render(registry.get<Plight::Component::RenderData>(mapEntity), rWindowRenderTarget);
            renderer.render(registry.get<Plight::Component::RenderData>(playerEntity), rWindowRenderTarget);
            renderer.render(registry.get<Plight::Component::RenderData>(enemyEntity), rWindowRenderTarget);
            window.update();

            ++frameCount;

            auto const currentHitPoints = registry.get<Component::HitPoints>(playerEntity).m_value;
            if (currentHitPoints < 0)
            {
                std::cout << "DEATH\n";
                break;
            }
            else if (currentHitPoints < lastHitPoints)
                std::cout << "OUCHIE(" << currentHitPoints << ")\n";
            else if (currentHitPoints > lastHitPoints)
                std::cout << "HEAL(" << lastHitPoints << ")\n";
            lastHitPoints = currentHitPoints;
        }
    }
}
