#include "plight/include/graphics/uniform.h"

#include "glew/include/glew.h"


namespace Plight::Graphics::Uniform
{
    template<typename DataType>
    void
    updateBuffer(unsigned int uniformBufferObject,
                 std::vector<DataType> const& rPaddedData)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferObject);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, rPaddedData.size() * sizeof(DataType), &rPaddedData[0]);
    }

    void
    updateFloatBuffer(unsigned int uniformBufferObject,
                      std::vector<float> const& rPaddedData)
    {
        updateBuffer(uniformBufferObject, rPaddedData);
    }

    void
    updateIntBuffer(unsigned int uniformBufferObject,
                    std::vector<int> const& rPaddedData)
    {
        updateBuffer(uniformBufferObject, rPaddedData);
    }
}
