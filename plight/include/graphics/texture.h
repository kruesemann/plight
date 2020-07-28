#pragma once
#include <vector>


namespace Plight::Graphics
{
    /*
        Texture format
    */
    enum class ETextureFormat
    {
        RGBA,       // Standard color format with opacity for clammed 8-bit values
        RGBA16F     // Floating point framebuffer format for non-clammed 16-bit values
    };

    /*
        Data for generating a new texture
    */
    struct TextureData
    {
        // Color data for each pixel (can be empty)
        std::vector<unsigned char> m_data;

        // Width of the texture in pixels
        int                     m_width = 0;

        // Height of the texture in pixels
        int                     m_height = 0;

        // Texture format
        ETextureFormat          m_textureFormat = ETextureFormat::RGBA;
    };

    /*
        Object managing an OpenGL texture
    */
    struct Texture
    {
        static Texture          create(TextureData const&);

        // Width of the texture in pixels
        int                     m_width = 0;

        // Height of the texture in pixels
        int                     m_height = 0;

        // OpenGL-internal texture object
        unsigned int            m_textureObject = 0;

        // OpenGL-internal texture unit bound to the texture object
        unsigned int            m_textureUnit = 0;
    };
}
