#include "plight/include/graphics/shader_manager.h"

#include "plight/include/component/uniform_buffer_data.h"

#include "plight/include/graphics/uniform_buffer_info.h"

#include "glew/include/glew.h"

#include <exception>
#include <fstream>


namespace Plight::Graphics
{
    /*
        Helper struct for char*& arguments
    */
    struct CharArray
    {
        CharArray(int size)
            : m_chars(new char[size])
        {
        }

        ~CharArray()
        {
            delete[] m_chars;
        }

        char* m_chars;
    };


    ShaderManager::~ShaderManager()
    {
        // Deallocate programs
        for (auto const& rShader : m_shaderMap)
            glDeleteProgram(rShader.second.m_programId);
    }

    /*
        Creates a shader if it does not already exist and returns it
    */
    Shader const&
    ShaderManager::getOrCreateShader(String const& rName,
                                     std::vector<Graphics::UniformBufferInfo> const& rUniformBufferInfo)
    {
        auto const itShader = m_shaderMap.find(rName);
        if (itShader == m_shaderMap.end())
        {
            auto& rShader = m_shaderMap[rName];
            rShader.m_programId = createProgram(rName);
            rShader.m_uniformBufferDataMap = createUniformBuffers(rShader.m_programId, rUniformBufferInfo);
            return rShader;
        }
        return itShader->second;
    }

    /*
        Creates a shader consisting of a vertex and a fragment shader and creates its program id
    */
    unsigned int
    ShaderManager::createProgram(String const& rName)
    {
        // Create shaders
        auto const shaderSourceFileTemplate = String("resource/%.%").arg(rName);
        auto const vertexShaderId = createShaderFromString(readStringFromFile(shaderSourceFileTemplate.arg("glvs")), EShaderType::Vertex);
        auto const fragmentShaderId = createShaderFromString(readStringFromFile(shaderSourceFileTemplate.arg("glfs")), EShaderType::Fragment);

        // Create program
        auto const programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);

        // Link program
        glLinkProgram(programId);

        // Check program
        auto ok = GL_FALSE;
        glGetProgramiv(programId, GL_LINK_STATUS, &ok);
        if (ok != GL_TRUE)
            throw std::exception(String("Graphics error: %").arg(getProgramLog(programId)).c_str());

        // Clean up
        glDetachShader(programId, vertexShaderId);
        glDetachShader(programId, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return programId;
    }

    /*
        Creates a shader of given type from the given string
    */
    unsigned int
    ShaderManager::createShaderFromString(String const& rShaderString, EShaderType shaderType)
    {
        unsigned int shaderId = 0;

        // Create
        switch (shaderType)
        {
        case EShaderType::Vertex:
            shaderId = glCreateShader(GL_VERTEX_SHADER);
            break;
        case EShaderType::Fragment:
            shaderId = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }

        // Set source
        auto const* source = rShaderString.c_str();
        glShaderSource(shaderId, 1, &source, NULL);

        // Compile
        glCompileShader(shaderId);

        // Check
        auto ok = GL_FALSE;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ok);
        if (ok != GL_TRUE)
            throw std::exception(String("Graphics error: Unable to compile shader %:\n%\n\n%")
                                 .arg(shaderId)
                                 .arg(rShaderString)
                                 .arg(getShaderLog(shaderId)).c_str());

