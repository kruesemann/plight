#pragma once
#include "plight/include/common/string.h"

#include <unordered_map>


namespace Plight::Graphics
{
    /*
        Creates, compiles, manages and cleans up GL programs
    */
    class ShaderManager final
    {
    public:

                                ~ShaderManager();

        unsigned int            getOrCreateShader(String const&);

    private:

        /*
            Shader type
        */
        enum class EShaderType
        {
            Vertex,
            Fragment
        };

        static unsigned int    createShader(String const&);

        static unsigned int    createShaderFromString(String const&, EShaderType);

        static String          readStringFromFile(String const&);

        static String          getShaderLog(unsigned int);
        static String          getProgramLog(unsigned int);

        // Map between shader names and program id's
        std::unordered_map<std::string, unsigned int> m_programMap;

    };
}
