#include "plight/include/graphics/render_data_factory.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/shader_manager.h"
#include "plight/include/graphics/uniform_data.h"

#include "glew/include/glew.h"

#include <exception>


namespace Plight::Graphics::RenderDataFactory
{
    /*
        Attribute data for creating a vertex array
    */
    struct AttributeInfo
    {
        int                 m_location;
        size_t              m_dimension;
    };

    /*
        Uniform data for creating a uniform buffer
    */
    struct UniformInfo
    {
        unsigned int        m_location;
        size_t              m_size;
        EUniformDataType    m_dataType;
    };

    /*
        Compute number of components for the accumulated data of one vertex
        Performs consistency-checks on the attributes' respective data sizes and dimensions
    */
    size_t
    computeVertexDimension(std::vector<Attribute> const& rAttributes, size_t vertexNumber)
    {
        size_t result = 0;
        for (auto const& rAttribute : rAttributes)
        {
            if (rAttribute.m_data.size() % rAttribute.m_dimension != 0)
                throw std::exception(String("Graphics error: Mismatch between data and dimension for vertex attribute '%'")
                                     .arg(rAttribute.m_name).c_str());

            if (rAttribute.m_data.size() / rAttribute.m_dimension != vertexNumber)
                throw std::exception(String("Graphics error: Mismatch between vertex attributes. Attribute '%' differs in number of vertices (has %, should have %)")
                                     .arg(rAttribute.m_name)
                                     .arg(rAttribute.m_data.size() / rAttribute.m_dimension)
                                     .arg(vertexNumber).c_str());

            result += rAttribute.m_dimension;
        }
        return result;
    }

    /*
        Accumulates the attributes into one data array passed to OpenGL
    */
    std::vector<float>
    accumulateAttributes(std::vector<Attribute> const& rAttributes, size_t vertexNumber, size_t vertexDimension)
    {
        std::vector<float> result(vertexNumber * vertexDimension, 0.0f);
        size_t offset = 0;
        for (auto const& rAttribute : rAttributes)
        {
            for (size_t vertex = 0; vertex < vertexNumber; ++vertex)
            {
                auto const accumulatedIndex = vertex * vertexDimension + offset;
                auto const attributeIndex = vertex * rAttribute.m_dimension;

                for (size_t component = 0; component < rAttribute.m_dimension; ++component)
                    result.at(accumulatedIndex + component) = rAttribute.m_data.at(attributeIndex + component);
            }
            offset += rAttribute.m_dimension;
        }
        return result;
    }

    /*
        Creates a uniform buffer object
    */
    template<class UniformContainer>
    void
    createUniform(UniformContainer& rTargetContainer, unsigned int shaderProgramId, int location, size_t size)
    {
        auto& rResult = rTargetContainer.emplace_back();

        glUniformBlockBinding(shaderProgramId, location, 0);

        glGenBuffers(1, &rResult.m_uniformBufferObject);

        glBindBuffer(GL_UNIFORM_BUFFER, rResult.m_uniformBufferObject);
        glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), NULL, GL_STATIC_DRAW);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, rResult.m_uniformBufferObject, 0, size);
    }

    /*
        Creates a render data component from a shader name, a list of attributes, a sequence of indices for the render order of vertices and a list of uniforms
        Performs consistency-checks on the attributes' respective data sizes and dimensions
    */
    Component::RenderData
    create(ShaderManager& rShaderManager,
           String const& rShaderName,
           std::vector<Attribute> const& rAttributes,
           std::vector<int> const& rIndices,
           std::vector<UniformData> const& rUniformData)
    {
        Component::RenderData result;
        result.m_shaderProgramId = rShaderManager.getOrCreateShader(rShaderName);
        result.m_vertexVisits = rIndices.size();

        if (rAttributes.empty())
            return result;

        size_t const vertexNumber = rAttributes.at(0).m_data.size() / rAttributes.at(0).m_dimension;
        auto const vertexDimension = computeVertexDimension(rAttributes, vertexNumber);
        auto const accumulatedAttributes = accumulateAttributes(rAttributes, vertexNumber, vertexDimension);

        // Find attribute locations
        std::vector<AttributeInfo> attributeInfos;
        for (auto const& rAttribute : rAttributes)
        {
            auto const location = glGetAttribLocation(result.m_shaderProgramId, rAttribute.m_name.c_str());
            if (location < 0)
                throw std::exception(String("Graphics error: Failed to retrieve %'s location.")
                                     .arg(rAttribute.m_name).c_str());
            attributeInfos.push_back(AttributeInfo{location, rAttribute.m_dimension});
        }

        // Find uniform block locations
        std::vector<UniformInfo> uniformInfos;
        for (auto const& rUniform : rUniformData)
        {
            auto const location = glGetUniformBlockIndex(result.m_shaderProgramId, rUniform.m_name.c_str());
            if (location == GL_INVALID_INDEX)
                throw std::exception(String("Graphics error: Failed to retrieve %'s location.")
                                     .arg(rUniform.m_name).c_str());
            uniformInfos.push_back(UniformInfo{location, rUniform.m_size, rUniform.m_dataType});
        }

        // Create vertex array and its buffers
        unsigned int vertexBufferObject = 0;
        unsigned int indexBufferObject = 0;
        glGenVertexArrays(1, &result.m_vertexArrayObject);
        glGenBuffers(1, &vertexBufferObject);
        glGenBuffers(1, &indexBufferObject);

        glBindVertexArray(result.m_vertexArrayObject);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, accumulatedAttributes.size() * sizeof(float), &accumulatedAttributes[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, rIndices.size() * sizeof(int), &rIndices[0], GL_STATIC_DRAW);

        // Pass attribute meta data to OpenGL
        auto const vertexSize = static_cast<GLsizei>(vertexDimension * sizeof(float));
        size_t offset = 0;
        for (auto const& rAttributeInfo : attributeInfos)
        {
            glVertexAttribPointer(rAttributeInfo.m_location, static_cast<GLint>(rAttributeInfo.m_dimension), GL_FLOAT, GL_FALSE, vertexSize, (void*)offset);
            glEnableVertexAttribArray(rAttributeInfo.m_location);
            offset += rAttributeInfo.m_dimension * sizeof(float);
        }

        // Create uniform buffers
        for (auto const& rUniformInfo : uniformInfos)
        {
            switch (rUniformInfo.m_dataType)
            {
            case EUniformDataType::Float:
                createUniform(result.m_floatUniformBufferData, result.m_shaderProgramId, rUniformInfo.m_location, rUniformInfo.m_size * sizeof(float));
                break;
            case EUniformDataType::Int:
                createUniform(result.m_intUniformBufferData, result.m_shaderProgramId, rUniformInfo.m_location, rUniformInfo.m_size * sizeof(int));
                break;
            }
        }

        return result;
    }
}
