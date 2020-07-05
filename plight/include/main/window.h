#pragma once
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

        void                    setSize(unsigned int, unsigned int);
        std::pair<int, int>     getSize() const;

        void                    toggleFullscreen();

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

        // Window width (not resolution)
        unsigned int            m_width = 800;

        // Window height (not resolution)
        unsigned int            m_height = 600;

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
