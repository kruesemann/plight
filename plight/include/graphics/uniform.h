#pragma once
#include "plight/include/common/matrix.h"
#include "plight/include/common/vector.h"

#include <vector>


namespace Plight
{
    namespace Component
    {
        struct UniformBufferData;
        struct UniformData;
    }
}

namespace Plight::Graphics::Uniform
{
    void                        update(Component::UniformData const&,
                                       float const&);

    void                        update(Component::UniformData const&,
                                       Vector<float, 2> const&);

    void                        update(Component::UniformData const&,
                                       Vector<float, 3> const&);

    void                        update(Component::UniformData const&,
                                       Vector<float, 4> const&);

    void                        update(Component::UniformData const&,
                                       Matrix<float, 3, 3> const&);

    void                        update(Component::UniformData const&,
                                       Matrix<float, 4 , 4> const&);

    template<typename DataType>
    void                        updateBuffer(Component::UniformBufferData const&,
                                             std::vector<DataType> const&);
}
