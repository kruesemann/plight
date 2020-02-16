#pragma once
#include "plight/include/component/uniform_data.h"


namespace Labyrinth::Component
{
    /*
        Component for updating uniform "u_modelViewMatrix"
    */
    struct UniformModelViewMatrix
    {
        Plight::Component::UniformData m_uniformData;
    };
}
