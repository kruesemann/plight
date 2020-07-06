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
        : m_renderTarget(m_size[0],
                         m_size[1],
                         true /* isDisplay */)
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
    Window::setPosition(int x, int y)
    {
        m_position[0] = x;
        m_position[1] = y;
    }

    void
    Window::setSize(int width, int height)
    {
        m_size[0] = width;
        m_size[1] = height;
    }

    void
    Window::setResolution(int width, int height)
    {
        m_renderTarget.m_width = width;
        m_renderTarget.m_height = height;
    }

    Graphics::RenderTarget const&
    Window::getRenderTarget() const
    {
        return m_renderTarget;
    }

    void
    Window::toggleFullscreen()
    {
        auto* pMonitor = glfwGetPrimaryMonitor();
        auto const* pMonitorMode = glfwGetVideoMode(pMonitor);
        switch (m_screenMode)
        {
        case EScreenMode::Windowed:
            m_screenMode = EScreenMode::Fullscreen;
            glfwSetWindowMonitor(m_pWindow, pMonitor, 0, 0, pMonitorMode->width, pMonitorMode->height, pMonitorMode->refreshRate);
            break;
        case EScreenMode::Fullscreen:
            m_screenMode = EScreenMode::Windowed;
            glfwSetWindowMonitor(m_pWindow, NULL, m_position[0], m_position[1], m_size[0], m_size[1], pMonitorMode->refreshRate);
            break;
        }
    }

    EScreenMode
    Window::getScreenMode() const
    {
        return m_screenMode;
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
        m_pWindow = glfwCreateWindow(m_size[0], m_size[1], "Plight", NULL, NULL);
        if (!m_pWindow)
            throw std::exception("Window error: Unable to create GLFW window");

        // Get GL context
        glfwMakeContextCurrent(m_pWindow);

        // Enable V-Sync
        glfwSwapInterval(m_options.m_enableVSync ? 1 : 0);

        // Set window as user pointer for callbacks
        glfwSetWindowUserPointer(m_pWindow, this);

        // Set window/framebuffer resize callback
        glfwSetFramebufferSizeCallback(m_pWindow,
                                       [](GLFWwindow* pGlfwWindow,
                                          int width,
                                          int height)
                                       {
                                           if (auto* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGlfwWindow)))
                                               pWindow->setResolution(width, height);
                                       });

        // Set window resize callback
        glfwSetWindowSizeCallback(m_pWindow,
                                  [](GLFWwindow* pGlfwWindow,
                                     int width,
                                     int height)
                                  {
                                      if (auto* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGlfwWindow)))
                                          if (pWindow->getScreenMode() != EScreenMode::Fullscreen)
                                            pWindow->setSize(width, height);
                                  });

        // Set window position callback
        glfwSetWindowPosCallback(m_pWindow,
                                 [](GLFWwindow* pGlfwWindow,
                                    int x,
                                    int y)
                                 {
                                     if (auto* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(pGlfwWindow)))
                                         if (pWindow->getScreenMode() != EScreenMode::Fullscreen)
                                            pWindow->setPosition(x, y);
                                 });

        toggleFullscreen();
    }
}
