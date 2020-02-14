#pragma once
#include <array>


namespace Plight
{
    template<typename DataType, size_t Size>
    class Vector
    {
    public:

        explicit                Vector(std::array<DataType, Size> const& rData)
                                    : m_data(rData) {}

        std::array<DataType, Size> m_data;

    };
}
