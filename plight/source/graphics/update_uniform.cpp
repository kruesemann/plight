#include "plight/include/graphics/update_uniform.h"

#include "plight/include/component/uniform_data.h"

#include "glew/include/glew.h"


namespace Plight::Graphics::UpdateUniform
{
    void
    update(Component::UniformData const& rUniformData,
           float const& rData)
    {
        // Use shader
        glUseProgram(rUniformData.m_shaderProgramId);
        glUniform1f(static_cast<int>(rUniformData.m_location), rData);
    }

    void
    update(Component::UniformData const& rUniformData,
           Vector<float, 2> const& rData)
    {
        // Use shader
        glUseProgram(rUniformData.m_shaderProgramId);
        glUniform2fv(static_cast<int>(rUniformData.m_location), 1, rData.m_data.data());
    }

    void
    update(Component::UniformData const& rUniformData,
           Vector<float, 3> const& rData)
    {
        // Use shader
        glUseProgram(rUniformData.m_shaderProgramId);
        glUniform3fv(static_cast<int>(rUniformData.m_location), 1, rData.m_data.data());
    }

    void
    update(Component::UniformData const& rUniformData,
           Vector<float, 4> const& rData)
    {
        // Use shader
        glUseProgram(rUniformData.m_shaderProgramId);
        glUniform4fv(static_cast<int>(rUniformData.m_location), 1, rData.m_data.data());
    }

    void
    update(Component::UniformData const& rUniformData,
           Matrix<float, 3, 3> const& rData)
    {
        // Use shader
        glUseProgram(rUniformData.m_shaderProgramId);
        glUniformMatrix3fv(rUniformData.m_location, 1, GL_FALSE, rData.m_data.data());
    }

    void
    update(Component::UniformData const& rUniformData,
           Matrix<float, 4, 4> const& rData)
    {
        // Use shader
        glUseProgram(rUniformData.m_shaderProgramId);
        glUniformMatrix4fv(rUniformData.m_location, 1, GL_FALSE, rData.m_data.data());
    }
}