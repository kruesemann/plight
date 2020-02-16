#pragma once
#include "plight/include/component/uniform_data.h"


namespace Labyrinth::Component
{
    /*
        Component for updating uniform "u_color"
    */
    struct UniformColor
    {
        Plight::Component::UniformData m_uniformData;
    };
}