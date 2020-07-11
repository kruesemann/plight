#pragma once
#include "plight/include/common/string.h"

#include "plight/include/graphics/texture.h"


namespace Plight::Graphics
{
    /*
        Basic data for shader uniform texture initialization
    */
    struct UniformTextureInfo
    {
                            UniformTextureInfo(String const& rName,
                                               Texture const& rTexture)
                                : m_name(rName)
                                , m_texture(rTexture) {}

        // Name of the uniform texture in the shader
        String              m_name;

        // Texture
        Texture             m_texture;
    };
}
