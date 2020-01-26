#include "plight/include/main/plight.h"

#include "plight/include/main/window.h"

#include "glew/include/glew.h"

#include <iostream>


void test()
{
    Plight::Window window;

    // Initialize GL
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::exception("Graphics error: Unable to initialize GLEW");

    // Enable depth testing
    //glEnable(GL_DEPTH_TEST);

    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Set clear color
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (window.pollEvents())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        window.update();
    }
}
