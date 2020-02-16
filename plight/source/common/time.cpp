#include "plight/include/common/time.h"

#include "glfw/include/glfw3.h"


namespace Plight
{
    Time
    Time::now()
    {
        return {glfwGetTime()};
    }
}
