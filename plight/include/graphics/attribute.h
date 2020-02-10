#pragma once
#include "plight/include/common/string.h"

#include <vector>


namespace Plight::Graphics
{
    /*
        Data of a vertex attribute for a shader
    */
    struct Attribute
    {
        // Attribute name in the shader
        String                  m_name;

        // Size of attribute data for one vertex
        size_t                  m_dimension = 0;

        // Accumulated attribute data for all vertices
        std::vector<float>      m_data;
    };
}
