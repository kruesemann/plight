#pragma once
#include <vector>


namespace Plight::Graphics
{
    /*
        Chunk of updated data that is written to a shader uniform buffer
    */
    template<typename DataType>
    struct UniformBufferUpdateData
    {
        // Offset (in bytes) where the updated data should be written
        size_t                  m_offset = 0;

        // Updated data
        std::vector<DataType>   m_data;
    };
}
