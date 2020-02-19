#pragma once
#include "plight/include/component/uniform_buffer_data.h"


namespace Labyrinth::Component
{
    /*
        Component for updating uniform buffer "b_color"
    */
    struct UniformColor
    {
        Plight::Component::UniformBufferData m_uniformBufferData;
    };
}
