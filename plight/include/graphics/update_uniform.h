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

namespace Plight::Graphics::UpdateUniform
{
    /*
        Functions for updating shader uniforms
    */
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
                                       Matrix<float, 4, 4> const&);
    
    /*
        Template function for updating shader uniform buffers
    */
    template<typename DataContainer>
    void                        updateBuffer(Component::UniformBufferData const&,
                                             DataContainer const&);
}
