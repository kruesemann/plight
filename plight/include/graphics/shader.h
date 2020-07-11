#pragma once
#include <unordered_map>


namespace Plight::Component
{
    struct UniformBufferData;
    struct UniformTextureData;
}

namespace Plight::Graphics
{
    /*
        Holds data necessary for creating RenderData and UniformBufferData-holding components
    */
    struct Shader
    {
        // OpenGL-internal shader program id
        unsigned int            m_programId = 0;

        // Map between uniform block names and the data of the uniform buffer to which the block is bound
        std::unordered_map<std::string, Component::UniformBufferData> m_uniformBufferDataMap;

        // Map between uniform texture names and the data of the uniform texture to which the uniform is bound
        std::unordered_map<std::string, Component::UniformTextureData> m_uniformTextureDataMap;
    };
}
