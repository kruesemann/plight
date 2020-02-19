#pragma once
#include "plight/include/common/string.h"


namespace Plight::Graphics
{
    /*
        Basic data for shader uniform buffer initialization
    */
    struct UniformBufferInfo
    {
                            UniformBufferInfo(String const& rName, size_t byteSize)
                                : m_name(rName)
                                , m_byteSize(byteSize) {}

        // Name of the uniform block in the shader
        String              m_name;

        // Raw size (in bytes) of the uniform block (mind padding!)
        size_t              m_byteSize;
    };
}
