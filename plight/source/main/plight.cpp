#include "plight/include/main/plight.h"

#include "plight/include/component/attribute_array.h"
#include "plight/include/component/position.h"
#include "plight/include/component/shader.h"
#include "plight/include/component/velocity.h"

#include "plight/include/graphics/error.h"

#include "plight/include/system/movement.h"
#include "plight/include/system/renderer.h"

#include "entt/src/entt/entt.hpp"

#include "glew/include/glew.h"
#include "glfw/include/glfw3.h"

#include <iostream>


namespace Plight
{
    void test()
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Prevent window resizing
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Automatically adjust to monitor
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);

        // Create window
        auto* window = glfwCreateWindow(800, 600, "Plight", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        // Get GL context
        glfwMakeContextCurrent(window);

        // Initialize GL
        glewExperimental = GL_TRUE;
        glewInit();

        // Enable V-Sync
        glfwSwapInterval(1);

        // Enable alpha blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glfwMakeContextCurrent(window);

        entt::registry registry;
        System::Renderer renderer;

        Component::AttributeArrayData attributeArrayData;
        attributeArrayData.m_attributes = {1.0f, 1.0f,    // top right
                                           1.0f, 0.0f,    // bottom right
                                           0.0f, 0.0f,    // bottom left
                                           0.0f, 1.0f};   // top left
        attributeArrayData.m_indices = {0, 1, 3, 1, 2, 3};
        attributeArrayData.m_attributeInfo = {Component::AttributeInfo{2, String("a_vertexPosition")}};
        attributeArrayData.m_vertexAttributeSize = 2;

        Component::AttributeArray attributeArray;
        attributeArray.m_vertexNumber = 6;

        auto entity = registry.create();
        registry.assign<Component::ShaderInfo>(entity, String("test_shader"));
        registry.assign<Component::AttributeArrayData>(entity, attributeArrayData);
        registry.assign<Component::AttributeArray>(entity, attributeArray);

        try
        {
            renderer.initialize(registry);
        }
        catch (Graphics::Error const& rError)
        {
            std::cout << rError.getMessage() << "\n";
            return;
        }

        auto lastTime = glfwGetTime();

        while (true)
        {
            auto currentTime = glfwGetTime();

            glfwPollEvents();
            if (glfwWindowShouldClose(window))
                return;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer.render(registry);
            glfwSwapBuffers(window);
        }

        renderer.cleanUp(registry);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
