#pragma once
#include "plight/include/graphics/render_target.h"

#include <utility>


struct GLFWwindow;

namespace Plight
{
    /*
        Screen mode
    */
    enum class EScreenMode
    {
        Windowed,
        Fullscreen
    };

    /*
        Window for displaying and input polling
    */
    class Window
    {
    public:

                                Window();
                                ~Window();

        void                    setPosition(int, int);
        void                    setSize(int, int);

        void                    setResolution(int, int);
        Graphics::RenderTarget const& getRenderTarget() const;

        void                    toggleFullscreen();
        EScreenMode             getScreenMode() const;

        void                    setResizable(bool);

        void                    setScaleToMonitor(bool);

        void                    setEnableVSync(bool);

        void                    requestQuit();

        bool                    pollEvents() const;

        int                     pollKey(int) const;

        void                    update();

    private:

        /*
            Helper struct for GLFW initialization and termination
        */
        struct Glfw
        {
                                Glfw();
                                ~Glfw();
        };

        /*
            Helper struct for Window Options
        */
        struct Options
        {
            // Flag if the window is externally resizable
            bool                m_resizable = true;

            // Flag if the window is resized based on the monitor content scale
            bool                m_scaleToMonitor = true;

            // Flag if VSync is enabled
            bool                m_enableVSync = true;
        };

        void                    initialize();

        // Internal GLFW
        Glfw                    m_glfw;

        // last window non-fullscreen position
        int                     m_position[2] = {50, 50};

        // last window non-fullscreen size (not resolution)
        int                     m_size[2] = {800, 600};

        // Render target with width, height and framebuffer id (= 0)
        Graphics::RenderTarget  m_renderTarget;

        // Window options
        Options                 m_options;

        // Internal GLFW window
        GLFWwindow*             m_pWindow = nullptr;

        // Screen mode
        EScreenMode             m_screenMode = EScreenMode::Windowed;

        // Flag if the window was requested to close
        mutable bool            m_quitRequested = false;
    };
}
