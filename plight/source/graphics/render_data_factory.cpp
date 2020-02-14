#include "plight/include/graphics/render_data_factory.h"

#include "plight/include/graphics/attribute.h"
#include "plight/include/graphics/shader_manager.h"

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
        Creates a render data component from a shader name, a list of attributes and a sequence of indices for the render order of vertices
        Performs consistency-checks on the attributes' respective data sizes and dimensions
    */
    Component::RenderData
    create(ShaderManager& rShaderManager,
           String const& rShaderName,
           std::vector<Attribute> const& rAttributes,
           std::vector<int> const& rIndices)
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

        return result;
    }
}
