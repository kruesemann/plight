#pragma once
#include "plight/include/common/string.h"


namespace Plight::Graphics
{
    enum class EUniformDataType
    {
        Float,
        Int
    };

    struct UniformData
    {
        String                  m_name;

        size_t                  m_size = 0;

        EUniformDataType        m_dataType = EUniformDataType::Float;
    };
}