        return shaderId;
    }

    /*
        Reads file and writes contents to a string
    */
    String
    ShaderManager::readStringFromFile(String const& rFullFilePath)
    {
        String result;
        std::ifstream file(rFullFilePath.c_str());

        if (!file)
            throw std::exception(String("IO error: Unable to open file %").arg(rFullFilePath).c_str());

        // Get shader source
        result.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        return result;
    }

    /*
        Get the compile log for given shader
    */
    String
    ShaderManager::getShaderLog(unsigned int shaderId)
    {
        String log;

        if (glIsShader(shaderId))
        {
            // Get info string length
            int infoLogLength = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

            // Get info log
            CharArray infoLog(infoLogLength);
            glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, infoLog.m_chars);
            log = String(infoLog.m_chars);
        }
        return log;
    }

    /*
        Get the compile log for given program
    */
    String
    ShaderManager::getProgramLog(unsigned int programId)
    {
        String log;
        if (glIsProgram(programId))
        {
            // Get info string length
            int infoLogLength = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

            // Get info log
            CharArray infoLog(infoLogLength);
            glGetProgramInfoLog(programId, infoLogLength, &infoLogLength, infoLog.m_chars);
            log = String(infoLog.m_chars);
        }
        return log;
    }

    /*
        Creates given uniform buffers
    */
    std::unordered_map<std::string, Component::UniformBufferData>
    ShaderManager::createUniformBuffers(unsigned int programId, std::vector<Graphics::UniformBufferInfo> const& rUniformBufferInfo)
    {
        std::unordered_map<std::string, Component::UniformBufferData> result;

        for (auto const& rInfo : rUniformBufferInfo)
        {
            if (result.find(rInfo.m_name) != result.end())
                throw std::exception(String("Graphics error: Multiple uniform buffers for '%' not allowed.")
                                     .arg(rInfo.m_name).c_str());

            auto& rData = result[rInfo.m_name];

            // Find uniform block location
            auto const location = glGetUniformBlockIndex(programId, rInfo.m_name.c_str());
            if (location == GL_INVALID_INDEX)
                throw std::exception(String("Graphics error: Failed to retrieve %'s location.")
                                     .arg(rInfo.m_name).c_str());

            unsigned int bindingPoint = 0;
            if (rInfo.m_shareBetweenShaders)
            {
                static std::unordered_map<std::string, UniformBufferObjectData> uniformBlockBufferObjectDataMap;

                auto const itBufferObject = uniformBlockBufferObjectDataMap.find(rInfo.m_name);
                if (itBufferObject == uniformBlockBufferObjectDataMap.end())
                {
                    auto const uniformBufferObjectData = createUniformBuffer(rInfo);
                    uniformBlockBufferObjectDataMap.emplace(rInfo.m_name, uniformBufferObjectData);
                    bindingPoint = uniformBufferObjectData.m_bindingPoint;
                    rData.m_uniformBufferObject = uniformBufferObjectData.m_uniformBufferObject;
                }
                else if (itBufferObject->second.m_byteSize != rInfo.m_byteSize)
                {
                    throw std::exception(String("Graphics error: Inconsistent shared buffer size (%: % vs %).")
                                         .arg(rInfo.m_name)
                                         .arg(rInfo.m_byteSize)
                                         .arg(itBufferObject->second.m_byteSize).c_str());
                }
                else
                {
                    bindingPoint = itBufferObject->second.m_bindingPoint;
                    rData.m_uniformBufferObject = itBufferObject->second.m_uniformBufferObject;
                }
            }
            else
            {
                auto const uniformBufferObjectData = createUniformBuffer(rInfo);
                bindingPoint = uniformBufferObjectData.m_bindingPoint;
                rData.m_uniformBufferObject = uniformBufferObjectData.m_uniformBufferObject;
            }

            glUniformBlockBinding(programId, location, bindingPoint);

        }

        return result;
    }

    /*
        Creates given uniform buffer
    */
    ShaderManager::UniformBufferObjectData
    ShaderManager::createUniformBuffer(Graphics::UniformBufferInfo const& rInfo)
    {
        static unsigned int nextBindingPoint = 0;

        UniformBufferObjectData result;
        result.m_bindingPoint = nextBindingPoint;
        result.m_byteSize = rInfo.m_byteSize;

        glGenBuffers(1, &result.m_uniformBufferObject);

        glBindBuffer(GL_UNIFORM_BUFFER, result.m_uniformBufferObject);
        glBufferData(GL_UNIFORM_BUFFER, result.m_byteSize, NULL, GL_STATIC_DRAW);

        glBindBufferRange(GL_UNIFORM_BUFFER, result.m_bindingPoint, result.m_uniformBufferObject, 0, result.m_byteSize);

        ++nextBindingPoint;
        return result;
    }
}
