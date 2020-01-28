#include "plight/include/graphics/shader_manager.h"

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
        for (auto const& rShader : m_programMap)
            glDeleteProgram(rShader.second);
    }

    /*
        Checks if a shader already exists and in that case returns its program id; creates one in the other case
    */
    unsigned int
    ShaderManager::getOrCreateShader(String const& rName)
    {
        auto const itProgram = m_programMap.find(rName);
        if (itProgram == m_programMap.end())
        {
            auto programId = createShader(rName);
            m_programMap.emplace(rName, programId);
            return programId;
        }
        return itProgram->second;
    }

    /*
        Creates a shader consisting of a vertex and a fragment shader and creates its program id
    */
    unsigned int
    ShaderManager::createShader(String const& rName)
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
}
