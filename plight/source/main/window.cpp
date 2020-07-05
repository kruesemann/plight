#include "plight/include/main/window.h"

#include "glfw/include/glfw3.h"

#include <exception>


namespace Plight
{
    Window::Glfw::Glfw()
    {
        if (!glfwInit())
            throw std::exception("Window error: Unable to initialize GLFW");
    }

    Window::Glfw::~Glfw()
    {
        glfwTerminate();
    }

    Window::Window()
    {
        initialize();
    }

    Window::~Window()
    {
        // Destroy window
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    void
    Window::setSize(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;
        glfwSetWindowSize(m_pWindow, m_width, m_height);
    }

    std::pair<int, int>
    Window::getSize() const
    {
        std::pair<int, int> result;
        glfwGetWindowSize(m_pWindow, &result.first, &result.second);
        return result;
    }

    void
    Window::toggleFullscreen()
    {
        auto* pMonitor = glfwGetPrimaryMonitor();
        auto const* pMonitorMode = glfwGetVideoMode(pMonitor);
        switch (m_screenMode)
        {
        case EScreenMode::Windowed:
            glfwSetWindowMonitor(m_pWindow, pMonitor, 0, 0, pMonitorMode->width, pMonitorMode->height, pMonitorMode->refreshRate);
            m_screenMode = EScreenMode::Fullscreen;
            break;
        case EScreenMode::Fullscreen:
            glfwSetWindowMonitor(m_pWindow, NULL, 50, 50, m_width, m_height, pMonitorMode->refreshRate);
            glViewport(0, 0, m_width, m_height);
            m_screenMode = EScreenMode::Windowed;
            break;
        }
    }

    void
    Window::setResizable(bool resizable)
    {
        if (m_options.m_resizable != resizable)
        {
            m_options.m_resizable = resizable;
            initialize();
        }
    }

    void
    Window::setScaleToMonitor(bool scaleToMonitor)
    {
        if (m_options.m_scaleToMonitor != scaleToMonitor)
        {
            m_options.m_scaleToMonitor = scaleToMonitor;
            initialize();
        }
    }

    void
    Window::setEnableVSync(bool enableVSync)
    {
        if (m_options.m_enableVSync != enableVSync)
        {
            m_options.m_enableVSync = enableVSync;
            initialize();
        }
    }

    /*
        Request the window to close
    */
    void
    Window::requestQuit()
    {
        m_quitRequested = true;
    }

    /*
        Polls events from GLFW; returns false if the window should be closed
    */
    bool
    Window::pollEvents() const
    {
        glfwPollEvents();
        if (m_quitRequested)
        {
            m_quitRequested = false;
            return false;
        }
        return !glfwWindowShouldClose(m_pWindow);
    }

    int
    Window::pollKey(int key) const
    {
        return glfwGetKey(m_pWindow, key);
    }

    /*
        Update the display
    */
    void
    Window::update()
    {
        glfwSwapBuffers(m_pWindow);
    }

    /*
        Create a new GLFW window
    */
    void
    Window::initialize()
    {
        // Destroy old window
        if (m_pWindow)
        {
            glfwDestroyWindow(m_pWindow);
            m_pWindow = nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Prevent window resizing
        glfwWindowHint(GLFW_RESIZABLE, m_options.m_resizable ? GL_TRUE : GL_FALSE);

        // Automatically adjust to monitor
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, m_options.m_scaleToMonitor ? GL_TRUE : GL_FALSE);

        // Create window
        m_pWindow = glfwCreateWindow(m_width, m_height, "Plight", NULL, NULL);
        if (!m_pWindow)
            throw std::exception("Window error: Unable to create GLFW window");

        // Get GL context
        glfwMakeContextCurrent(m_pWindow);

        // Enable V-Sync
        glfwSwapInterval(m_options.m_enableVSync ? 1 : 0);
    }
}
