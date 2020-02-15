#pragma once
#include "plight/include/graphics/update_uniform.h"

#include "plight/include/component/uniform_data.h"

#include "glew/include/glew.h"


namespace Plight::Graphics::UpdateUniform
{
    
    template<typename DataContainer>
    void
    updateBuffer(Component::UniformBufferData const& rUniformBufferData,
                 DataContainer const& rPaddedData)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, rUniformBufferData.m_uniformBufferObject);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, rPaddedData.size() * sizeof(typename DataContainer::value_type), &rPaddedData[0]);
    }
}
