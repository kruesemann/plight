#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "plight/include/component/render_data.h"

#include "plight/include/graphics/renderer.h"

#include "glew/include/glew.h"

#include <iostream>


void test()
{
    Plight::Window window;
    Plight::Graphics::Renderer renderer;

    renderer.setBlendFunction(Plight::Graphics::EBlendFactor::CurrentAlpha,
                              Plight::Graphics::EBlendFactor::OneMinusCurrentAlpha);

    Plight::Component::RenderData renderData;
    renderData.m_vertexNumber = 6;


    // Setup shader
    {
        auto ok = GL_FALSE;
        char infoLog[512];

        auto const* vertexShaderSource = "#version 400 core\n"
            "in vec3 a_vertexPosition; \n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(a_vertexPosition, 1.0);\n"
            "}\0";
        auto const* fragmentShaderSource = "#version 400 core\n"
            "void main()\n"
            "{\n"
            "   gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
            "}\n\0";

        auto const vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShaderId);
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            //Get info log
            glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
            std::cerr << "vertex shader fail: " << infoLog << "\n";
        }


        auto const fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShaderId);
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            //Get info log
            glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
            std::cerr << "fragment shader fail: " << infoLog << "\n";
        }


        // Generate program
        renderData.m_shaderProgramId = glCreateProgram();
        glAttachShader(renderData.m_shaderProgramId, vertexShaderId);
        glAttachShader(renderData.m_shaderProgramId, fragmentShaderId);

        // Link program
        glLinkProgram(renderData.m_shaderProgramId);
        glGetProgramiv(renderData.m_shaderProgramId, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            glGetProgramInfoLog(renderData.m_shaderProgramId, 512, NULL, infoLog);
            std::cout << "program fail: " << infoLog << "\n";
        }

        glDetachShader(renderData.m_shaderProgramId, vertexShaderId);
        glDetachShader(renderData.m_shaderProgramId, fragmentShaderId);

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

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

    glDeleteProgram(renderData.m_shaderProgramId);
}
