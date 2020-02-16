#include "plight/include/common/time.h"

#include "glfw/include/glfw3.h"


namespace Plight
{
    /*
        Get current timestamp
    */
    Time
    Time::now()
    {
        return {glfwGetTime()};
    }
}
