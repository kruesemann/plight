#pragma once
#include <array>


namespace Plight
{
    /*
        Basic matrix template
    */
    template<typename DataType, size_t Width, size_t Height>
    class Matrix
    {
    public:

        explicit                Matrix(std::array<DataType, Width * Height> const& rData)
                                    : m_data(rData) {}

        std::array<DataType, Width * Height> m_data;

    };
}
