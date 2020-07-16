#include "plight/include/graphics/texture.h"

#include "glew/include/glew.h"


namespace Plight::Graphics
{
    /*
        Create a new texture with given data
    */
    Texture
    Texture::create(TextureData const& rData)
    {
        static unsigned int textureUnit = 0;

        Texture result;
        result.m_width = rData.m_width;
        result.m_height = rData.m_height;
        result.m_textureUnit = textureUnit;
        ++textureUnit;

        // Create GL texture
        glGenTextures(1, &result.m_textureObject);

        // Bind texture object to the 2D target in its own texture unit
        glActiveTexture(GL_TEXTURE0 + result.m_textureUnit);
        glBindTexture(GL_TEXTURE_2D, result.m_textureObject);

        // Update with given image
        glTexImage2D(GL_TEXTURE_2D,
                     0, /* mipmap level */
                     GL_RGBA,
                     result.m_width,
                     result.m_height,
                     0, /* border */
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     rData.m_data.empty() ? nullptr : rData.m_data.data() /* image */);

        // Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return result;
    }
}
