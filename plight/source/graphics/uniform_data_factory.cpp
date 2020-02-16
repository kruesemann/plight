#include "plight/include/graphics/uniform_data_factory.h"

#include "plight/include/graphics/shader_manager.h"
#include "plight/include/graphics/uniform_info.h"

#include "glew/include/glew.h"

#include <exception>


namespace Plight::Graphics::UniformDataFactory
{
    /*
        Initialize shader uniform and return component for updates
    */
    Component::UniformData
    create(ShaderManager& rShaderManager,
           String const& rShaderName,
           Graphics::UniformInfo const& rUniformInfo)
    {
        Component::UniformData result;
        result.m_shaderProgramId = rShaderManager.getOrCreateShader(rShaderName);

        result.m_location = glGetUniformLocation(result.m_shaderProgramId, rUniformInfo.m_name.c_str());
        if (result.m_location < 0)
            throw std::exception(String("Graphics error: Failed to retrieve %'s location.")
                                 .arg(rUniformInfo.m_name).c_str());

        return result;
    }

    /*
        Initialize shader uniform buffer and return component for updates
    */
    Component::UniformBufferData
    create(ShaderManager& rShaderManager,
           String const& rShaderName,
           Graphics::UniformBufferInfo const& rUniformBufferInfo)
    {
        Component::UniformBufferData result;
        auto const shaderProgramId = rShaderManager.getOrCreateShader(rShaderName);

        // Find uniform block locations
        auto const location = glGetUniformBlockIndex(shaderProgramId, rUniformBufferInfo.m_name.c_str());
        if (location == GL_INVALID_INDEX)
            throw std::exception(String("Graphics error: Failed to retrieve %'s location.")
                                 .arg(rUniformBufferInfo.m_name).c_str());

        // Create uniform buffers
        static unsigned int bindingPoint = 0;

        glUniformBlockBinding(shaderProgramId, location, bindingPoint);

        glGenBuffers(1, &result.m_uniformBufferObject);

        glBindBuffer(GL_UNIFORM_BUFFER, result.m_uniformBufferObject);
        glBufferData(GL_UNIFORM_BUFFER, rUniformBufferInfo.m_rawSize, NULL, GL_STATIC_DRAW);

        glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, result.m_uniformBufferObject, 0, rUniformBufferInfo.m_rawSize);
        ++bindingPoint;

        return result;
    }
}
