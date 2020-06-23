#pragma once
#include <cmath>

namespace Plight
{
    /*
        Integer square root
    */
    inline
    int
    sqrt(int x)
    {
        return static_cast<int>(std::sqrtf(static_cast<float>(x)));
    }
}
