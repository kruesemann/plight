#pragma once
#include "plight/include/common/string.h"

#include "plight/include/graphics/shader.h"


namespace Plight
{
    namespace Graphics
    {
        struct UniformBufferInfo;
    }
}

namespace Plight::Graphics
{
    /*
        Creates, compiles, manages and cleans up GL programs including their uniform variables
    */
    class ShaderManager final
    {
    public:

                                ~ShaderManager();

        Shader const&           getOrCreateShader(String const&,
                                                  std::vector<Graphics::UniformBufferInfo> const&);

    private:

        /*
            Shader type
        */
        enum class EShaderType
        {
            Vertex,
            Fragment
        };

        static unsigned int    createProgram(String const&);

        static unsigned int    createShaderFromString(String const&, EShaderType);

        static String          readStringFromFile(String const&);

        static String          getShaderLog(unsigned int);
        static String          getProgramLog(unsigned int);

        static std::unordered_map<std::string, Component::UniformBufferData> createUniformBuffers(unsigned int,
                                                                                                  std::vector<Graphics::UniformBufferInfo> const&);

        // Map between shader names and data
        std::unordered_map<std::string, Shader> m_shaderMap;

    };
}
