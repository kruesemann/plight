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
                            UniformTextureInfo(String const& rIdentifier,
                                               String const& rName,
                                               Texture const& rTexture)
                                : m_identifier(rIdentifier)
                                , m_name(rName)
                                , m_texture(rTexture) {}

        // Identifier by which the uniform texture can be retrieved
        String              m_identifier;

        // Name of the uniform texture in the shader
        String              m_name;

        // Texture
        Texture             m_texture;
    };
}
