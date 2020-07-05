#pragma once
#include "plight/include/common/matrix.h"

#include <cmath>


namespace Plight::Graphics
{
    float
    computeFocalLength(int diagonal, int fieldOfView)
    {
        return 0.5f * diagonal / std::tan(fieldOfView * 3.14159265359f / 720.0f);
    }
    
    Matrix<float, 4, 4>
    compute2DCameraMatrix(int diagonal, int fieldOfView, int width, int height)
    {
        auto const focalLength = computeFocalLength(diagonal, fieldOfView);
        return Matrix<float, 4, 4>(std::array<float, 16>{
            focalLength / width,                 0.0f, 0.0f, 0.0f,
                           0.0f, focalLength / height, 0.0f, 0.0f,
                           0.0f,                 0.0f, 1.0f, 0.0f,
                           0.0f,                 0.0f, 0.0f, 1.0f});
    }

    Matrix<float, 4, 4>
    compute2DProjectionMatrix(int x, int y)
    {
        return Matrix<float, 4, 4>(std::array<float, 16>{
                              1.0f,                   0.0f, 0.0f, 0.0f,
                              0.0f,                   1.0f, 0.0f, 0.0f,
                              0.0f,                   0.0f, 1.0f, 0.0f,
            -x / 10000.0f, -y / 10000.0f, 0.0f, 1.0f});
    }
}
