#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "plight/include/component/render_data.h"

#include "plight/include/graphics/renderer.h"
#include "plight/include/graphics/shader_manager.h"

#include "glew/include/glew.h"

#include <iostream>


void test()
{
    Plight::Window window;
    Plight::Graphics::Renderer renderer;
    Plight::Graphics::ShaderManager shaderManager;

    renderer.setBlendFunction(Plight::Graphics::EBlendFactor::CurrentAlpha,
                              Plight::Graphics::EBlendFactor::OneMinusCurrentAlpha);

    Plight::Component::RenderData renderData;
    renderData.m_vertexNumber = 6;


    // Setup shader
    renderData.m_shaderProgramId = shaderManager.getOrCreateShader(Plight::String("test_shader"));

    // Setup attribute array
    {
        unsigned int vertexBufferObject = 0;
        unsigned int indexBufferObject = 0;

        float attributes[] = {
            1.0f, 1.0f,     // top right
            1.0f, 0.0f,     // bottom right
            0.0f, 0.0f,     // bottom left
            0.0f, 1.0f      // top left
        };

        int indices[] = {0, 1, 3, 1, 2, 3};

        glGenVertexArrays(1, &renderData.m_vertexArrayObject);
        glGenBuffers(1, &vertexBufferObject);
        glGenBuffers(1, &indexBufferObject);

        glBindVertexArray(renderData.m_vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), attributes, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderData.m_vertexNumber * sizeof(int), indices, GL_STATIC_DRAW);

        auto const location = glGetAttribLocation(renderData.m_shaderProgramId, "a_vertexPosition");
        glVertexAttribPointer(location, static_cast<GLint>(2), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(2 * sizeof(float)), (void*)0);
        glEnableVertexAttribArray(location);
    }

    while (window.pollEvents())
    {
        renderer.render(renderData);
        window.update();
    }
}
