#pragma once
#include "plight/include/graphics/update_uniform.h"

#include "plight/include/component/uniform_buffer_data.h"
#include "plight/include/component/uniform_texture_data.h"

#include "glew/include/glew.h"


namespace Plight::Graphics
{
    /*
        Function for updating shader uniform buffers
    */
    void
    updateUniformBuffer(Component::UniformBufferData const& rUniformBufferData)
    {
        auto const updateBuffer = [](auto const& rUniformUpdateData)
        {
            if (rUniformUpdateData.m_data.empty())
                return;

            glBufferSubData(GL_UNIFORM_BUFFER,
                            rUniformUpdateData.m_offset,
                            rUniformUpdateData.m_data.size() * sizeof(decltype(rUniformUpdateData.m_data)::value_type),
                            &rUniformUpdateData.m_data[0]);
        };

        glBindBuffer(GL_UNIFORM_BUFFER, rUniformBufferData.m_uniformBufferObject);

        /*for (auto const& rBoolUpdateData : rUniformBufferData.m_boolUpdateData)
            updateBuffer(rBoolUpdateData);*/
        for (auto const& rIntUpdateData : rUniformBufferData.m_intUpdateData)
            updateBuffer(rIntUpdateData);
        for (auto const& rUintUpdateData : rUniformBufferData.m_uintUpdateData)
            updateBuffer(rUintUpdateData);
        for (auto const& rFloatUpdateData : rUniformBufferData.m_floatUpdateData)
            updateBuffer(rFloatUpdateData);
        for (auto const& rDoubleUpdateData : rUniformBufferData.m_doubleUpdateData)
            updateBuffer(rDoubleUpdateData);
    }

    /*
        Function for updating shader uniform textures
    */
    void
    updateUniformTexture(Component::UniformTextureData const& rUniformTextureData)
    {
        glUniform1i(rUniformTextureData.m_uniformLocation, rUniformTextureData.m_textureUnit);
    }
}
